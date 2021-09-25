#ifndef __MEMORY_H__
#define __MEMORY_H__
#include <stdint.h>
#include <string>
#include <vector>

/**
 * @brief Memory class
 * This class is used to emulate the memories in simulation
 * 
 */
class Memory
{
	public:
	/**
	 * @brief pointer to memory array
	 * 
	 */
	uint8_t * mem;

	/**
	 * @brief size of memory
	 * 
	 */
	uint64_t size;


	/**
	 * @brief Construct a new Memory object
	 * 
	 * @param max_addr size of memory
	 */
	Memory(uint64_t max_addr);

	/**
	 * @brief Destroy the Memory object
	 * 
	 */
	~Memory();

	/**
	 * @brief Check if the address is valid
	 * 
	 * @param addr address
	 * @return true if address is within bounds
	 * @return false if address is out of bounds
	 */
	bool isValidAddress(uint64_t addr);

	/**
	 * @brief Fetch an 8-bit byte from memory
	 * 
	 * @param addr address
	 * @return uint8_t data
	 */
	uint8_t fetch(uint64_t addr);

	/**
	 * @brief Store a 8-bit byte to memory
	 * 
	 * @param addr address
	 * @param byte byte
	 */
	void store(uint64_t addr, uint8_t byte);

	/**
	 * @brief Initialize memory from an elf file
	 * only sections that match flag signatures are loaded
	 * 
	 * @param ifile filename
	 * @param flags_signatures allowed flag signatures
	 */
	unsigned int initFromElf(std::string ifile, std::vector<int> flags_signatures);
};

#endif // __MEMORY_H__