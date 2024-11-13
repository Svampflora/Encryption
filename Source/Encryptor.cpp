#include "Encryptor.h"
#include <iostream>

std::string Encryptor::encrypt(std::string message) 
{
    //return Caesar_cipher(message, 2);

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

std::string Encryptor::Caesar_cipher(std::string_view message, int steps)  noexcept
{
   std::string result;

   for (const char c : message)
   {
       if (c >= 'A' && c <= 'Z')
       {
           result += (c - 'A' + steps) % 26 + 'A';
       }

       else if (c >= 'a' && c <= 'z')
       {
           result += (c - 'a' + steps) % 26 + 'a';
       }

       else
       {
           result += c;
       }
   }

   return result;
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

bool Encryptor::is_consonant(char c) noexcept
{
    const char lower = static_cast<char>(std::tolower(c)); //TODO: problem
    return (lower >= 'a' && lower <= 'z') && !(lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u');
}

