#ifndef __RVDEFS_H__
#define __RVDEFS_H__

// Datatype for a register
#ifdef RV_XLEN_32
    const int XLEN = 32;
    using REG = uint32_t;
    using REGS = int32_t;
#else
    const int XLEN = 64;
    using REG = uint64_t;
    using REGS = int64_t;
#endif

// Other data types
using Byte      = uint8_t;
using halfWord  = uint16_t;
using Word      = uint32_t;
using Double    = uint64_t;

/**
 * @brief RISC-V ISA definition
 * 
 */
struct ISAdef
{
    bool ISA_EMBEDDED;  // Uses half the registers
   
    bool ISA_M; // Multiply
    bool ISA_A; // Atomic
    bool ISA_F; // Single Precision Floating point
    bool ISA_D; // Double Precision Floating point
    bool ISA_C; // Compressed
};

#endif // __RVDEFS_H__