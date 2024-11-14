#pragma once
#include "Text_box.h"

class Decryptor
{


private: 
	float letter_frequency(const std::string_view _message, const char _letter) const noexcept;
};