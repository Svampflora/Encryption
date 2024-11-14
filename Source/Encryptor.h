#pragma once
#include "Text_box.h"



class Encryptor
{
	std::vector<std::function<std::string(const std::string&)>> ciphers;
	//std::string keyword;
	//char key_letter{'a'};

public:
	std::string encrypt(std::string message);
	void add_cipher(std::function<std::string(const std::string&)> cipher);
	//void set_keyword(std::string_view keyword) noexcept;
	//void set_key_letter(const char keyword) noexcept;
	void draw() const noexcept;

	static std::string addative_cipher(std::string_view message, int shift) noexcept;
	static std::string multiplicative_cipher(std::string_view message, int t);
	static std::string keyword_cipher(std::string_view message, std::string_view key_word, char key_letter);
    static std::string r�varspr�k(std::string_view message) noexcept;

private:
	static std::string unique_letters(const std::string_view& message);
	static bool is_consonant(char c) noexcept;
	static bool is_valid_multiplier(int t);

};