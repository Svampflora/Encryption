#pragma once

#include <string_view>
#include <vector>
#include <functional>
#include "Text_box.h"
#include "Utilities.h"


class Encryptor
{
	std::string text;
	std::vector<std::function<std::string(const std::string&)>> ciphers;

public:

	std::string encrypt(std::string message);
	void add_cipher(std::function<std::string(const std::string&)> cipher);
	void draw() const noexcept;

	static std::string Caesar_cipher(std::string_view text, int steps) noexcept;
    static std::string Rövarspråk(std::string_view message) noexcept;

private:
	static bool is_consonant(char c) noexcept;
};