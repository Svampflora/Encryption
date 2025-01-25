#include "Cipher_analyser.h"
#include <algorithm>
#include <unordered_set>
#include <numeric>




std::wstring Cipher_analyser::monoalphabetic_attack(const std::wstring_view _message, const Language& language) //TODO: fix this
{
    const std::wstring_view alphabet = language.alphabet;


    std::vector<float> letter_frequencies;
    letter_frequencies.reserve(alphabet.size());
    for (auto ch : alphabet)
    {
        letter_frequencies.emplace_back(letter_frequency(_message, ch));
    }

    std::wstring deciphered_message;


    return deciphered_message;
}

std::vector<size_t> Cipher_analyser::sort_indices(const std::vector<float>& frequencies)
{
    std::vector<size_t> indices(frequencies.size());
    //std::iota(indices.begin(), indices.end(), 0);

    //std::sort(indices.begin(), indices.end(), [&frequencies](size_t a, size_t b) 
    //    {
    //    return frequencies[a] > frequencies[b];
    //    });
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


bool Cipher_analyser::likely_language(const std::wstring_view message, const Language& language) const noexcept
{
    //std::transform(message.begin(), message.end(), message.begin(), ::towlower);

    int words_found = 0;
    for (auto& word : language.common_words)
    {
        if (message.find(word))
        {
            words_found++;
        }
    }

    if (words_found >= 1) 
    {
        return true;
    }

    return false;
}

bool Cipher_analyser::likely_monoalphabetic(const std::wstring_view message)
{
    //std::transform(message.begin(), message.end(), message.begin(), ::towlower);

    if (index_of_coincidence(message) > 0.05f)
    {
        return true;
    }

    return false;
}


