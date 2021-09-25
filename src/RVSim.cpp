#include <iostream>
#include <string>
#include <stdint.h>

#include "RVdefs.h"
#include "SimError.h"
#include "Bus.h"
#include "Memory.h"
#include "RVCPU.h"


// Object pointers
Bus<REG> * bus;
Memory * mem;
RVCPU * cpu;

// Exit simulator
void SimError::Exit(int status)
{
    bus->~Bus();
    mem->~Memory();
    cpu->~RVCPU();

    exit(status);
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
    // Fetch CLI Arguments


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

    for(int i=0; i<10; i++)
    {
        cpu->step();
    }

    SimError::Exit(EXIT_SUCCESS);
}