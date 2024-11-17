#include "Encryptor.h"
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <locale>
#include <codecvt>


static inline std::wstring utf8_to_wstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

static inline std::string wstring_to_utf8(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}


static inline bool is_swedish_alpha(int c)
{
    c = std::tolower(c);
    std::locale swe;
    return  std::isalpha(c, swe);
}

std::string Encryptor::encrypt(std::string message) 
{
    std::string encrypted_message = message;
    for (const auto& cipher : ciphers)
    {
        encrypted_message = cipher(encrypted_message);
    }
    ciphers.clear();
    return encrypted_message;
}

void Encryptor::add_cipher(std::function<std::string(const std::string&)> cipher)
{
    ciphers.push_back(cipher);
}

void Encryptor::draw() const noexcept
{

}

std::string Encryptor::addative_cipher(std::string_view message, int steps, bool decrypt)  noexcept
{
   std::wstring encrypted_message;
   encrypted_message.reserve(message.size());

   if (decrypt)
   {
       steps = -steps;
   }

   for (const char c : message)
   {
       if (c >= 'A' && c <= 'Z')
       {
           encrypted_message += (c - 'A' + steps) % 26 + 'A';
       }

       else if (c >= 'a' && c <= 'z')
       {
           encrypted_message += (c - 'a' + steps) % 26 + 'a';
       }

       else
       {
           encrypted_message += c;
       }
   }

   return wstring_to_utf8(encrypted_message);
}

std::string Encryptor::multiplicative_cipher(std::string_view message, int t, bool decrypt)
{

    if (!is_valid_multiplier(t)) 
    {
        return "Invalid multiplier!";
    }

    if (decrypt) {
        t = modular_inverse(t, 26);
        if (t == -1) {
            return "No modular inverse exists for the given multiplier!";
        }
    }

    std::string encrypted_message;
    encrypted_message.reserve(message.size());

    for (const char c : message) 
    {
        if (is_swedish_alpha(c))
        {
            const bool is_upper = std::isupper(c);
            const int base = is_upper ? 'A' : 'a';
            const int original_pos = c - base;
            const int new_pos = (original_pos * t) % 26;

            encrypted_message += narrow_cast<char>(new_pos + base);
        }
        else 
        {
            encrypted_message += c;
        }
    }

    return encrypted_message;
}

std::string Encryptor::keyword_cipher(std::string_view message, std::string_view key_word, char key_letter, bool decrypt)
{
    
    const std::string alphabet = ALPHABET_SVE;
    const std::string::size_type key_pos = alphabet.find(key_letter);
    const std::string unique_key_word = unique_letters(key_word);

    if (key_pos == std::string::npos) 
    {
        return "Invalid key letter!";
    }

    std::string shifted_alphabet = unique_letters(unique_key_word + alphabet);
    shifted_alphabet = shifted_alphabet.substr(key_pos) + shifted_alphabet.substr(0, key_pos);

    std::string encrypted_message;
    for (const char ch : message) 
    {
        if (is_swedish_alpha(ch))
        {
            const bool upper_case = std::isupper(ch);
            const char lower_char = narrow_cast<char>(std::tolower(ch));

            const std::string& source_alphabet = decrypt ? shifted_alphabet : alphabet;
            const std::string& target_alphabet = decrypt ? alphabet : shifted_alphabet;

            const auto index = source_alphabet.find(lower_char);
            if (index != std::string::npos) {
                const char processed_char = target_alphabet.at(index);
                encrypted_message += upper_case ? static_cast<char>(std::toupper(processed_char)) : processed_char;
            }
        }
        else 
        {
            encrypted_message += ch;
        }
    }

    return encrypted_message;
}

std::string Encryptor::vigenere(std::string_view message, std::string_view keyword, bool decrypt)
{
    std::string encrypted_message;
    const size_t key_length = keyword.size();
    size_t key_index = 0;

    for (const char ch : message) 
    {
        if (is_swedish_alpha(ch))
        {
            const char key_char = narrow_cast<char>(std::tolower(keyword.at(key_index % key_length)));
            int shift = key_char - 'a';

            if (decrypt)
            {
                shift = -shift;
            }

            if (std::islower(ch)) 
            {
                encrypted_message += ((ch - 'a' + shift) % 26) + 'a';
            }
            else 
            {
                encrypted_message += ((ch - 'A' + shift) % 26) + 'A';
            }

            ++key_index;
        }
        else 
        {
            encrypted_message += ch;
        }
    }

    return encrypted_message;
}


std::string Encryptor::rövarspråk(std::string_view message, bool decrypt) noexcept
{

   std::string result;
   if(decrypt)
   {
       for (auto it = message.begin(); it != message.end(); ++it) {
           const char c = *it;
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

   for (const char c : message)
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

std::string Encryptor::unique_letters(const std::string_view& message)
{
    std::string result;
    for (char c : message)
    {
        c = narrow_cast<char>(std::tolower(c));
        if (result.find(c) == std::string::npos)
        {
            result.push_back(c);
        }
    }
    return result;
}

bool Encryptor::is_consonant(char c) noexcept
{
    const char lower = narrow_cast<char>(std::tolower(c)); //TODO: problem?
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
