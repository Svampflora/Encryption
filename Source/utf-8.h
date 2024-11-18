#pragma once
#include <string>


std::string utf8_encode(const std::wstring& wide_string);

std::wstring utf8_decode(const std::string& utf8_string);