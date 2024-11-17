#pragma once
#include <fstream>
#include <string>
#include <stdexcept>


class File_manager 
{
    std::string file_path_;
public:
    explicit File_manager(const std::string_view& file_path) noexcept
        : file_path_(file_path) {}

    std::string read() const 
    {
        std::ifstream file(file_path_, std::ios::in);
        if (!file) {
            throw std::ios_base::failure("Failed to open file for reading: " + file_path_);
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return content;
    }

    void write(const std::string_view& content) const
    {
        std::ofstream file(file_path_, std::ios::out);
        if (!file) 
        {
            throw std::ios_base::failure("Failed to open file for writing: " + file_path_);
        }
        file << content;
        file.close();
    }

    void append(const std::string_view& content) const 
    {
        std::ofstream file(file_path_, std::ios::app);
        if (!file) {
            throw std::ios_base::failure("Failed to open file for appending: " + file_path_);
        }
        file << content;
        file.close();
    }


    std::vector<std::string> read_lines() const 
    {
        std::ifstream file(file_path_, std::ios::in);
        if (!file) {
            throw std::ios_base::failure("Failed to open file for reading: " + file_path_);
        }
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
        return lines;
    }

    bool exists() const 
    {
        std::ifstream file(file_path_);
        return file.good();
    }
};
