#include "Util.h"

#include <fstream>
#include <sstream>



/**
 * @brief removes preceeding whitespaces in a string
 * 
 * @param s string
 * @return std::string 
 */
std::string Util::lStrip(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}


/**
 * @brief removes succeeding whitespaces in a string
 * 
 * @param s string
 * @return std::string 
 */
std::string Util::rStrip(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}


/**
 * @brief removes preceding & succeeding whitespaces in a string
 * 
 * @param s string
 * @return std::string 
 */
std::string Util::strip(const std::string& s)
{
    return rStrip(lStrip(s));
}


// =============================== STRING TOKENIZING =====================================
/**
 * @brief splits a string accordint to delimiter 
 * 
 * @param txt input string
 * @param strs vector of strings parts
 * @param ch delimiter
 * @return size_t 
 */
size_t Util::tokenize(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

// =============================== FILE READER =====================================
/**
 * @brief reads a binary file
 * 
 * @param memfile filepath
 * @return std::vector<char> contents
 */
std::vector<char> Util::fReadBin(std::string memfile)
{        
    std::vector<char> fcontents;
    std::ifstream f (memfile, std::ios::out | std::ios::binary);
    
    if(!f)
    {
        throw "file access failed";
    }
    try
    {
        while(!f.eof())
        {    
            char byte;
            f.read((char *) &byte, 1);
            fcontents.push_back(byte);
        }
    }
    catch(...)
    {
        throw "file reading failed!";
    }
    f.close();
    return fcontents;
}


/**
 * @brief Reads a file and returns its contents
 * 
 * @param filepath Filepath
 * @return Vector of strings containing file contents
 */
std::vector<std::string> Util::fRead (std::string filepath)
{
    // returns a vector of strings
    std::vector<std::string> text;

    // input file stream
    std::ifstream fin(filepath.c_str());
    if(!fin){
        throw "file access failed";
    }

    // reading file line by line and appending into the vector of strings
    std::string raw_line;
    while(getline(fin,raw_line))
    {
        text.push_back(raw_line);
    }

    // close file
    fin.close();
    return text;
}


// =================================== FILE WRITER ====================================
/**
 * @brief Write to a file
 * 
 * @param filepath Filepath
 */
void Util::fWrite (std::vector<std::string> data, std::string filepath)
{
    std::ofstream File(filepath);
    if(!File)
    {
        throw "file writing failed";
    }
    for(unsigned int i=0; i<data.size(); i++)
    {
        File << data[i] <<"\n";
    }
    File.close();
}

// ================================ Runtime disassembly =================================
/**
 * @brief Get the Stdout From shell Command
 * 
 * @param cmd shell command to execute
 * @return std::string command output
 */
std::string Util::GetStdoutFromCommand(std::string cmd) {

  std::string data;
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  cmd.append(" 2>&1");

  stream = popen(cmd.c_str(), "r");

  if (stream) {
    while (!feof(stream))
      if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
  }
  return data;
}

/**
 * @brief Get the Disassembly of input file using riscv objdump
 * 
 * @param filename input filename
 * @return std::map<uint32_t, std::string> map of disassembly
 */
std::map<uint32_t, Util::DisassembledLine> Util::getDisassembly(std::string filename)
{
	std::string command = "";
	#ifdef RV32_COMPILER
		command +="riscv32-unknown-elf-objdump -d ";
	#else
		command += "riscv64-unknown-elf-objdump -d ";
	#endif
	command+=filename;
	
	// Get command output
	std::string output = GetStdoutFromCommand(command);
	
	std::stringstream s(output);

	// Parse command output
	std::map<uint32_t, DisassembledLine> dis;
	
	std::string line;
	while(std::getline(s, line))
	{        
        if(strip(line).length() == 0)
            continue;

        if(!(line.back() == ':' || line[0] != ' '))
        {
    
            line = strip(line);
            unsigned int colonAt = line.find(':');

            uint32_t addr = std::stoi(strip(line.substr(0, colonAt)), 0, 16);
            DisassembledLine d;
            d.instr = (uint32_t)std::stol(strip(line.substr(colonAt+1, colonAt+15)), 0, 16);
            d.disassembly = strip(line.substr(colonAt+16));

            dis.insert({addr, d});
        }
	}
	return dis;
}
