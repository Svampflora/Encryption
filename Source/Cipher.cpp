#include "Cipher.h"
#include <iostream>
#include <cctype>
#include <unordered_set>
#include <algorithm>
#include <random>
#include "NLFSR.h"
#include "SBox.h"



std::wstring Addative::encrypt(std::wstring_view message, bool decrypt)
{
    const std::wstring_view alphabet = ALPHABET_SWE;

    if (decrypt)
    {
        steps = -steps;
    }

    std::unordered_map<wchar_t, wchar_t> substitution_map;
    for (auto it = alphabet.begin(); it != alphabet.end(); ++it)
    {
        auto wrapped_it = wrap_around_iterator(it, alphabet.begin(), alphabet.end(), steps);
        substitution_map[*it] = *wrapped_it;
    }

    return apply_substitution_map(message, substitution_map);
}

//std::wstring Addative::encrypt(std::wstring_view message, bool decrypt)
//{
//        const std::wstring_view alphabet = ALPHABET_SWE;
//        std::wstring_view shifterd_alphabet = alphabet;
//        
//        if (decrypt)
//        {
//            std::rotate(shifterd_alphabet.begin(), shifterd_alphabet.begin() + steps, shifterd_alphabet.end());
//        }
//        else
//        {
//            std::rotate(shifterd_alphabet.rbegin(), shifterd_alphabet.rbegin() + steps, shifterd_alphabet.rend());
//        }
//    
//        std::unordered_map<wchar_t, wchar_t> substitution_map;
//        for (size_t i = 0; i < alphabet.size(); ++i)
//        {
//            substitution_map[alphabet[i]] = shifterd_alphabet[i];
//        }
//    
//        return apply_substitution_map(message, substitution_map);
//}

std::wstring Multiplicative::encrypt(std::wstring_view message, bool decrypt)
{
    const std::wstring_view alphabet = ALPHABET_SWE;
    const int alphabet_length = narrow_cast<int>(alphabet.length());
    if (!is_valid_multiplier(term))
    {
        return L"Invalid multiplier!";
    }

    if (decrypt)
    {
        term = modular_inverse(term, alphabet_length);
        if (term == -1)
        {
            return L"No modular inverse exists for the given multiplier!";
        }
    }

    std::unordered_map<wchar_t, wchar_t> substitution_map;
    for (auto it = alphabet.begin(); it != alphabet.end(); ++it)
    {
        auto wrapped_it = wrap_around_iterator(it, alphabet.begin(), alphabet.end(), narrow_cast<int>(std::distance(alphabet.begin(), it)) * term);
        substitution_map[*it] = *wrapped_it;
    }

    return apply_substitution_map(message, substitution_map);
}






std::wstring c_pseudo_random(const size_t length, const std::wstring available_chars, unsigned long seed)
{
    if (available_chars.empty())
    {
        throw std::invalid_argument("available_chars must not be empty.");
    }

    std::mt19937 rng(seed);
    std::uniform_int_distribution<size_t> distribution(0, available_chars.size() - 1);

    std::wstring random_string;
    random_string.reserve(length);

    for (size_t i = 0; i < length; ++i)
    {
        const size_t random_index = distribution(rng);
        random_string += available_chars.at(random_index);
    }

    return random_string;
}

std::wstring nlfsr_pseudo_random(const size_t length, const std::wstring& available_chars, const uint32_t seed)
{
    const size_t char_count = available_chars.size();
    if (char_count == 0)
    {
        throw std::invalid_argument("available_chars must not be empty");
    }

    constexpr uint32_t register_size = 16;
    constexpr uint32_t feedback_taps = 0b10100100001;
    NLFSR nlfsr(seed, register_size, feedback_taps);

    std::wstring random_string;
    random_string.reserve(length);
    for (size_t i = 0; i < length; ++i)
    {
        const size_t index = nlfsr.next_value(32) % char_count;
        random_string += available_chars.at(index);
    }

    return random_string;
}

std::wstring to_low_letters(const std::wstring_view& string_view)
{
    std::wstring low_letters;
    for (const wchar_t c : string_view)
    {
        low_letters += narrow_cast<wchar_t>(std::tolower(c));
    }
    return low_letters;
}

std::wstring unique_letters(const std::wstring_view& message)
{
    std::wstring result;
    for (wchar_t c : message)
    {
        c = narrow_cast<wchar_t>(std::tolower(c));
        if (result.find(c) == std::wstring::npos)
        {
            result.push_back(c);
        }
    }
    return result;
}

std::wstring apply_substitution_map(std::wstring_view message, std::unordered_map<wchar_t, wchar_t> substitution_map)
{
    std::wstring encrypted_message;
    encrypted_message.reserve(message.size());
    for (const wchar_t c : message)
    {
        const wchar_t key = narrow_cast<wchar_t>(std::tolower(c));
        if (substitution_map.contains(key))
        {
            encrypted_message += set_case(substitution_map.at(key), std::isupper(c));
        }
        else
        {
            encrypted_message += c;

        }
    }
    return encrypted_message;
}

unsigned long djb2(const wchar_t* str) //TODO: translate to C++
{
    if (!str)
    {
        throw;
    }
    unsigned long hash = 5381;
    int c;

    while (*str)
    {
        c = *str++;
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

constexpr int modular_inverse(int t, int mod) noexcept
{
    int a = t, m = mod;
    int x0 = 0, x1 = 1;

    while (a > 1)
    {
        const int q = a / m;
        int temp = m;

        m = a % m;
        a = temp;

        temp = x0;
        x0 = x1 - q * x0;
        x1 = temp;
    }

    if (x1 < 0)
    {
        x1 += mod;
    }

    return a == 1 ? x1 : -1;
}

wchar_t set_case(wchar_t c, bool upper_case) noexcept
{
    return upper_case ? static_cast<wchar_t>(std::toupper(c)) : c;
}

bool is_consonant(wchar_t c) noexcept
{
    const wchar_t lower = narrow_cast<wchar_t>(std::tolower(c)); //TODO: problematic cast?
    return (lower >= 'a' && lower <= 'z') && !(lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u');
}

bool is_valid_multiplier(int t)
{
    static const std::unordered_set<int> valid_multipliers = { 1, 3, 5, 9, 11, 15, 17, 19, 21, 23, 25 };
    return valid_multipliers.count(t) > 0;
}


