#include "Encryptor.h"
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <random>
#include "NLFSR.h"


wchar_t set_case(wchar_t c, bool upper_case) noexcept
{
    return upper_case ? static_cast<wchar_t>(std::toupper(c)) : c;
}

unsigned long djb2(const wchar_t* str) noexcept //TODO: translate to C++
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

    const uint32_t register_size = 16;
    const uint32_t feedback_taps = 0b10100100001;
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

std::wstring Encryptor::encrypt(std::wstring message) 
{
    return cipher(message, false);
}

std::wstring Encryptor::decrypt(std::wstring message)
{
    return cipher(message, true);
}

void Encryptor::select_cipher(std::function<std::wstring(const std::wstring&, const bool)> _cipher) 
{
    cipher = _cipher;
}

void Encryptor::draw() const noexcept
{

}

std::wstring Encryptor::addative_cipher(std::wstring_view message, int steps, bool decrypt)
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

//std::wstring Encryptor::addative_cipher(std::wstring_view message, int steps, bool decrypt)
//{
//    const std::wstring_view alphabet = ALPHABET_SWE;
//    std::wstring_view shifterd_alphabet = alphabet;
//    
//    if (decrypt)
//    {
//        std::rotate(shifterd_alphabet.begin(), shifterd_alphabet.begin() + steps, shifterd_alphabet.end());
//    }
//    else
//    {
//        std::rotate(shifterd_alphabet.rbegin(), shifterd_alphabet.rbegin() + steps, shifterd_alphabet.rend());
//    }
//
//    std::unordered_map<wchar_t, wchar_t> substitution_map;
//    for (size_t i = 0; i < alphabet.size(); ++i)
//    {
//        substitution_map[alphabet[i]] = shifterd_alphabet[i];
//    }
//
//    return apply_substitution_map(message, substitution_map);
//}

std::wstring Encryptor::multiplicative_cipher(std::wstring_view message, int t, bool decrypt)
{
    const std::wstring_view alphabet = ALPHABET_SWE;
    const int alphabet_length = narrow_cast<int>(alphabet.length());
    if (!is_valid_multiplier(t)) 
    {
        return L"Invalid multiplier!";
    }

    if (decrypt) 
    {
        t = modular_inverse(t, alphabet_length);
        if (t == -1) 
        {
            return L"No modular inverse exists for the given multiplier!";
        }
    }

    std::unordered_map<wchar_t, wchar_t> substitution_map;
    for (auto it = alphabet.begin(); it != alphabet.end(); ++it)
    {
        auto wrapped_it = wrap_around_iterator(it, alphabet.begin(), alphabet.end(), narrow_cast<int>(std::distance(alphabet.begin(), it)) * t); 
        substitution_map[*it] = *wrapped_it;
    }
    
    return apply_substitution_map(message, substitution_map);

}

std::wstring Encryptor::keyword_cipher(std::wstring_view message, const std::wstring_view _keyword, const wchar_t key_letter, const bool decrypt)
{
    const std::wstring keyword = to_low_letters(_keyword);
    const std::wstring alphabet = ALPHABET_SWE.data(); //TODO: inject alphabet to class
    const std::wstring::size_type key_pos = alphabet.find(narrow_cast<wchar_t>(std::tolower(key_letter)));
    const std::wstring unique_key_word = unique_letters(keyword);

    if (key_pos == std::string::npos) 
    {
        return L"Invalid key letter!";
    }

    std::wstring shifted_alphabet = unique_letters(unique_key_word + alphabet);
    shifted_alphabet = shifted_alphabet.substr(key_pos) + shifted_alphabet.substr(0, key_pos);

    const std::wstring_view source_alphabet = decrypt ? shifted_alphabet : alphabet;
    const std::wstring_view target_alphabet = decrypt ? alphabet : shifted_alphabet;
    std::unordered_map<wchar_t, wchar_t> substitution_map;
    for (size_t i = 0; i < source_alphabet.size(); ++i) 
    {
        substitution_map[source_alphabet.at(i)] = target_alphabet.at(i);
    }

    return apply_substitution_map(message, substitution_map);
}

std::wstring Encryptor::vigenere(std::wstring_view message, std::wstring_view _keyword, bool decrypt)
{
    const std::wstring keyword = to_low_letters(_keyword);
    const std::wstring_view alphabet = ALPHABET_SWE;
    const std::wstring::size_type alphabet_length = alphabet.size();
    const std::wstring::size_type key_length = keyword.size();
    std::wstring new_message;
    rsize_t key_index = 0;
    bool debug = false;
    for (const wchar_t ch : message)
    {

        if (ch == L'a')
        {
            debug = true;
        }

        const wchar_t low_ch = narrow_cast<wchar_t>(std::tolower(ch));
        if(alphabet.contains(low_ch))
        {
            const wchar_t key_char = keyword.at(key_index % key_length);
            const size_t shift = alphabet.find(key_char);
            const size_t ch_index = alphabet.find(low_ch);

            const size_t index = (ch_index + (decrypt ? (alphabet_length - shift) : shift)) % alphabet_length;
            const wchar_t letter = alphabet.at(index);

            new_message += set_case(letter, std::isupper(ch));

            ++key_index;
        }
        else 
        {
            new_message += ch;
        }
    }

    return new_message;
}


std::wstring Encryptor::rövarspråk(std::wstring_view message, bool decrypt) noexcept
{

   std::wstring result;
   if(decrypt)
   {
       for (auto it = message.begin(); it != message.end(); ++it) {
           const wchar_t c = *it;
           result += c;

           if (is_consonant(c)) 
           {
               const auto next_it = std::next(it);
               const auto next_next_it = std::next(it, 2);

               if (next_it != message.end() && next_next_it != message.end() &&
                   *next_it == 'o' && *next_next_it == c) 
               {
                   it = next_next_it;
               }
           }
       }
       return result;
   }

   for (const wchar_t c : message)
   {
       if (is_consonant(c))
       {
           result += c;
           result += 'o';
           result += c;
       }
       else
       {
           result += c;
       }
   }
   return result;
}

std::wstring Encryptor::hashed_keyword(std::wstring_view message, std::wstring_view keyword, bool decrypt)
{
    const std::wstring_view alphabet = ALPHABET_SWE;
    const unsigned long hash = djb2(keyword.data());
    std::wstring filter = c_pseudo_random(message.length(), alphabet.data(), hash);
    std::wstring result = vigenere(message, filter, decrypt);

    return result;
}

std::wstring Encryptor::shift_register(std::wstring_view message, std::wstring_view keyword, bool decrypt)
{
    const std::wstring_view alphabet = ALPHABET_SWE;
    const unsigned long hash = djb2(keyword.data());
    std::wstring filter = nlfsr_pseudo_random(message.length(), alphabet.data(), hash);
    std::wstring result = vigenere(message, filter, decrypt);

    return result;
}



std::wstring Encryptor::to_low_letters(const std::wstring_view& string_view)
{
    std::wstring low_letters; 
    for (const wchar_t c : string_view)
    {
        low_letters += narrow_cast<wchar_t>(std::tolower(c));
    }
    return low_letters;
}

std::wstring Encryptor::unique_letters(const std::wstring_view& message)
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

std::wstring Encryptor::apply_substitution_map(std::wstring_view message, std::unordered_map<wchar_t, wchar_t> substitution_map)
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

bool Encryptor::is_consonant(wchar_t c) noexcept
{
    const wchar_t lower = narrow_cast<wchar_t>(std::tolower(c)); //TODO: problematic cast?
    return (lower >= 'a' && lower <= 'z') && !(lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u');
}

bool Encryptor::is_valid_multiplier(int t) 
{
    static const std::unordered_set<int> valid_multipliers = { 1, 3, 5, 9, 11, 15, 17, 19, 21, 23, 25 };
    return valid_multipliers.count(t) > 0;
}

constexpr int Encryptor::modular_inverse(int t, int mod) noexcept
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



