#include <cctype>
#include <iostream>
#include <iomanip> // E.g., std::quoted
#include <string>
#include <string_view>
#include <sstream> // E.g., std::istringstream, std::ostringstream
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <deque>
#include <array>
#include <ranges> // E.g., std::subrange, std::view 
#include <utility> // E.g., std::pair,..
#include <algorithm> // E.g., std::find, std::all_of,...
#include <numeric> // E.g., std::accumulate
#include <limits> // E.g., std::numeric_limits
#include <fstream>
// #include <experimental/generator> // supported by visual studio 2022 17.8.2 with project setting/compiler switch /std:c++latest
#include <format>
#include <optional>
#include <regex>
#include <compare>


char const* example = R"(467..114..
...*......
..35..633.
......#...
617*......
.....+.58.
..592.....
......755.
...$.*....
.664.598..)";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22 x 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
struct Solution {
  Answers part1{};
  Answers part2{};
};


using Model = std::vector<std::string>;

Model parse(auto& in) {
    Model result{};
    std::string line{};
    while (std::getline(in,line)) {
        result.push_back(line);
    }
    return result;
}

void print_model(const Model& model) {
  std::cout << NL << "Model:";
  for (const auto& line : model) {
    std::cout << NL << line;
  }
}

struct Position {
  Integer row;
  Integer col;
  auto operator<=>(const Position&) const = default;
};

struct PositionedToken {
  std::string label;
  Position pos;
  bool operator<(const PositionedToken& other) const {
    return pos < other.pos;
  }
};

using PositionedTokens = std::vector<PositionedToken>;

PositionedTokens to_positioned_tokens(const std::vector<std::string>& grid, const std::regex& token) {
  PositionedTokens result;

  for (Integer i = 0; i < grid.size(); ++i) {
    auto begin = std::sregex_iterator(grid[i].begin(), grid[i].end(), token);
    auto end = std::sregex_iterator();

    for (std::sregex_iterator it = begin; it != end; ++it) {
      std::smatch match = *it;
      result.push_back({match.str(), {i, static_cast<Integer>(match.position())}});
    }
  }

  return result;
}

void print_positioned_tokens(const PositionedTokens& PositionedTokens) {
  std::cout << NL << "tokens:";
  for (const auto& PositionedToken : PositionedTokens) {
    std::cout << NL << "at (" << PositionedToken.pos.row << "," << PositionedToken.pos.col << ") is: " << std::quoted(PositionedToken.label);
  }
}

std::vector<std::pair<PositionedToken,PositionedToken>> to_adjacent_pairs(PositionedTokens const& first,PositionedTokens const& second) {
  std::vector<std::pair<PositionedToken,PositionedToken>> result{};
  for (auto const& first_token : first) {
    std::cout << NL << "frame of" << std::quoted(first_token.label) << " at (" << first_token.pos.row << "," << first_token.pos.col << ")";
    std::vector<Position> frame{};
    for (int r = first_token.pos.row - 1; r <= first_token.pos.row + 1; ++r) {
      std:: cout << NT << "r: " << r << std::flush;
      for (int c = first_token.pos.col - 1; c <= first_token.pos.col + static_cast<int>(first_token.label.size()); ++c) {
        std::cout << " c: " << c;
        frame.push_back({r,c});
      }
    }
    for (auto const& second_token : second) {
      for (auto const& [r,c] : frame) {
        if (r == second_token.pos.row && c == second_token.pos.col) {
          std::cout << NT << "overlap at (" << r << "," << c << ")";
          result.push_back({first_token,second_token});
        }
      }
    }
  }
  return result;
}

namespace part1 {
  Result solve_for(Model& model) {
      Result result{};
      print_model(model);
      auto candidates = to_positioned_tokens(model,std::regex(R"(\d+)"));
      print_positioned_tokens(candidates);
      auto symbols = to_positioned_tokens(model,std::regex(R"([^\d\.])"));
      print_positioned_tokens(symbols);
      auto adjacent_pairs = to_adjacent_pairs(candidates,symbols);
      std::cout << NL << "adjacent pairs:";
      for (auto const& [first,second] : adjacent_pairs) {
        std::cout << NT << "first: " << std::quoted(first.label) << " at (" << first.pos.row << "," << first.pos.col << ")";
        std::cout << NT << "second: " << std::quoted(second.label) << " at (" << second.pos.row << "," << second.pos.col << ")";
        result += std::stoi(first.label);
      }
      return result; // 537832
  }
}

namespace part2 {
  Result solve_for(Model& model) {
    std::cout << NL << "part2::solve_for(model)";
    Result result{};
    print_model(model);
    auto candidates = to_positioned_tokens(model,std::regex(R"(\d+)"));
    print_positioned_tokens(candidates);
    auto symbols = to_positioned_tokens(model,std::regex(R"([^\d\.])"));
    print_positioned_tokens(symbols);
    auto adjacent_pairs = to_adjacent_pairs(candidates,symbols);
    std::cout << NL << "adjacent pairs:";
    for (auto const& [first,second] : adjacent_pairs) {
      std::cout << NT << "first: " << std::quoted(first.label) << " at (" << first.pos.row << "," << first.pos.col << ")";
      std::cout << NT << "second: " << std::quoted(second.label) << " at (" << second.pos.row << "," << second.pos.col << ")";
    }
    // Ok, so now we are looking for all '*' that have exactly two neighbouring numbers
    std::map<PositionedToken,PositionedTokens> gears{};
    for (auto const& [first,second] : adjacent_pairs) {
      if (second.label == "*") {
        gears[second].push_back(first);
        std::cout << NT << "'*' at (" << second.pos.row << "," << second.pos.col << ") member " << std::quoted(first.label) << " count:" << gears[first].size();
      }
    }
    for (auto const& [gear,neighbours] : gears) {
      if (neighbours.size() == 2) {
        std::cout << NT << "gear " << std::quoted(gear.label) << " at (" << gear.pos.row << "," << gear.pos.col << ") has 2 neighbors";
        result += std::stoi(neighbours[0].label) * std::stoi(neighbours[1].label);
      }
    }

    return result;
  }
}

int main(int argc, char *argv[])
{
  Solution solution{};
  std::cout << NL << "argc : " << argc;
  Answer part1_answer{ "Failed to obtain any input",0 };
  Answer part2_answer{ "Failed to obtain any input",0 };
  if (argc == 1) {
    std::cout << NL << "no data file provided ==> WIll use hard coded example input";
    std::istringstream in{ example };
    auto model = parse(in);
    part1_answer = { "Example",part1::solve_for(model) };
    part2_answer = { "Example",part2::solve_for(model) };
    solution.part1.push_back(part1_answer);
    solution.part2.push_back(part2_answer);
  }
  else for (int i = 0; i < argc; ++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
    if (i > 0) {
      std::ifstream in{ argv[i] };
      if (in) {
        auto model = parse(in);
        part1_answer = { argv[i],part1::solve_for(model) };
        part2_answer = { argv[i],part2::solve_for(model) };
      }
      else {
        part1_answer = { std::string{"Failed to open file "} + argv[i],-1 };
        part2_answer = { std::string{"Failed to open file "} + argv[i],-1 };
      }
      solution.part1.push_back(part1_answer);
      solution.part2.push_back(part2_answer);
    }
  }
  std::cout << NL << NL << "------------ REPORT----------------";
  std::cout << NL << "<Part 1>";
  for (auto const& answer : solution.part1) {
    if (answer.second != 0) std::cout << NT << "answer[" << answer.first << "] " << answer.second;
    else std::cout << NT << "answer[" << answer.first << "] " << " NO OPERATION ";
  }
  std::cout << NL << "<Part 2>";
  for (auto const& answer : solution.part2) {
    if (answer.second != 0) std::cout << NT << "answer[" << answer.first << "] " << answer.second;
    else std::cout << NT << "answer[" << answer.first << "] " << " NO OPERATION ";
  }
  std::cout << "\n\n";
  return 0;
}