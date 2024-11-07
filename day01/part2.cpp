#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <ranges>
#include <source_location>
#include <string_view>
#include <unordered_map>

#include "aoc_tools.hpp"

constexpr std::array<std::pair<std::string_view, uint8_t>, 9> str_numbers{
    std::pair{"one", 1},
    std::pair{"two", 2},
    std::pair{"three", 3},
    std::pair{"four", 4},
    std::pair{"five", 5},
    std::pair{"six", 6},
    std::pair{"seven", 7},
    std::pair{"eight", 8},
    std::pair{"nine", 9}
};

template <typename Range>
std::pair<uint8_t, const char*> find_string_digit(const std::string& line, const Range& indices)
{
    uint8_t found_digit = 0;
    const char* digit_ptr = nullptr;

    std::ranges::find_if(indices, [&line, &found_digit, &digit_ptr](size_t i) {
        return std::ranges::any_of(str_numbers, [&line, i, &found_digit, &digit_ptr](const std::pair<std::string_view, uint8_t> pair) {
            if (std::string_view line_view = line; line_view.substr(i, pair.first.size()) == pair.first) {
                found_digit = pair.second;
                digit_ptr = &line[i];
                return true;
            }
            return false;
        });
    });

    return std::make_pair(found_digit, digit_ptr);
}

std::pair<uint8_t, const char*> find_integer_digit(const std::string& line, const std::ranges::range auto& indices)
{
    uint8_t found_digit = 0;
    const char* digit_ptr = nullptr;

    std::ranges::find_if(indices, [&line, &found_digit, &digit_ptr](size_t i) {
        if (std::isdigit(line[i])) {
            found_digit = line[i] - '0';
            digit_ptr = &line[i];
            return true;
        }
        return false;
    });

    return std::make_pair(found_digit, digit_ptr);
}

template <typename Comparator>
std::pair<uint8_t, const char*> select_digit(
    const std::pair<uint8_t, const char*>& a,
    const std::pair<uint8_t, const char*>& b,
    Comparator comp)
{
    if (a.first == 0 && b.first != 0) return b;
    if (b.first == 0 && a.first != 0) return a;
    return comp(a.second, b.second) ? a : b;
}

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));

    uint64_t answer = 0;
    std::ranges::for_each(input_lines, [&answer](const auto& line)
    {
        auto forward_indices = std::views::iota(std::size_t{0}, line.size());
        auto backward_indices = std::views::iota(std::size_t{0}, line.size()) | std::views::reverse;

        auto first_str_digit = find_string_digit(line, forward_indices);
        auto last_str_digit = find_string_digit(line, backward_indices);

        auto first_int_digit = find_integer_digit(line, forward_indices);
        auto last_int_digit = find_integer_digit(line, backward_indices);

        auto first_digit_pair = select_digit(first_str_digit, first_int_digit, std::less{});
        auto last_digit_pair = select_digit(last_str_digit, last_int_digit, std::greater{});

        answer += first_digit_pair.first * 10 + last_digit_pair.first;
    });

    std::cout << std::format("{}", answer);
}