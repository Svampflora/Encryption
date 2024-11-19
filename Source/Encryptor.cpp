#include "Encryptor.h"
#include <iostream>
#include <unordered_set>
#include <algorithm>


std::wstring Encryptor::encrypt(std::wstring message) 
{
    for (const auto& cipher : ciphers)
    {
        message = cipher(message);
    }
    ciphers.clear();
    return message;
}

void Encryptor::add_cipher(std::function<std::wstring(const std::wstring&)> cipher)
{
    ciphers.push_back(cipher);
}

void Encryptor::draw() const noexcept
{

}

std::wstring Encryptor::addative_cipher(std::wstring_view message, int steps, bool decrypt) 
{
   std::wstring encrypted_message;
   encrypted_message.reserve(message.size());

   if (decrypt)
   {
       steps = -steps;
   }

   for (const wchar_t c : message)
   {
       const bool is_upper = std::isupper(c);
       if (const int index = char_to_index(c) > -1)
       {
           const int new_index = (index + steps) % ALPHABET_SIZE;
           encrypted_message += index_to_char(new_index, is_upper);
       }
       else
       {
           encrypted_message += c;
       }
   }

   return encrypted_message;
}

std::wstring Encryptor::multiplicative_cipher(std::wstring_view message, int t, bool decrypt)
{

    if (!is_valid_multiplier(t)) 
    {
        return L"Invalid multiplier!";
    }

    if (decrypt) {
        t = modular_inverse(t, 26);
        if (t == -1) {
            return L"No modular inverse exists for the given multiplier!";
        }
    }

    std::wstring encrypted_message;
    encrypted_message.reserve(message.size());

    for (const wchar_t c : message)
    {
        if (const int index = char_to_index(c) > -1)
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

std::wstring Encryptor::keyword_cipher(std::wstring_view message, std::wstring_view key_word, char key_letter, bool decrypt)
{
    
    const std::wstring alphabet = ALPHABET_SWE; //TODO: inject alphabet to class
    const std::wstring::size_type key_pos = alphabet.find(key_letter);
    const std::wstring unique_key_word = unique_letters(key_word);

    if (key_pos == std::string::npos) 
    {
        return L"Invalid key letter!";
    }

    std::wstring shifted_alphabet = unique_letters(unique_key_word + alphabet);
    shifted_alphabet = shifted_alphabet.substr(key_pos) + shifted_alphabet.substr(0, key_pos);

    std::wstring encrypted_message;
    for (const wchar_t ch : message)
    {
        if (const int index = char_to_index(ch) > -1)
        {
            const bool upper_case = std::isupper(ch);
            const wchar_t lower_char = narrow_cast<wchar_t>(std::tolower(ch));

            const std::wstring& source_alphabet = decrypt ? shifted_alphabet : alphabet;
            const std::wstring& target_alphabet = decrypt ? alphabet : shifted_alphabet;
            const auto new_index = source_alphabet.find(lower_char);

            if (new_index != std::wstring::npos)
            {
                const wchar_t processed_char = target_alphabet.at(new_index);
                encrypted_message += upper_case ? static_cast<wchar_t>(std::toupper(processed_char)) : processed_char;
            }
        }
        else 
        {
            encrypted_message += ch;
        }
    }

    return encrypted_message;
}

std::wstring Encryptor::vigenere(std::wstring_view message, std::wstring_view keyword, bool decrypt)
{
    std::wstring encrypted_message;
    const size_t key_length = keyword.size();
    size_t key_index = 0;

    for (const wchar_t ch : message)
    {
        if (const int index = char_to_index(ch) > -1)
        {
            const wchar_t key_char = narrow_cast<wchar_t>(std::tolower(keyword.at(key_index % key_length)));
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

int Encryptor::char_to_index(wchar_t c) noexcept
{
    c = narrow_cast<wchar_t>(std::tolower(c));
    unsigned int index = 0;
    for (const auto letter : ALPHABET_SWE)
    {
        if (letter == c)
        {
            return index;
        }
        index++;
    }

    return -1;
}

wchar_t Encryptor::index_to_char( int index, bool uppercase) noexcept
{
    const wchar_t c = ALPHABET_SWE[index % ALPHABET_SIZE];
    return uppercase ? narrow_cast<wchar_t>(std::toupper(c)) : c;
}

bool Encryptor::is_consonant(wchar_t c) noexcept
{
    const wchar_t lower = narrow_cast<wchar_t>(std::tolower(c)); //TODO: problem?
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

