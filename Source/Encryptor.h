#pragma once
#include "Text_box.h"
#include "File_manager.h"

class Encryptor
{
	//std::wstring alphabet{ ALPHABET_SWE.data()};
	std::function<std::wstring(const std::wstring&, const bool)> cipher;

public:
	std::wstring encrypt(std::wstring message);
	std::wstring decrypt(std::wstring message);
	void select_cipher(std::function<std::wstring(const std::wstring&, const bool)> cipher);
	void draw() const noexcept;

	static std::wstring addative_cipher(std::wstring_view message, int shift, bool decrypt = false);
	static std::wstring multiplicative_cipher(std::wstring_view message, int t, bool decrypt = false);
	static std::wstring keyword_cipher(std::wstring_view message, const std::wstring_view key_word, const wchar_t key_letter, const bool decrypt = false);
	static std::wstring vigenere(std::wstring_view message, std::wstring_view keyword, bool decrypt = false);
    static std::wstring rövarspråk(std::wstring_view message, bool decrypt = false) noexcept;
	static std::wstring hashed_keyword(std::wstring_view message, std::wstring_view keyword, bool decrypt = false);
	static std::wstring shift_register(std::wstring_view message, std::wstring_view keyword, bool decrypt);
	static std::wstring vernam_cipher(std::wstring_view message, std::wstring_view keyword, bool decrypt);


private:
	static std::wstring to_low_letters(const std::wstring_view& string);
	static std::wstring unique_letters(const std::wstring_view& message);
	static std::wstring apply_substitution_map(std::wstring_view message, std::unordered_map<wchar_t, wchar_t> substitution_map);
	static constexpr int modular_inverse(int t, int mod) noexcept;
	static bool is_consonant(wchar_t c) noexcept;
	static bool is_valid_multiplier(int t);


};