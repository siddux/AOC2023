#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <ranges>
#include <source_location>
#include <string_view>

#include "aoc_tools.hpp"

struct Race {
    int64_t time;
    int64_t distance;
};

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));
    
    uint64_t answer = 1;

    auto time_view = input_lines[0] 
        | std::views::drop_while([](char c){ return !std::isdigit(c); })
        | std::views::filter([](char c){ return !std::isspace(c); });
    auto distance_view = input_lines[1] 
        | std::views::drop_while([](char c){ return !std::isdigit(c); })
        | std::views::filter([](char c){ return !std::isspace(c); });

    std::string time_line(time_view.begin(), time_view.end());
    std::string distance_line(distance_view.begin(), distance_view.end());
    std::stringstream time_ss(time_line);
    std::stringstream distance_ss(distance_line);

    std::vector<int64_t> times;
    std::vector<int64_t> distances;
    
    std::copy(std::istream_iterator<int64_t>(time_ss),
              std::istream_iterator<int64_t>(),
              std::back_inserter(times)
    );
    std::copy(std::istream_iterator<int64_t>(distance_ss),
              std::istream_iterator<int64_t>(),
              std::back_inserter(distances)
    );

    std::vector<Race> races;
    for (size_t i = 0; i < times.size(); ++i) {
        races.push_back(Race{times[i], distances[i]});
    }

    std::vector<Race> races_options;
    for (const auto [total_time, goal_distance] : races) {
        for (int64_t load_time = 1; load_time < total_time; ++load_time) {
            int64_t travel_time = total_time - load_time;

            int64_t travel_speed = load_time;
            int64_t travel_distance = travel_time * travel_speed;

            if (travel_distance > goal_distance) {
                races_options.push_back(Race{load_time, travel_distance});
            }
        }
        answer *= races_options.size();
        races_options.clear();
    }

    for (const auto& race : races_options) {
        std::cout << "Race(Load time: " << race.time << ", Travelled distance: " << race.distance << ")\n";
    }

    std::cout << std::format("{}", answer);
}