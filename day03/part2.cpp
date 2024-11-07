#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <ranges>
#include <source_location>
#include <unordered_map>
#include <vector>

#include "aoc_tools.hpp"

struct Number {
    int64_t value;
    int64_t col_start;
    int64_t col_end;

    bool operator<(const Number &other) const {
        return col_start < other.col_start;
    }
};

struct NumbersStore {
    std::vector<Number> numbers;

    std::span<Number> get_next_to(int64_t col) {
        auto first = std::ranges::lower_bound(numbers, Number{0, col, 0}, std::less<>{});

        auto last = first;

        if (last != numbers.end()) {
            if (last->col_start == col || last->col_start == col + 1) {
                std::advance(last, 1);
            }
        }

        if (first != numbers.begin() && std::prev(first)->col_end >= col - 1) {
            std::advance(first, -1);
        }

        return std::ranges::subrange(first, last);
    }
};

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));
    
    int64_t answer = 0;

    std::unordered_map<int64_t, NumbersStore> store;
    std::vector<std::pair<int64_t, int64_t>> gears;

    for (int64_t row = 0; row < input_lines.size(); ++row) {
        int64_t number = 0;  // Current parsed number
        int64_t start_col = 0;
        for (int64_t col = 0; col < input_lines[row].size(); ++col) {

            if (input_lines[row][col] == '*') {
                gears.push_back({row,col});
            }

            if (std::isdigit(input_lines[row][col])) {
                if (number == 0) {
                    start_col = col;
                }
                
                number *= 10;
                number += input_lines[row][col] - '0';
            } else if (number != 0) {
                Number number_data{number, start_col, col-1};
                store[row].numbers.push_back(number_data);

                number = 0;
                start_col = 0;
            }
        }
        if (number != 0) {
            Number number_data{number, start_col, input_lines[row].size()-1};
            store[row].numbers.push_back(number_data);
        }
    }

    auto get_ratio = [&](std::pair<int64_t, int64_t> coord) {
        auto numbers = {store[coord.first - 1].get_next_to(coord.second),
                        store[coord.first].get_next_to(coord.second),
                        store[coord.first + 1].get_next_to(coord.second)};

        auto joined = numbers | std::views::join;

        if (std::ranges::count_if(joined, [](auto &){return true;}) != 2) {
            return 0z;
        }

        return std::ranges::fold_left(joined | std::views::transform(&Number::value), 1z, std::multiplies<>{});  
    };

    answer = std::ranges::fold_left(gears | std::views::transform(get_ratio), 0z, std::plus<>{});
    
    std::cout << std::format("{}", answer);
}