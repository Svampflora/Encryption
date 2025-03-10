#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "utf-8.h"


class File_manager 
{
    std::string file_path_;
public:
    explicit File_manager(const std::string& file_path) noexcept
        : file_path_(file_path) {}


    std::wstring read() {
        std::ifstream file(file_path_, std::ios::binary);
        if (!file) {
            throw std::ios_base::failure("Failed to open file for reading: " + file_path_);
        }

        std::ostringstream oss;
        oss << file.rdbuf();
        const std::string utf8_content = oss.str();

        return utf8_decode(utf8_content);
    }



    void write(const std::wstring& content) {
        std::ofstream file(file_path_, std::ios::binary);
        if (!file) {
            throw std::ios_base::failure("Failed to open file for writing: " + file_path_);
        }

        const std::string utf8_content = utf8_encode(content);
        file << utf8_content;
        file.close();
    }
};
