#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <iostream>

class SBox 
{
private:
    std::vector<unsigned int> box;

    SBox(const std::vector<unsigned int>& _box) : box(_box) {}

public:
    SBox(const size_t size = 16) //TODO: provide seed?
    {
        box.resize(size);
        std::iota(std::begin(box), std::end(box), 0);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(box.begin(), box.end(), gen);
    }

    SBox inverse() const
    {
        std::vector<unsigned int> inverse(box.size());
        for (unsigned int i = 0; i < box.size(); ++i)
        {
            inverse.at(box.at(i)) = i;
        }
        return SBox(inverse);
    }

    unsigned int substitute(unsigned int input) const
    {
        return box.at(input);
    }

    std::vector<unsigned int> substitute_block(const std::vector<unsigned int>& block) const
    {
        std::vector<unsigned int> output(block.size());
        for (size_t i = 0; i < block.size(); ++i) 
        {
            output.at(i) = substitute(block.at(i));
        }
        return output;
    }
};


