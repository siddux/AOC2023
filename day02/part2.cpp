#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <source_location>
#include <ranges>
#include <regex>

#include "aoc_tools.hpp"

constexpr uint8_t kMaxRedCubes = 12;
constexpr uint8_t kMaxGreenCubes = 13;
constexpr uint8_t kMaxBlueCubes = 14;

struct Game {
    uint8_t red_cubes = 0;
    uint8_t green_cubes = 0;
    uint8_t blue_cubes = 0;

    bool any_greater_than(const Game& other) const {
        return red_cubes > other.red_cubes ||
               green_cubes > other.green_cubes ||
               blue_cubes > other.blue_cubes;
    }

    uint64_t add_cubes() const {
        return red_cubes*green_cubes*blue_cubes;
    }
};

constexpr Game max_game = {kMaxRedCubes, kMaxGreenCubes, kMaxBlueCubes};

int extract_game_number(const std::string& line) {
    std::regex game_number_regex(R"(Game\s+(\d+):)");
    if (std::smatch match; std::regex_search(line, match, game_number_regex)) {
        return std::stoi(match[1].str());
    }
    return 0;
}

void parse_cube_counts(const std::string& line, std::vector<Game>& minimum_games) {
    auto sets = line 
        | std::views::split(';') 
        | std::views::transform([](const auto& set) {
            std::vector<std::string> words;
            for (auto&& word_range : set | std::views::split(' ')) {
                std::string word(word_range.begin(), word_range.end());
                std::erase_if(word, [](char c) { return c == ',' || c == ';'; });
                if (!word.empty()) {
                    words.push_back(std::move(word));
                }
            }
            return words;
        });

    Game minimum_game;
    for (const auto& words : sets) {
        Game game_set;
        int count = 0;
        for (const auto& word : words) {
            if (std::isdigit(word[0])) {
                count = std::stoi(word);
            } else if (word == "red") {
                game_set.red_cubes += count;
            } else if (word == "green") {
                game_set.green_cubes += count;
            } else if (word == "blue") {
                game_set.blue_cubes += count;
            }
        }

        if (game_set.red_cubes > minimum_game.red_cubes) {
            minimum_game.red_cubes = game_set.red_cubes;
        }
        if (game_set.green_cubes > minimum_game.green_cubes) {
            minimum_game.green_cubes = game_set.green_cubes;
        }
        if (game_set.blue_cubes > minimum_game.blue_cubes) {
            minimum_game.blue_cubes = game_set.blue_cubes;
        }
    }
    minimum_games.push_back(minimum_game);
}

int main()
{   
    const std::source_location location = std::source_location::current();
    std::filesystem::path input_path = std::filesystem::path(location.file_name()).parent_path() / "input.txt";
    auto input_lines = aoc_tools::splitLines(aoc_tools::readInput(input_path));
    
    uint64_t answer = 0;
    std::vector<Game> minimum_games;

    std::ranges::for_each(input_lines, [&minimum_games](const std::string& line)
    {
        int game_number = extract_game_number(line);
        
        parse_cube_counts(line, minimum_games);
    });

    std::ranges::for_each(minimum_games, [&answer](const Game& game)
    {
        answer += game.add_cubes();
    });

    std::cout << std::format("{}", answer);
}