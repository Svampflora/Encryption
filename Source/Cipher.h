#pragma once
#include <string_view>
#include <unordered_map>
#include "Settings.h"
#include "Utilities.h"

class Cipher
{
public:
	virtual std::wstring encrypt(std::wstring_view message, bool decrypt) = 0;
	virtual ~Cipher() = 0;
};

class Addative : public Cipher
{
	int steps;
public:
	Addative(const int _steps) noexcept : steps(_steps) {};
	std::wstring encrypt(std::wstring_view message, bool decrypt) override;
};

class Multiplicative : public Cipher
{
	int term;
public:
	Multiplicative(const int _term) noexcept : term(_term) {};
	std::wstring encrypt(std::wstring_view message, bool decrypt) override;
};

class Keyword : public Cipher
{
	std::wstring keyword{};
	wchar_t key_letter;
public:
	Keyword(std::wstring_view _keyword) noexcept : keyword(_keyword), key_letter(_keyword.front()) {};
	std::wstring encrypt(std::wstring_view message, bool decrypt) override;
	void edit(std::wstring_view _keyword);
};

std::wstring c_pseudo_random(const size_t length, const std::wstring available_chars, unsigned long seed);
std::wstring nlfsr_pseudo_random(const size_t length, const std::wstring& available_chars, const uint32_t seed);
std::wstring to_low_letters(const std::wstring_view& string_view);
std::wstring unique_letters(const std::wstring_view& message);
std::wstring apply_substitution_map(std::wstring_view message, std::unordered_map<wchar_t, wchar_t> substitution_map);
unsigned long djb2(const wchar_t* str); //TODO: translate to C++
constexpr int modular_inverse(int t, int mod) noexcept;
wchar_t set_case(wchar_t c, bool upper_case) noexcept;
bool is_consonant(wchar_t c) noexcept;
bool is_valid_multiplier(int t);




