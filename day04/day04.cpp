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
#include <cmath> // std::pow


char const* example = R"(Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11)";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int64_t; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22 x 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
struct Solution {
  Answers part1{};
  Answers part2{};
};

using Numbers = std::vector<int>;
using Card = std::tuple<std::set<int>,Numbers>; 
using Model = std::vector<Card>;

Model parse(auto& in) {
  Model result{};
  std::string line{};
  while (std::getline(in,line)) {
    result.push_back({});
    auto& [winners, numbers] = result.back();
    std::cout << NL << "line : " << std::quoted(line);
    std::string_view sv{ line };
    sv.remove_prefix(sv.find_first_of(' ') + 1);
    sv.remove_prefix(sv.find_first_of(':') + 1);
    while (true) {
        sv.remove_prefix(sv.find_first_not_of(' ')); // skip spaces
        size_t end = sv.find_first_of(' ');
        if (end == std::string_view::npos || sv[0] == '|') {
            break;
        }
        winners.insert(std::stoi(std::string(sv.substr(0, end))));
        sv.remove_prefix(end);
    }    
    sv.remove_prefix(sv.find_first_of('|') + 1);
    while (true) {
        sv.remove_prefix(sv.find_first_not_of(' ')); // skip spaces
        size_t end = sv.find_first_of(' ');
        if (end == std::string_view::npos || sv[0] == '|') {
            break;
        }
        numbers.push_back(std::stoi(std::string(sv.substr(0, end))));
        sv.remove_prefix(end);
    }    
    numbers.push_back(std::stoi(sv.data()));
  }
  return result;
}

void print_model(Model const& model) {
  for (auto const& [winning, numbers] : model) {
    std::cout << NL << "winning : ";
    for (auto const& number : winning) {
      std::cout << number << T;
    }
    std::cout << NL << "numbers : ";
    for (auto const& number : numbers) {
      std::cout << number << T;
    }
  }
}

namespace part1 {
  Result solve_for(Model& model) {
    Result result{};
    std::cout << NL << NL << "part1";
    print_model(model);
    for (auto const& [winning, numbers] : model) {
      std::cout << NT << "winners";
      auto points = std::accumulate(numbers.begin(), numbers.end(), 0, [winning=winning](auto sum, auto const& number) {
        if (winning.contains(number)) {
          sum = (sum==0)?1:sum*2;
          std::cout << " " << number;
        }
        return sum;
      });
      std::cout << " --> " << points;
      result += points;
    }
    // count the numbers in "number" that are in "winning"
    return result;
  }
}

namespace part2 {
  Result solve_for(Model& model) {
    Result result{};
    std::cout << NL << NL << "part2";
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