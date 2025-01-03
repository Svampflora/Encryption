#pragma once
#include "Text_box.h"

static constexpr float LETTER_FREQUENCY_ENG[]{ 0.82f,0.15f,2.8f,0.43f,0.127f,0.22f,0.20f,0.61f,0.70f,
0.015f,0.077f,0.40f,0.24f,0.67f,0.75f,0.19f,0.0095f,0.60f,0.63f,0.91f,0.28f,0.098f,0.24f,0.015f,0.20f,0.0074f };
static constexpr float LETTER_FREQUENCY_SWE[]{ 0.9383f,0.1535f,0.1486f,0.4702f,0.10149f,0.2027f,0.2862f,
0.2090f,0.5817f,0.0614f,0.3140f,0.5275f,0.3471f,0.8542f,0.4482f,0.1839f,0.0020f,0.8431f,0.6590f,0.7691f,
0.1919f,0.2415f,0.0142f,0.0159f,0.0708f,0.0070f,0.134f,0.180f, 0.131f };


class Cipher_analyser
{

public:

	std::wstring simple_attack(const std::wstring_view _message);
	std::wstring known_cipher_attack(const std::wstring_view _message);

private: 
	std::vector<size_t> sort_indices(const std::vector<float>& frequencies);
	float letter_frequency(const std::wstring_view& _message, const wchar_t _letter) const noexcept;
	float index_of_coincidence(const std::wstring_view _message);

};