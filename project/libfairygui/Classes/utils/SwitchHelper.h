#ifndef __SWITCHHELPER_H__
#define __SWITCHHELPER_H__

#include <stdint.h>

typedef std::uint64_t hash_t;

constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

inline hash_t hash_(char const* str)
{
    hash_t ret{ basis };

    while (*str) {
        ret ^= *str;
        ret *= prime;
        str++;
    }

    return ret;
}

constexpr hash_t hash_compile_time(char const* str, hash_t last_value = basis)
{
    return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
}

constexpr hash_t operator "" _hash(char const* p, size_t)
{
    return hash_compile_time(p);
}

#endif
