#pragma once
#include <iostream>
#include <bitset>

class NLFSR 
{
public:
    NLFSR(uint32_t seed, uint32_t size, uint32_t feedback_taps) noexcept
        : state(seed), register_size(size), taps(feedback_taps) {}

    uint32_t next_bit() noexcept
    {
        uint32_t feedback = 0;
        for (uint32_t i = 0; i < register_size; ++i) 
        {
            if ((taps >> i) & 1) 
            {
                feedback ^= (state >> i) & 1;
            }
        }
        state = ((state << 1) | feedback) & ((1 << register_size) - 1);
        return feedback;
    }

    uint32_t next_value(uint32_t bit_width) noexcept
    {
        uint32_t value = 0;
        for (uint32_t i = 0; i < bit_width; ++i) 
        {
            value = (value << 1) | next_bit();
        }
        return value;
    }

private:
    uint32_t state;
    uint32_t register_size;
    uint32_t taps; 

};
