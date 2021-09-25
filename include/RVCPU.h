#ifndef __RVCPU_H__
#define __RVCPU_H__

#include "RVdefs.h"
#include "Bus.h"

class RVCPU
{
    private:
    /**
     * @brief ISA definition for the CPU
     */
    ISAdef CPU_ISA;

    /**
     * @brief Number of registers in the CPU
     */
    unsigned int nRegs;

    /**
     * @brief Reset address for the program counter
     */
    REG PC_RESET_ADDR = 0x00000000;

    /**
     * @brief Struct defining architectural state of the processor
     */
    struct RVState
    {
        REG PC;
        REG X[XLEN];
    } state;

    /**
     * @brief Bus object
     * 
     */
    Bus<REG> * bus;

    public:
    /**
     * @brief Construct a new RVCPU object
     * 
     * @param pc_init_address program counter reset address
     * @param ISA_def RISC-V ISA definition
     * @param system_bus bus object pointer
     */
    RVCPU(REG pc_init_address, ISAdef ISA_def, Bus<REG> * system_bus);
    
    /**
     * @brief Get the value of the specified register
     * 
     * @param reg_no register number
     * @return REG value stored in tegister
     */
    REG getRegValue(unsigned int reg_no);

    /**
     * @brief Get value of program counter
     * 
     * @return REG 
     */
    REG getPCValue();

    /**
     * @brief Reset CPU
     */
    void reset();

    /**
     * @brief Step CPU by one cycle
     */
    void step();
};

#endif // __RVCPU_H__