#pragma once
#include "Text_box.h"



class Encryptor
{
	std::vector<std::function<std::string(const std::string&)>> ciphers;


public:
	std::string encrypt(std::string message);
	void add_cipher(std::function<std::string(const std::string&)> cipher);
	void draw() const noexcept;

	static std::string addative_cipher(std::string_view message, int shift, bool decrypt = false) noexcept;
	static std::string multiplicative_cipher(std::string_view message, int t, bool decrypt = false);
	static std::string keyword_cipher(std::string_view message, std::string_view key_word, char key_letter, bool decrypt = false);
	static std::string vigenere(std::string_view message, std::string_view keyword, bool decrypt = false);
    static std::string rövarspråk(std::string_view message, bool decrypt = false) noexcept;

private:
	static std::string unique_letters(const std::string_view& message);
	static bool is_consonant(char c) noexcept;
	static bool is_valid_multiplier(int t);
	static int modular_inverse(int t, int mod) noexcept;

};