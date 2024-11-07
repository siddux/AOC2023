#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <ranges>
#include <source_location>
#include <string_view>

#include "aoc_tools.hpp"

struct Info
{
    uint64_t count=1;
    uint64_t matches;
};

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));
    
    uint64_t answer = 0;

    auto line_to_score = [](const auto &line) {
        std::array<std::vector<int64_t>,2> cards;

        for (auto &&[idx, chunk] : line |
            std::views::drop_while([](char c){return c != ':';}) |
            std::views::drop(1) | std::views::split('|') |
            std::views::enumerate) {
            std::string chunk_str(chunk.begin(), chunk.end());
            std::istringstream s{chunk_str};
            std::ranges::copy(std::istream_iterator<int64_t>(s),
                            std::istream_iterator<int64_t>(),
                            std::back_inserter(cards[idx]));
        }

        std::ranges::sort(cards[0]);
        std::ranges::sort(cards[1]);
        std::vector<int64_t> out;
        std::ranges::set_intersection(cards[0], cards[1], std::back_inserter(out));

        return Info{1uz, out.size()};
    };

    std::vector<Info> cards_info;
    std::ranges::for_each(input_lines, [&](const auto& line)
    {
        auto info = line_to_score(line);
        cards_info.push_back(info);
    });

    for (size_t i = 0; i < cards_info.size(); ++i) {
        for (size_t j = i + 1; j <= i + cards_info.at(i).matches; ++j) {
            cards_info.at(j).count += cards_info.at(i).count;
        }
    }
    
    answer = std::ranges::fold_left(cards_info | std::views::transform(&Info::count), 0z, std::plus<>{});

    std::cout << std::format("{}", answer);
}