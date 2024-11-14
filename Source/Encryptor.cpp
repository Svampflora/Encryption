#include "Encryptor.h"
#include <iostream>
#include <unordered_set>
#include <algorithm>

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

std::string Encryptor::addative_cipher(std::string_view message, int steps)  noexcept
{
   std::string encrypted_message;
   encrypted_message.reserve(message.size());

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

   return encrypted_message;
}

std::string Encryptor::multiplicative_cipher(std::string_view message, int t) noexcept 
{

    if (!is_valid_multiplier(t)) 
    {
        return "Invalid multiplier!";
    }

    std::string encrypted_message;
    encrypted_message.reserve(message.size());

    for (const char c : message) 
    {
        if (std::isalpha(c)) 
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

std::string Encryptor::keyword_cipher(std::string_view message, std::string_view key_word, char key_letter) 
{
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    const std::string::size_type key_pos = alphabet.find(key_letter);
    const std::string unique_key_word = unique_letters(key_word);

    std::string shifted_alphabet = unique_letters(unique_key_word + alphabet);
    shifted_alphabet = shifted_alphabet.substr(key_pos) + shifted_alphabet.substr(0, key_pos);

    std::string encrypted_message;
    for (const char ch : message) 
    {
        if (isalpha(ch)) 
        {
            const char lower_char = std::tolower(ch);
            const auto index = alphabet.find(lower_char);
            const char encrypted_char = shifted_alphabet.at(index);
            encrypted_message += isupper(ch) ? std::toupper(encrypted_char) : encrypted_char;
        }
        else 
        {
            encrypted_message += ch;
        }
    }

    return encrypted_message;
}




std::string Encryptor::Rövarspråk(std::string_view message) noexcept
{
   std::string result;
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
        c = std::tolower(c);
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

