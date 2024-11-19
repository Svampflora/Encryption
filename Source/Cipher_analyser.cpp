#include "Cipher_analyser.h"
#include <algorithm>
#include <numeric>


std::wstring Cipher_analyser::simple_attack(const std::wstring_view _message)
{
    std::vector<float> message_letter_frequency;
    message_letter_frequency.reserve(ALPHABET_SIZE);

    for (const wchar_t c : ALPHABET_SWE)
    {
        message_letter_frequency.emplace_back(letter_frequency(_message, c));
    }

    const auto sorted_language_indices = sort_indices(std::vector<float>(std::begin(LETTER_FREQUENCY_SWE), std::end(LETTER_FREQUENCY_SWE)));
    const auto sorted_message_indices = sort_indices(message_letter_frequency);

    std::unordered_map<wchar_t, wchar_t> substitution_map;
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) 
    {
        const wchar_t cipher_letter = ALPHABET_SWE[sorted_message_indices[i]];
        const wchar_t plain_letter = ALPHABET_SWE[sorted_language_indices[i]];
        substitution_map[cipher_letter] = plain_letter;
    }

    std::wstring deciphered_message;
    deciphered_message.reserve(_message.size());
    for (const wchar_t c : _message) 
    {
        if (substitution_map.count(c)) 
        {
            deciphered_message += substitution_map[c];
        }
        else 
        {
            deciphered_message += c;
        }
    }

    return deciphered_message;
}

std::vector<size_t> Cipher_analyser::sort_indices(const std::vector<float>& frequencies)
{
    std::vector<size_t> indices(frequencies.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(indices.begin(), indices.end(), [&frequencies](size_t a, size_t b) {
        return frequencies[a] > frequencies[b];
        });
    return indices;
}

float Cipher_analyser::letter_frequency(const std::wstring_view _message, const wchar_t _letter) const noexcept
{
    if (_message.empty()) return 0.0f;

    int occurrences = 0;
    for (const wchar_t& letter : _message)
    {
        if (letter == _letter)
        {
            occurrences++;
        }
    }

    return static_cast<float>(occurrences) / _message.size();
}

float Cipher_analyser::index_of_coincidence(const std::wstring_view _message)
{
    return 0.0f;
}


