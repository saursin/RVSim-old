#include <iostream>
#include <string>
#include <stdint.h>

#include "cxxopts.hpp"

#include "SimInfo.h"
#include "Util.h"
#include "RVdefs.h"
#include "SimError.h"
#include "Bus.h"
#include "Memory.h"
#include "RVCPU.h"

// ============ Global variables ==============
// Flags
bool verbose_flag;
bool debug_mode;

unsigned long int maxitr;
unsigned long int mem_size;

std::string ifile = "";
std::string signature_file = "";



// Object pointers
Bus<REG> * bus;
Memory * mem;
RVCPU * cpu;

/** 
 * @brief Exit simulator
 */
void SimError::Exit(int status)
{
    if(bus)
        bus->~Bus();
    if(mem)
        mem->~Memory();
    if(cpu)
        cpu->~RVCPU();

    exit(status);
}

/**
 * @brief parses command line arguments given to the assembler and 
 * accordingly sets appropriate internal flags/variables and/or displays 
 * info messages.
 * 
 * @param argc argument count
 * @param argv argument vector
 * @param infile input file name (pointer)
 */
void parse_commandline_args(int argc, char**argv, std::string &infile)
{
	try
	{
		// Usage Message Header
		cxxopts::Options options(argv[0], std::string(sim_version_info)+"\nA RISC-V ISA Simulator\n");
		
		options.positional_help("input").show_positional_help();

		// Adding CLI options
		options.add_options("General")
		("h,help", "Show this message")
		("version", "Show version information")
		("i,input", "Specify an input file", cxxopts::value<std::string>(infile));
		
		options.add_options("Config")
		("maxitr", "Specify maximum simulation iterations", cxxopts::value<unsigned long int>(maxitr)->default_value(std::to_string(100000)))
		("memsize", "Specify size of memory to simulate", cxxopts::value<unsigned long int>(mem_size)->default_value(std::to_string(65536)))
		//("uart-broadcast", "enable uart broadcasting over", cxxopts::value<unsigned long int>(mem_size)->default_value(std::to_string(default_mem_size)))
		;

		options.add_options("Debug")
		("v,verbose", "Turn on verbose output", cxxopts::value<bool>(verbose_flag)->default_value("false"))
		("d,debug", "Start in debug mode", cxxopts::value<bool>(debug_mode)->default_value("false"))
		("signature", "Enable signature sump at hault (Used for riscv compliance tests)", cxxopts::value<std::string>(signature_file)->default_value(""))
		;


	    options.parse_positional({"input"});

		//options.allow_unrecognised_options();
		
		// parse CLI options
		auto result = options.parse(argc, argv);

		if(result.unmatched().size() != 0)
		{
			std::string unknown_args;
			for(unsigned int i=0; i<result.unmatched().size(); i++)
				unknown_args = unknown_args + result.unmatched()[i] + (i==result.unmatched().size()-1 ? "" :", ");
			SimError::throwError("Unrecognized arguments [" + unknown_args + "]", true);
		}

		if (result.count("help"))
		{
			std::cout << options.help() << std::endl;
			exit(EXIT_SUCCESS);
		}
		if (result.count("version"))
		{
			std::cout << sim_version_info << std::endl << sim_copyright_info << std::endl;
			exit(EXIT_SUCCESS);
		}
		if (result.count("input")>1)
		{
			SimError::throwError("Multiple input files specified", true);
		}
		if (result.count("input")==0)
		{
			SimError::throwError("No input files specified", true);
		}

		if (verbose_flag)
			std::cout << "Input File: " << infile << "\n";

	}
	catch(const cxxopts::OptionException& e)
	{
		SimError::throwError("Error while parsing command line arguments: " + (std::string)e.what(), true);
	}	
}



// Implement Bus
template <class REG>
REG Bus<REG>::request(REG address, REG data, int sel, bool write) 
{
    if(write)
    {
        if(sel & 0b0001)
            mem->store(address, (uint8_t)(data & 0x000000ff));
        
        if(sel & 0b0010)
            mem->store(address+1, (uint8_t)(data & 0x0000ff00)>>8);

        if(sel & 0b0100)
            mem->store(address+2, (uint8_t)(data & 0x00ff0000)>>16);

        if(sel & 0b1000)
            mem->store(address+3, (uint8_t)(data & 0xff000000)>>24);

        return 0;
    }
    else
    {
        REG rdata = 0;

        if(sel & 0b0001)
            rdata |= (REG) mem->fetch(address);
        
        if(sel & 0b0010)
            rdata |= ((REG) mem->fetch(address+1)) << 8;

        if(sel & 0b0100)
            rdata |= ((REG) mem->fetch(address+2)) << 16;

        if(sel & 0b1000)
            rdata |= ((REG) mem->fetch(address+3)) << 24;

        return rdata;
    }
}



int main(int argc, char ** argv)
{
    // Parse CLI Arguments
    parse_commandline_args(argc, argv, ifile);

    // Create memory object
    mem = new Memory(65536);

    // Create a new RVCPU object
    ISAdef cpu_isa_definition = 
    {
        false, // ISA_EMBEDDED
        false, // ISA_M
        false, // ISA_A
        false, // ISA_F
        false, // ISA_D
        false  // ISA_C
    };
    
    cpu = new RVCPU(0x00000000, cpu_isa_definition, bus);
	
	// Run simulation
	if(debug_mode)
	{
		std::string input;
		while(true)
		{
			// Parse Input
			std::cout << ": ";
			getline(std::cin, input);
			
			// Tokenize
			std::vector<std::string> token;
			Util::tokenize(input, token, ' ');

			// Parse Command
			if((token[0] == "q") | (token[0] == "quit"))
			{
				// Quit simulator
				std::cout << "Exiting due to user interuption\n";
				SimError::Exit(EXIT_SUCCESS);
			}
			else if(token[0] == "r")
			{
				// Run indefinitely
				cpu->run(-1);
			}
			else if(token[0] == "rst")
			{
				// Reset Simulator
				cpu->reset();
			}
			else if(token[0] == "")
			{
				// Run for 1 cycles
				cpu->run(1);
			}
			else if(token[0] == "for")
			{
				// run for specified cycles
				if(token.size()<2)
					SimError::throwError("\"for\" command expects one argument\n");
				else
					cpu->run(std::stoi(token[1]));
			}
			else if(token[0] == "verbose-on")
			{
				// turn on verbose
				verbose_flag = true;
			}
			else if(token[0] == "verbose-off")
			{
				// turn on verbose
				verbose_flag = false;
			}
			else
			{
				SimError::throwError("Unknown command \"" + token[0] + "\"\n");
			}
			input.clear();
		}
	}
	else
	{
		cpu->run(-1);
	}

	// Control must never Reach Here //
	SimError::throwError("PROGRAM CONTROL FAULT", true);
	return 0;
}