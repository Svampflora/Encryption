#include "Cipher_analyser.h"
#include <algorithm>
#include <unordered_set>
#include <numeric>




std::wstring Cipher_analyser::simple_attack(const std::wstring_view _message)
{
    const std::wstring alphabet = ALPHABET_SWE.data();
    std::vector<float> message_letter_frequency;
    const size_t alphabet_size = alphabet.length();
    message_letter_frequency.reserve(alphabet_size);

    float total_letter_frequency = 0.0f;
    for (const wchar_t c : alphabet)
    {
        const float frequency = letter_frequency(_message, c);
        message_letter_frequency.emplace_back(frequency);
        total_letter_frequency += frequency;
    }

    const auto sorted_language_indices = sort_indices(std::vector<float>(std::begin(LETTER_FREQUENCY_SWE), std::end(LETTER_FREQUENCY_SWE)));
    const auto sorted_message_indices = sort_indices(message_letter_frequency);

    std::unordered_map<wchar_t, wchar_t> substitution_map;
    for (size_t i = 0; i < alphabet_size; ++i)
    {
        const wchar_t cipher_letter = alphabet.at(sorted_message_indices[i]);
        const wchar_t plain_letter = alphabet.at(sorted_language_indices[i]);
        substitution_map[cipher_letter] = plain_letter;
    }

    std::wstring deciphered_message;
    deciphered_message.reserve(_message.size());
    for (const wchar_t c : _message) 
    {
        deciphered_message += substitution_map[c];
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

float Cipher_analyser::letter_frequency(const std::wstring_view& _message, const wchar_t _letter) const noexcept
{
    if (_message.empty()) return 0.0f;

    int occurrences = 0;
    int characters = 0;
    for (const wchar_t& letter : _message)
    {
        if (letter == _letter)
        {
            occurrences++;
        }
        characters++;
    }

    return static_cast<float>(occurrences) / characters;
}

float Cipher_analyser::index_of_coincidence(const std::wstring_view _message) 
{
    std::wstring alphabet = ALPHABET_SWE.data();
    std::unordered_map<wchar_t, int> frequency;
    int total_letters = 0;

    for (const wchar_t c : _message) 
    {
        if (std::find(alphabet.begin(), alphabet.end(), c) != alphabet.end())
        {
            const wchar_t lower_c = narrow_cast<wchar_t>(std::tolower(c));
            frequency[lower_c]++;
            total_letters++;
        }
    }

    float numerator = 0.0f;
    for (const auto& [letter, count] : frequency) 
    {
        numerator += count * (count - 1);
    }

    const float denominator = static_cast<float>(total_letters * (total_letters - 1));
    return (denominator > 0) ? (numerator / denominator) : 0.0f;
}



