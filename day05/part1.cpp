#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <ranges>
#include <source_location>
#include <string_view>

#include "aoc_tools.hpp"

struct Range {
    int64_t input_offset;
    int64_t output_offset;
    int64_t size;

    bool operator<(const Range &other) const {
        return input_offset < other.input_offset;
    }
};

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));
    
    uint64_t answer = 0;

    std::vector<Range> map;
    std::vector<std::vector<Range>> maps;

    auto seeds_view = input_lines.front() | std::views::drop_while([](char c){return !std::isdigit(c);});
    std::string seeds_line(seeds_view.begin(), seeds_view.end());
    std::stringstream seeds_ss(seeds_line);
    std::vector<int64_t> seeds;
    std::copy(std::istream_iterator<int64_t>(seeds_ss),
                std::istream_iterator<int64_t>(),
                std::back_inserter(seeds)
    );
    input_lines.erase(input_lines.begin());

    std::ranges::for_each(input_lines, [&map, &maps](const auto& line)
    {
        if (std::isdigit(line[0])) {
            std::stringstream ss(line);
            std::vector<int64_t> numbers;
            std::copy(std::istream_iterator<int64_t>(ss),
                      std::istream_iterator<int64_t>(),
                      std::back_inserter(numbers)
            );
            Range range = {Range{numbers[1], numbers[0], numbers[2]}};
            map.push_back(range);
        } else {
            if (!map.empty()) {
                std::ranges::sort(map, std::less<>{});
                maps.push_back(map);
                map.clear();
            }
        }
    });

    if (!map.empty()) {
        std::ranges::sort(map, std::less<>{});
        maps.push_back(map);
    }

    std::vector<int64_t> locations;
    std::ranges::for_each(seeds, [&maps, &locations](auto seed)
    {
        std::ranges::for_each(maps, [&seed](const auto& next_map)
        {
            auto it = std::ranges::upper_bound(next_map, Range{seed, 0, 0}, std::less<>{});

            if (it == next_map.begin()) {
                return seed;
            }
            it = std::prev(it);

            if (seed - it->input_offset > it->size) {
                return seed;
            }
            seed = (seed - it->input_offset) + it->output_offset;
            return seed;
        });
        locations.push_back(seed);
    });

    answer = *std::ranges::min_element(locations);

    std::cout << std::format("{}", answer);
}