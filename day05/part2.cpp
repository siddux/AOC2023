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

struct Seed {
    int64_t offset;
    int64_t size;

    bool operator<(const Seed &other) const {
        return offset < other.offset;
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

    auto seed_ranges = seeds 
        | std::views::chunk(2)
        | std::views::transform([](auto pair) {
            int64_t offset = pair[0];
            int64_t size = pair[1];
            return Seed{offset, size};
        });
    std::vector<Seed> extended_seeds;
    std::ranges::copy(seed_ranges, std::back_inserter(extended_seeds));
    std::ranges::sort(extended_seeds, std::less<>{});

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

    std::vector<Seed> locations;
    for (const auto& next_map : maps) {
        std::vector<Seed> new_extended_seeds;
        for (auto [offset, size] : extended_seeds) {
            while (size > 0) {
                auto it = std::ranges::upper_bound(next_map, Range{offset, 0, 0}, std::less<>{});
                if (it != next_map.begin()) {
                    it = std::prev(it);
                }

                if (it == next_map.end() || offset < it->input_offset || offset >= it->input_offset + it->size) {
                    new_extended_seeds.push_back({offset, size});
                    break;
                } else {
                    int64_t available = (it->input_offset + it->size) - offset;
                    int64_t actual = std::min(available, size);
                    int64_t transformed_offset = offset - it->input_offset + it->output_offset;
                    new_extended_seeds.push_back({transformed_offset, actual});
                    offset += actual;
                    size -= actual;
                }
            }
        }
        extended_seeds = std::move(new_extended_seeds);
    }
    locations = extended_seeds;

    std::ranges::sort(locations, std::less<>{});
    answer = locations[0].offset;

    std::cout << std::format("{}", answer);
}