#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <source_location>
#include <string_view>

#include "aoc_tools.hpp"

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));
    
    uint64_t answer = 0;
    std::ranges::for_each(input_lines, [&answer](const auto& line)
    {
        auto first_digit_it = std::ranges::find_if(line, [](char c) { return std::isdigit(c);});
        auto last_digit_it = std::ranges::find_if(line.rbegin(), line.rend(), [](char c) { return std::isdigit(c);});

        if (first_digit_it != line.end() && last_digit_it != line.rend()) {
            std::string str_value;
            str_value += *first_digit_it;
            str_value += *last_digit_it;
            answer += std::stoi(str_value);
        }
    });

    std::cout << std::format("{}", answer);
}