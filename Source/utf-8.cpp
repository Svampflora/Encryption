#include "utf-8.h"
#include "Utilities.h"


#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
std::string utf8_encode(const std::wstring& wide_string) noexcept
{
    std::string utf8_string;
    for (const wchar_t wc : wide_string) 
    {
        uint32_t wide_char = static_cast<uint32_t>(wc); // Promote to larger type

        if (wide_char < 0x80) { // 1-byte (ASCII)
            utf8_string += static_cast<char>(wide_char);
        }
        else if (wide_char < 0x800) { // 2-byte
            utf8_string += static_cast<char>(0xC0 | (wide_char >> 6));
            utf8_string += static_cast<char>(0x80 | (wide_char & 0x3F));
        }
        else if (wide_char < 0x10000) { // 3-byte
            utf8_string += static_cast<char>(0xE0 | (wide_char >> 12));
            utf8_string += static_cast<char>(0x80 | ((wide_char >> 6) & 0x3F));
            utf8_string += static_cast<char>(0x80 | (wide_char & 0x3F));
        }
        else if (wide_char < 0x110000) { // 4-byte
            utf8_string += static_cast<char>(0xF0 | (wide_char >> 18));
            utf8_string += static_cast<char>(0x80 | ((wide_char >> 12) & 0x3F));
            utf8_string += static_cast<char>(0x80 | ((wide_char >> 6) & 0x3F));
            utf8_string += static_cast<char>(0x80 | (wide_char & 0x3F));
        }
    }
    return utf8_string;
}


std::wstring utf8_decode(const std::string& utf8_string) noexcept
{
    std::wstring wide_string;
    for (size_t i = 0; i < utf8_string.size();) 
    {
        wchar_t wc = 0;
        unsigned const char c = utf8_string[i];
        if (c < 0x80) { // 1-byte (ASCII)
            wc = c;
            i += 1;
        }
        else if ((c & 0xE0) == 0xC0) { // 2-byte
            wc = (c & 0x1F) << 6;
            wc |= (utf8_string[i + 1] & 0x3F);
            i += 2;
        }
        else if ((c & 0xF0) == 0xE0) { // 3-byte
            wc = (c & 0x0F) << 12;
            wc |= (utf8_string[i + 1] & 0x3F) << 6;
            wc |= (utf8_string[i + 2] & 0x3F);
            i += 3;
        }
        else if ((c & 0xF8) == 0xF0) { // 4-byte
            wc = (c & 0x07) << 18;
            wc |= (utf8_string[i + 1] & 0x3F) << 12;
            wc |= (utf8_string[i + 2] & 0x3F) << 6;
            wc |= (utf8_string[i + 3] & 0x3F);
            i += 4;
        }
        wide_string += wc;
    }
    return wide_string;
}

#pragma warning(pop)