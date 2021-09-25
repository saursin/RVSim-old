#include <vector>

#include "Memory.h"
#include "SimError.h"
#include "elfio.hpp"

/**
 * @brief Construct a new Memory object
 * 
 * @param max_addr size of memory
 */
Memory::Memory(uint64_t max_addr)
{
    size = max_addr;

    // Allocate memory
    if(!(mem = new uint8_t[max_addr])) 
    {
        SimError::throwError("Failed to allocate memory object", true);
    }
}


/**
 * @brief Destroy the Memory object
 * 
 */
Memory::~Memory()
{
    delete [] mem;
    size = 0;
}


/**
 * @brief Check if the address is valid
 * 
 * @param addr address
 * @return true if address is within bounds
 * @return false if address is out of bounds
 */
bool Memory::isValidAddress(uint64_t addr)
{
    return (addr < size);
}


/**
 * @brief Fetch an 8-bit byte from memory
 * 
 * @param addr address
 * @return uint8_t data
 */
uint8_t Memory::fetch(uint64_t addr)
{
    if(!isValidAddress(addr))
    {
        char errmsg[40];
        sprintf(errmsg, "Address out of bounds : 0x%08x", (unsigned int)addr);
        SimError::throwError(errmsg, true);
        return 0;
    }
    return (uint8_t) mem[addr];
}


/**
 * @brief Store a 8-bit byte to memory
 * 
 * @param addr address
 * @param byte byte
 */
void Memory::store(uint64_t addr, uint8_t byte)
{
    if(!isValidAddress(addr))
    {
        char errmsg[40];
        sprintf(errmsg, "Address out of bounds : 0x%08x", (unsigned int)addr);
        SimError::throwError(errmsg, true);
        return;
    }
    mem[addr] = byte;
}

/**
 * @brief Initialize memory from an elf file
 * only sections that match flag signatures are loaded
 * 
 * @param ifile filename
 * @param flags_signatures allowed flag signatures
 */
unsigned int Memory::initFromElf(std::string ifile, std::vector<int> flags_signatures)
{
    // Initialize Memory object from input ELF File
    ELFIO::elfio reader;

    // Load file into elf reader
    if (!reader.load(ifile)) 
    {
        SimError::throwError("Can't find or process ELF file : " + ifile + "\n", true);
    }

    // Check ELF Class, Endiness & segment count
    if(reader.get_class() != ELFCLASS32)
        SimError::throwError("Elf file format invalid: should be 32-bit elf\n", true);
    if(reader.get_encoding() != ELFDATA2LSB)
        SimError::throwError("Elf file format invalid: should be little Endian\n", true);

    ELFIO::Elf_Half seg_num = reader.segments.size();

    if(seg_num == 0)
        SimError::throwError("Elf file format invalid: should consist of atleast one section\n", true);


    // Read elf and initialize memory
    //if(verbose_flag)
    //	std::cout << "Segments found : "<< seg_num <<"\n";

    unsigned int i = 0;
    while (i < seg_num) // iterate over all segments
    {
        const ELFIO::segment * seg = reader.segments[i];

        // Get segment properties
        
        if (seg->get_type() == SHT_PROGBITS)
        {
            int seg_flags = reader.segments[i]->get_flags();

            if(flags_signatures.end() != std::find(flags_signatures.begin(), flags_signatures.end(), seg_flags))	// Flag found in signature list
            {

                const char* seg_data = reader.segments[i]->get_data();
                const uint seg_size = reader.segments[i]->get_file_size();
                ELFIO::Elf64_Addr seg_strt_addr = reader.segments[i]->get_physical_address();

                //if(verbose_flag)
                //	printf("Loading Segment %d @ 0x%08x --- ", i, (unsigned int) reader.segments[i]->get_physical_address());
                
                long unsigned int offset = 0;
                while(offset<seg_size)
                {
                    store(seg_strt_addr + offset, seg_data[offset]);
                    offset++;
                }

                //if(verbose_flag)
                //	printf("done\n");
            }
        }
        i++;
    }
    return (unsigned int) reader.get_entry();
}