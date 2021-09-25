#include <iostream>

#include "RVCPU.h"


/**
 * @brief Construct a new RVCPU object
 * 
 * @param pc_init_address program counter reset address
 * @param ISA_def RISC-V ISA definition
 * @param system_bus bus object pointer
 */
RVCPU::RVCPU(REG pc_init_address, ISAdef ISA_def, Bus<REG> *system_bus)
{
    PC_RESET_ADDR = pc_init_address;
    CPU_ISA = ISA_def;
    nRegs = ISA_def.ISA_EMBEDDED ? XLEN/2 : XLEN;
    bus = system_bus;
}


/**
 * @brief Get the value of the specified register
 * 
 * @param reg_no register number
 */
REG RVCPU::getRegValue(unsigned int reg_no)
{
    return state.X[reg_no];
}


/**
 * @brief Get value of program counter
 */
REG RVCPU::getPCValue()
{
    return state.PC;
}

/**
 * @brief Reset CPU
 */
void RVCPU::reset()
{
    // Reset PC
    state.PC = PC_RESET_ADDR;

    // Clear registers
    for(unsigned int i=0; i<nRegs; i++)
    {
        state.X[i] = 0;
    }
}

/**
 * @brief Step CPU by one cycle
 */
void RVCPU::step()
{
    std::cout <<"step" <<"\n";
}