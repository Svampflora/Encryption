#include "Decryptor.h"

float Decryptor::letter_frequency(const std::string_view _message, const char _letter) const noexcept
{
    if (_message.empty()) return 0.0f;

    int occurrences = 0;
    for (const char& letter : _message)
    {
        if (letter == _letter)
        {
            occurrences++;
        }
    }

    return static_cast<float>(occurrences) / _message.size();
}
