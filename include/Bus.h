#ifndef __BUS_H__
#define __BUS_H__

/**
 * @brief Struct that models a system bus
 * 
 * @tparam T data type used for address & data width
 */
template <class T>
struct Bus
{
    T request(T address, T data, int sel, bool write);
};

#endif // __BUS_H__