#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <source_location>

#include "aoc_tools.hpp"

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));
    
    uint64_t answer = 0;

    
    bool current = false;

    auto is_special = [&](int64_t row, int64_t col) {
        if (row < 0 || col < 0) {
            return false;
        }

        if (row >= input_lines.size() || col >= input_lines[row].size()) {
            return false;
        }
        
        return !std::isdigit(input_lines[row][col]) && input_lines[row][col] != '.';
    };

    
    for (uint64_t row = 0; row < input_lines.size(); ++row) {
        uint64_t number = 0;  // Current parsed number
        bool valid = false;   // True if has a special character adjacent to it
        bool last = false;    // True if there's a special character on the previous column
        for (uint64_t col = 0; col < input_lines[row].size(); ++col) {
            bool current = is_special(row, col) || is_special(row - 1, col) || is_special(row + 1, col);
            if (std::isdigit(input_lines[row][col])) {
                valid |= last || current;
                number *= 10;
                number += input_lines[row][col] - '0';
            } else if (number != 0) {
                valid |= current;

                if (valid) {
                    answer += number;
                }
                
                valid = false;
                number = 0;
            }
            last = current;
        }
        if (number != 0 && valid) {
            answer += number;
        }
        
    }
    

    std::cout << std::format("{}", answer);
}