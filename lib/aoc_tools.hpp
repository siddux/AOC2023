#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace aoc_tools
{
    static std::string readInput(const std::filesystem::path& path) {
        auto input = std::string{};
        if (auto readStream = std::ifstream(path); readStream.is_open())
        {
            input = std::string(
                std::istreambuf_iterator(readStream),
                std::istreambuf_iterator<char>());

            if (input[input.size() - 1] == '\n') {
                throw std::invalid_argument("input contains trailing new line!");
            }
        }
        return input;
    }

    static std::vector<std::string> splitLines(const std::string& input) {
        std::vector<std::string> lines;
        size_t start = 0;
        while (start < input.size()) {
            size_t end = input.find('\n', start);
            if (end == std::string::npos) {
                end = input.size();
            }
            lines.emplace_back(input.data() + start, end - start);
            start = end + 1;
        }
        return lines;
    }
}