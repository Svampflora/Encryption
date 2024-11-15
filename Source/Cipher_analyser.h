#pragma once
#include "Text_box.h"

class Cipher_analyser
{


private: 
	float letter_frequency(const std::string_view _message, const char _letter) const noexcept;
};