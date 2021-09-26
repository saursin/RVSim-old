#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>
#include <vector>
#include <map>

namespace Util 
{
    /**
     * @brief String stripping utility functions
     * 
     * @see https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/#:~:text=We%20can%20use%20combination%20of,functions%20to%20trim%20the%20string.
     */
    const std::string WHITESPACE = " \n\r\t\f\v";


    /**
     * @brief removes preceeding whitespaces in a string
     * 
     * @param s string
     * @return std::string 
     */
    std::string lStrip(const std::string& s);

    /**
     * @brief removes succeeding whitespaces in a string
     * 
     * @param s string
     * @return std::string 
     */
    std::string rStrip(const std::string& s);

    /**
     * @brief removes preceding & succeeding whitespaces in a string
     * 
     * @param s string
     * @return std::string 
     */
    std::string strip(const std::string& s);

    // =============================== STRING TOKENIZING =====================================
    /**
     * @brief splits a string accordint to delimiter 
     * 
     * @param txt input string
     * @param strs vector of strings parts
     * @param ch delimiter
     * @return size_t 
     */
    size_t tokenize(const std::string &txt, std::vector<std::string> &strs, char ch);

    // =============================== FILE READER =====================================
    /**
     * @brief reads a binary file
     * 
     * @param memfile filepath
     * @return std::vector<char> contents
     */
    std::vector<char> fReadBin(std::string memfile);

    /**
     * @brief Reads a file and returns its contents
     * 
     * @param filepath Filepath
     * @return Vector of strings containing file contents
     */
    std::vector<std::string> fRead (std::string filepath);

    // =================================== FILE WRITER ====================================
    /**
     * @brief Write to a file
     * 
     * @param filepath Filepath
     */
    void fWrite (std::vector<std::string> data, std::string filepath);

    // ================================ Runtime disassembly =================================
    /**
     * @brief Get the Stdout From shell Command
     * 
     * @param cmd shell command to execute
     * @return std::string command output
     */
    std::string GetStdoutFromCommand(std::string cmd);

    
    struct DisassembledLine
    {
        uint32_t instr;
        std::string disassembly;
    };

    /**
     * @brief Get the Disassembly of input file using riscv objdump
     * 
     * @param filename input filename
     * @return std::map<uint32_t, std::string> map of disassembly
     */
    std::map<uint32_t, DisassembledLine> getDisassembly(std::string filename);
}

#endif //__UTIL_H__