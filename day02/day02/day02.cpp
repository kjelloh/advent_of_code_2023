// day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <experimental/generator>
#include <iostream>
#include <ranges>
#include <sstream>
#include <iomanip>
#include <optional>
#include <algorithm>
#include <cctype>
#include <utility>
#include <map>
#include <vector>
#include <numeric>

auto example = R"(Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green)";

extern char const* part1;

std::experimental::generator<char> letters(char first)
{
  for (;; co_yield first++);
}

using IndexedToken = std::pair<int, std::string>;

std::experimental::generator<IndexedToken> lines(auto& in)
{
    std::string line{};
    int index{};
    while (std::getline(in,line)) {
        co_yield{ index,line };
        ++index;
    }
}

std::experimental::generator<IndexedToken> tokens(auto& token,char delimiter) {
    std::string split{};
    int index{};
    for (char ch : token) {
        if (ch == delimiter) {
            co_yield {index,split};
            ++index;
            split.clear();
        }
        else {
            split += ch;
        }
    }
    co_yield{ index,split };
}

std::optional<int> to_number(auto const& s) {
    std::optional<int> result{};
    if (!s.empty() and std::ranges::all_of(s, [](char ch) {return std::isdigit(ch);})) {
        result = std::stoi(s);
    }
    return result;
}


int main() {
    std::map<std::string, int> const BAGGED_CUBES{
         {"red",12}
        ,{"green",13}
        ,{"blue",14}
    };
    for (const char ch : letters('a') | std::views::take(26)) std::cout << ch << ' ';
    // std::istringstream in{ example };
    std::istringstream in{ part1 };
    std::string indent{ "\n" };
    std::vector<std::pair<int, std::vector<std::vector<std::pair<int, std::string>>>>> games{};
    for (auto const& token : lines(in)) {
        std::cout << indent << token.first << " : " << std::quoted(token.second);
        indent += '\t';
        for (auto const& token : tokens(token.second, ':')) {
            std::cout << indent << token.first << " : " << std::quoted(token.second);
            indent += '\t';
            switch (token.first) {
            case 0: {
                int game_no{};
                indent += '\t';
                for (auto const& token : tokens(token.second, ' ')) {
                    std::cout << indent << token.first << " : " << std::quoted(token.second);
                    switch (token.first) {
                    case 0: {/*skip*/} break;
                    case 1: {
                        if (auto number = to_number(token.second)) {
                            game_no = *number;
                            std::cout << indent << "==> Identified Game " << game_no;
                            games.push_back({});
                            games.back().first = game_no;
                        }
                        else {
                            std::cerr << indent << "Failed to identify game number from " << std::quoted(token.second);
                            exit(-1);
                        }
                    } break;
                    default: {
                        std::cerr << indent << " extraneous game metadata " << std::quoted(token.second);
                        exit(-1);
                    } break;
                    }
                }
                indent.erase(indent.end() - 1);
            } break;
            case 1: {
                for (auto const& token : tokens(token.second, ';')) {
                    games.back().second.push_back({}); // new outcome
                    std::cout << indent << token.first << " : " << std::quoted(token.second);
                    indent += '\t';
                    for (auto const& token : tokens(token.second, ',')) {
                        games.back().second.back().push_back({}); // new cube
                        std::cout << indent << token.first << " : " << std::quoted(token.second);
                        indent += '\t';
                        int count{};
                        for (auto const& token : tokens(token.second, ' ')) {
                            std::cout << indent << token.first << " : " << std::quoted(token.second);
                            if (auto number = to_number(token.second)) {
                                count = *number;
                            }
                            else if (!token.second.empty()) {
                                std::cout << indent << "==> " << count << " " << token.second << " cube(s)";
                                games.back().second.back().back().first = count;
                                games.back().second.back().back().second = token.second;
                            }
                        }
                        indent.erase(indent.end() - 1);
                    }
                    indent.erase(indent.end() - 1);
                }
            } break;
            default: std::cerr << "\nFailed to parse line " << token.first << " i.e., " << std::quoted(token.second); exit(-1);  break;
            }
            indent.erase(indent.end() - 1);
        }
        indent.erase(indent.end() - 1);
    }
    auto result_part1 = std::accumulate(games.begin(), games.end(), int{ 0 }, [&BAGGED_CUBES](auto acc, auto const& game) {
        std::cout << "\nFilter Game " << game.first;
        if (std::ranges::all_of(game.second, [&BAGGED_CUBES](auto const& outcomes) {
            std::cout << "\n\tFilter grab outcome";
            bool result = std::ranges::all_of(outcomes, [&BAGGED_CUBES](auto const& outcome) {
                std::cout << "\n\t\tComparing cube outcome " << outcome.first << " " << outcome.second << " with bag limit " << BAGGED_CUBES.at(outcome.second);
                bool result = (outcome.first <= BAGGED_CUBES.at(outcome.second));
                if (result) std::cout << " ok"; else std::cout << " UNACCEPATBLE";
                return result;
                // return false;
                });
            return result;
            })) {
            std::cout << "\nACCEPTED: Game " << game.first;
            acc += game.first;
            std::cout << " SUM:" << acc;
        }
        return acc;
        });
    auto result_part2 = std::accumulate(games.begin(), games.end(), int{ 0 }, [&BAGGED_CUBES](auto acc, auto const& game) {
        std::cout << "\nFilter Game " << game.first;
        std::map<std::string,int> min_bag{};
        if (std::ranges::all_of(game.second, [BAGGED_CUBES ,&min_bag](auto const& outcomes) {
            std::cout << "\n\tFilter grab outcome";
            bool result = std::ranges::all_of(outcomes, [BAGGED_CUBES ,&min_bag](auto const& outcome) {
                min_bag[outcome.second] = std::max(outcome.first, min_bag[outcome.second]);
                std::cout << "\n\t\trequired bag also for cube outcome " << outcome.first << " " << outcome.second << "  is " << min_bag[outcome.second];
                return true; // accept all outcomes
                });
            return result;
            })) {
            std::cout << "\nGame " << game.first;
            auto power = std::accumulate(min_bag.begin(), min_bag.end(), int{ 1 }, [](auto acc,auto const& entry) {
                acc *= entry.second;
                return acc;
                });
            std::cout << " has power " << power;
            acc += power;
            std::cout << " SUM:" << acc;
        }
        return acc;
        });
    std::cout << "\nAnswer part 1 = " << result_part1;
    std::cout << "\nAnswer part 2 = " << result_part2;
}

char const* part1 = R"(Game 1: 12 red, 2 green, 5 blue; 9 red, 6 green, 4 blue; 10 red, 2 green, 5 blue; 8 blue, 9 red
Game 2: 3 green, 7 red; 3 blue, 5 red; 2 green, 1 blue, 6 red; 3 green, 2 red, 3 blue
Game 3: 12 red, 18 blue, 3 green; 14 red, 4 blue, 2 green; 4 green, 15 red
Game 4: 14 blue, 8 red, 10 green; 7 green, 9 blue, 4 red; 4 green, 5 red
Game 5: 2 red, 1 blue, 4 green; 1 blue, 1 red, 5 green; 6 green, 3 red; 10 blue, 4 green, 1 red
Game 6: 5 red, 5 blue, 1 green; 5 blue, 15 red, 2 green; 3 green, 1 red, 9 blue
Game 7: 9 red, 8 blue, 13 green; 9 red, 7 blue, 10 green; 7 blue, 2 red, 11 green; 4 red, 6 blue, 10 green; 3 blue, 12 green, 1 red
Game 8: 20 red, 5 green, 10 blue; 14 red, 8 blue, 5 green; 5 green, 4 blue, 9 red; 18 red, 1 green; 2 blue, 1 green, 5 red
Game 9: 3 blue; 3 blue, 4 green, 1 red; 8 green, 2 blue, 4 red; 5 green, 4 red
Game 10: 18 red, 11 green, 3 blue; 2 blue, 19 red, 7 green; 4 green, 1 blue, 6 red; 4 green, 2 red, 4 blue; 10 green, 5 red, 2 blue; 13 red, 12 green, 4 blue
Game 11: 5 green, 5 blue, 3 red; 2 red, 8 blue, 4 green; 4 blue, 4 green, 2 red; 1 blue, 3 red, 2 green; 4 blue, 4 green; 6 blue, 2 red
Game 12: 6 blue, 1 green, 4 red; 12 blue, 4 red; 12 blue, 2 red, 6 green
Game 13: 11 red, 17 blue, 7 green; 20 red, 12 green, 9 blue; 15 red, 15 green, 14 blue; 7 red, 6 green, 3 blue
Game 14: 8 red, 17 green, 6 blue; 5 red, 13 blue, 7 green; 2 red, 15 green, 4 blue; 9 blue, 2 green; 7 green, 2 blue, 8 red; 10 green, 14 blue, 10 red
Game 15: 14 red, 4 green, 4 blue; 14 red, 2 green, 16 blue; 10 blue, 2 green, 6 red
Game 16: 2 red, 3 green; 5 green; 1 red, 1 blue; 2 red, 3 green, 1 blue; 5 red, 4 green; 5 red, 4 green
Game 17: 4 blue, 3 green, 9 red; 3 blue, 8 red, 1 green; 1 blue, 5 green; 8 green, 2 red; 10 red, 1 green
Game 18: 10 blue, 5 red; 1 green, 9 red, 9 blue; 5 blue, 3 red; 3 red, 1 blue; 2 blue, 9 red, 1 green; 6 red, 7 blue, 1 green
Game 19: 9 green, 2 red, 12 blue; 3 green, 9 red, 16 blue; 2 green, 17 blue; 11 green, 3 blue, 7 red; 2 red, 10 blue, 11 green
Game 20: 17 blue, 16 green; 13 green, 12 blue, 13 red; 6 red, 9 green, 6 blue
Game 21: 1 green, 3 red, 1 blue; 1 blue, 2 green, 2 red; 2 green, 1 blue, 3 red
Game 22: 7 green, 3 blue, 5 red; 2 green, 14 red, 3 blue; 2 green, 17 red; 2 blue, 15 red, 6 green; 4 green, 1 blue, 17 red
Game 23: 4 blue, 2 red; 2 red, 7 blue, 1 green; 6 red, 5 blue, 5 green; 9 red, 4 blue; 10 blue, 8 red, 11 green
Game 24: 1 red, 9 green, 5 blue; 14 green, 4 blue, 2 red; 5 blue, 1 red, 11 green; 3 blue, 2 red, 4 green
Game 25: 3 green, 2 red, 1 blue; 2 blue, 10 green, 1 red; 8 red, 4 green, 1 blue; 3 blue, 15 red, 6 green; 2 blue, 13 red, 8 green; 2 red, 5 blue, 5 green
Game 26: 5 green, 5 red; 12 green; 5 green, 3 blue, 4 red; 1 blue, 1 red, 17 green; 2 blue, 18 green
Game 27: 1 blue, 7 green, 3 red; 3 red, 1 green, 13 blue; 4 green, 8 blue; 1 red, 3 green, 4 blue; 9 blue, 2 red
Game 28: 9 red, 2 green, 5 blue; 5 red, 9 green; 5 blue, 1 red
Game 29: 4 green, 10 blue, 13 red; 2 red, 5 green, 5 blue; 2 red, 9 green, 11 blue; 9 blue, 9 red, 13 green; 13 blue, 2 green, 14 red; 3 green, 8 blue, 5 red
Game 30: 3 green, 7 red, 3 blue; 5 green, 5 blue, 12 red; 14 red, 6 green, 11 blue; 7 blue, 9 green, 11 red; 10 red, 1 blue, 4 green
Game 31: 6 green, 5 blue, 17 red; 16 blue, 17 green, 14 red; 13 green, 13 blue, 3 red; 18 red, 8 green, 14 blue; 18 green, 1 blue; 16 red, 6 blue
Game 32: 12 green, 2 blue; 6 blue, 5 red, 10 green; 13 green, 2 blue; 2 red, 6 blue, 6 green; 9 green, 8 red
Game 33: 4 blue, 6 red, 2 green; 7 red, 5 blue; 3 blue, 1 red, 1 green
Game 34: 2 red, 7 blue, 1 green; 2 blue, 1 green, 6 red; 6 red, 3 green, 7 blue; 4 green, 13 red, 1 blue; 15 blue, 1 green, 5 red
Game 35: 8 blue, 12 red, 7 green; 2 blue, 14 red, 3 green; 14 green, 8 blue, 7 red; 15 red, 12 blue, 12 green; 11 blue, 14 red, 1 green
Game 36: 11 red, 8 green, 2 blue; 17 red, 2 blue; 3 blue, 8 green, 19 red; 6 red, 3 blue, 3 green; 8 red, 5 green
Game 37: 2 blue, 3 red, 12 green; 3 red, 7 green, 4 blue; 7 blue, 3 red, 8 green; 13 green, 3 red, 2 blue; 2 green
Game 38: 2 blue, 14 red, 17 green; 1 blue, 13 green, 6 red; 14 green, 2 blue, 2 red; 4 blue, 1 green, 6 red; 2 red, 12 green, 2 blue; 4 red, 2 blue, 1 green
Game 39: 2 green, 3 red, 12 blue; 1 green, 14 blue, 16 red; 13 red, 9 blue, 1 green; 15 red, 1 green, 18 blue
Game 40: 17 green; 12 green, 6 blue; 1 red, 3 blue, 6 green; 1 red, 5 blue, 8 green
Game 41: 2 green, 8 red; 2 blue, 7 green, 14 red; 13 red, 2 green, 1 blue
Game 42: 1 red, 7 blue, 7 green; 2 green, 4 blue, 1 red; 15 green, 8 blue, 1 red; 1 red, 1 blue, 12 green; 6 blue, 10 green; 6 blue, 1 red, 15 green
Game 43: 8 blue, 1 green, 1 red; 3 green, 1 red; 2 red, 6 blue, 9 green; 2 blue, 3 green
Game 44: 8 green; 1 blue, 12 green, 16 red; 9 red, 9 green, 2 blue; 3 blue, 11 green, 4 red; 16 red, 8 blue, 11 green
Game 45: 4 blue, 3 red, 13 green; 2 red, 2 blue, 13 green; 11 green, 7 blue, 2 red; 9 green, 6 blue; 12 green
Game 46: 14 green; 9 blue, 11 green, 10 red; 19 green, 8 red, 14 blue; 12 red, 10 blue
Game 47: 1 green, 14 red; 2 blue, 11 green, 4 red; 6 red, 8 blue, 2 green
Game 48: 8 blue, 13 red; 5 red, 2 green, 10 blue; 9 red, 12 blue, 1 green
Game 49: 2 blue, 8 green; 1 red, 8 green, 4 blue; 6 red, 1 blue; 3 green, 2 red, 4 blue; 6 blue, 1 red, 9 green
Game 50: 7 red, 14 green, 4 blue; 9 red, 10 green, 2 blue; 3 red, 9 blue, 14 green; 2 green, 12 red, 5 blue; 10 blue, 6 green; 8 red, 1 blue, 7 green
Game 51: 4 red, 18 green, 1 blue; 15 green; 3 blue, 16 red, 17 green; 9 blue, 10 red, 13 green; 11 green, 14 red; 1 green, 7 blue, 1 red
Game 52: 7 red, 4 blue, 5 green; 1 green, 10 blue, 1 red; 4 blue, 5 red, 7 green
Game 53: 3 red, 1 blue; 1 green, 3 red, 2 blue; 2 red, 1 blue, 8 green
Game 54: 1 green, 11 red; 1 blue, 3 green, 1 red; 1 blue, 1 red
Game 55: 1 blue, 2 green; 5 blue, 3 red; 3 green, 8 red, 3 blue; 4 red, 4 blue, 3 green; 2 green, 4 red; 4 red, 2 blue
Game 56: 17 blue, 12 green; 1 red, 7 green, 16 blue; 3 blue, 4 green, 3 red; 7 blue, 12 red, 15 green; 4 red, 8 green
Game 57: 2 red, 3 blue; 1 red, 8 blue; 9 red, 11 green, 8 blue; 1 blue, 13 green, 6 red; 7 red, 8 green, 2 blue
Game 58: 13 blue, 8 red, 4 green; 2 green, 7 red; 6 green, 1 blue; 5 red, 8 blue, 5 green; 5 blue, 1 green, 3 red
Game 59: 8 red, 10 blue, 3 green; 9 red, 19 blue; 11 red, 2 green, 20 blue; 1 red, 8 blue, 3 green; 11 blue, 1 red, 2 green; 8 red, 3 green, 19 blue
Game 60: 1 green, 2 blue, 5 red; 6 red, 2 green, 2 blue; 12 green, 2 blue; 5 red, 5 green, 1 blue
Game 61: 4 red, 9 green, 1 blue; 15 green, 1 blue, 8 red; 2 blue, 20 green; 13 green, 1 blue, 3 red; 7 green, 7 red
Game 62: 7 red, 5 blue; 6 green, 6 blue, 7 red; 5 red, 4 blue, 2 green; 3 red, 3 green; 9 blue, 1 green, 1 red
Game 63: 6 blue; 1 blue, 4 red; 11 blue, 2 green, 3 red; 5 blue, 2 green, 7 red; 3 red, 11 blue
Game 64: 3 green, 3 blue, 5 red; 2 red, 6 blue, 3 green; 2 red, 6 blue; 3 green, 7 blue
Game 65: 6 red, 7 blue, 11 green; 15 green, 9 blue, 3 red; 8 green, 10 red, 1 blue; 16 blue, 6 red, 2 green; 3 green, 10 red, 14 blue; 10 red, 2 blue, 13 green
Game 66: 5 blue, 9 green; 2 green, 2 red, 7 blue; 4 red, 12 green, 1 blue; 2 red, 13 green, 7 blue; 3 red, 4 green, 2 blue
Game 67: 8 green, 5 red; 3 blue, 4 red, 10 green; 5 red, 12 blue, 11 green; 11 green, 4 blue; 5 blue, 4 green, 2 red; 1 red, 4 blue, 10 green
Game 68: 4 blue, 13 red, 1 green; 2 blue, 6 red, 1 green; 5 green, 13 red, 2 blue; 3 blue, 3 green, 5 red
Game 69: 6 red, 14 blue; 16 red, 17 blue; 4 red, 2 green; 14 red, 6 blue, 1 green; 16 red, 15 blue
Game 70: 5 blue, 6 red, 6 green; 6 green, 1 blue, 6 red; 3 blue, 12 red, 4 green
Game 71: 1 green, 13 blue, 1 red; 2 green, 2 red; 1 green, 1 red, 6 blue
Game 72: 5 green, 10 red; 13 blue, 7 red, 8 green; 12 red, 3 green, 2 blue
Game 73: 10 green, 5 red; 11 red, 13 blue, 11 green; 14 green, 5 blue, 1 red; 9 green, 13 red, 10 blue; 8 red, 11 green, 8 blue
Game 74: 10 blue, 1 green, 6 red; 7 blue, 8 green, 4 red; 1 red, 8 blue, 7 green; 7 green, 1 red, 10 blue; 6 red, 9 green, 4 blue
Game 75: 1 blue, 3 green; 15 blue, 2 green, 11 red; 9 red, 18 blue; 10 red, 17 blue, 2 green
Game 76: 3 green, 6 red, 4 blue; 7 green, 3 red; 5 blue, 15 red; 5 green, 2 blue, 20 red
Game 77: 5 blue, 3 green, 7 red; 6 blue, 3 green, 1 red; 13 red, 5 blue, 1 green; 2 red, 2 blue, 3 green; 4 green, 7 blue
Game 78: 6 red, 4 blue; 2 blue, 2 red; 8 blue, 1 green, 8 red; 4 red, 1 green, 10 blue; 2 green, 5 red, 13 blue; 7 red, 2 green, 5 blue
Game 79: 3 green, 11 red; 16 green, 9 red, 17 blue; 3 red; 17 green, 18 blue, 5 red; 3 green, 3 red; 6 red, 18 blue, 12 green
Game 80: 2 red, 5 blue, 1 green; 7 red; 2 red, 3 blue; 8 red, 5 blue; 8 red, 4 blue, 1 green
Game 81: 12 green, 6 blue; 9 blue, 5 green, 1 red; 1 red, 7 green, 1 blue; 7 green, 1 blue; 1 green, 4 blue, 1 red
Game 82: 12 green, 3 blue; 1 blue, 8 green; 15 green, 3 blue, 1 red
Game 83: 5 red, 8 green; 10 green, 3 red, 11 blue; 12 green, 6 blue, 3 red; 2 green, 13 red, 9 blue; 12 green, 5 blue
Game 84: 1 green, 8 red, 2 blue; 2 blue, 9 red; 1 blue, 6 red; 3 red, 1 green
Game 85: 2 blue, 7 red; 5 blue, 5 red, 6 green; 2 blue, 3 green, 7 red; 2 green, 1 blue, 5 red; 3 green, 2 blue; 5 red, 6 green, 1 blue
Game 86: 13 red, 9 green, 4 blue; 15 blue, 11 green; 12 red, 19 blue, 14 green
Game 87: 13 green, 5 red, 7 blue; 5 red, 17 green, 5 blue; 16 green, 4 blue, 5 red; 15 green, 6 blue, 5 red; 2 red, 7 green, 3 blue; 3 green, 2 blue
Game 88: 9 green, 1 red, 5 blue; 8 red, 7 green, 14 blue; 8 red, 9 blue, 10 green; 4 red, 10 blue; 10 red, 4 green, 19 blue; 9 red, 1 green
Game 89: 4 blue, 7 green, 1 red; 6 blue, 3 green; 7 blue, 1 red, 6 green; 4 blue, 4 green; 1 red, 11 green; 1 red, 1 green, 3 blue
Game 90: 4 red, 12 green, 4 blue; 1 red, 9 green; 5 green, 10 blue, 6 red; 6 red, 5 blue, 2 green
Game 91: 16 blue, 11 red; 2 green, 3 red, 12 blue; 1 green, 17 blue, 1 red
Game 92: 2 green, 2 blue, 1 red; 7 blue, 2 green; 8 blue, 1 red, 9 green; 11 blue, 5 green, 1 red
Game 93: 5 blue, 1 red, 5 green; 2 red; 5 red, 9 green, 13 blue
Game 94: 2 red, 3 blue; 2 blue, 8 red, 5 green; 1 red, 15 blue; 2 red, 8 blue, 5 green; 16 blue, 11 red; 6 green, 6 blue, 6 red
Game 95: 4 green, 5 red, 10 blue; 8 green, 1 red, 13 blue; 6 green, 9 blue, 7 red; 2 red, 2 blue, 2 green; 5 red, 9 green, 9 blue; 2 blue, 10 green, 3 red
Game 96: 2 green; 7 blue, 10 red, 4 green; 9 red, 2 blue, 9 green
Game 97: 5 blue, 6 red, 14 green; 6 green, 14 blue, 2 red; 2 blue, 4 green
Game 98: 8 green, 1 blue, 9 red; 1 blue, 10 green; 11 green, 1 blue, 3 red; 1 blue, 11 green, 10 red; 14 green, 7 red; 4 red, 10 green
Game 99: 1 red, 6 green, 3 blue; 7 blue, 1 red, 2 green; 1 red, 4 green; 6 green, 11 blue, 1 red; 4 blue, 2 green; 2 blue, 1 red, 6 green
Game 100: 5 green, 1 red; 1 red, 6 green; 6 blue, 1 red, 6 green; 6 blue, 1 green, 2 red; 8 blue, 1 red, 4 green; 8 green, 5 blue)";
