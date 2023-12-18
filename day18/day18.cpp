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


char const* example = R"(R 6 (#70c710)
D 5 (#0dc571)
L 2 (#5713f0)
D 2 (#d2c081)
R 2 (#59c680)
D 2 (#411b91)
L 5 (#8ceee2)
U 2 (#caa173)
L 1 (#1b58a2)
U 2 (#caa171)
R 2 (#7807d2)
U 3 (#a77fa3)
L 2 (#015232)
U 2 (#7a21e3))";

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

using Model = std::vector<std::tuple<char,int,std::string>>;

Model parse(auto& in) {
  Model result{};
  std::string line{};
  while (std::getline(in,line)) {
    std::string_view sv(line);
    std::vector<std::string> parts;
    while (!sv.empty()) {
        auto space = sv.find(' ');
        auto token = sv.substr(0, space);
        parts.push_back(std::string(token));
        sv.remove_prefix(space == sv.npos ? sv.size() : space + 1);
    }
    result.push_back(std::make_tuple(parts[0][0],std::stoi(parts[1]),parts[2]));
  }
  return result;
}

using Vector = std::tuple<int,int>;
using Grid = std::set<Vector>;

void print_grid(Grid const& grid) {
  std::cout << NL;
  auto upper_left = std::min_element(grid.begin(),grid.end(),[](auto const& lhs, auto const& rhs) {
    return std::get<0>(lhs) < std::get<0>(rhs) || (std::get<0>(lhs) == std::get<0>(rhs) && std::get<1>(lhs) < std::get<1>(rhs));
  });
  auto lower_right = std::max_element(grid.begin(),grid.end(),[](auto const& lhs, auto const& rhs) {
    return std::get<0>(lhs) < std::get<0>(rhs) || (std::get<0>(lhs) == std::get<0>(rhs) && std::get<1>(lhs) < std::get<1>(rhs));
  });
  for (int row = std::get<0>(*upper_left); row <= std::get<0>(*lower_right); ++row) {
    for (int col = std::get<1>(*upper_left); col <= std::get<1>(*lower_right); ++col) {
      Vector pos{row,col};
      if (grid.find(pos) != grid.end()) std::cout << '#';
      else std::cout << '.';
    }
    std::cout << NL;
  }
}

namespace part1 {
  Result solve_for(Model& model) {
    Result result{};
    std::set<Vector> grid{};
    Vector pos{0,0};
    for (auto const& [dir,steps,colour] : model) {
      std::cout << NL << dir << T << steps << T << colour;
      using Vector = std::tuple<int,int>;
      Vector delta{0,0};
      switch (dir) {
        case 'R':delta = {0,1};break;
        case 'L':delta = {0,-1}; break;
        case 'U':delta = {-1,0}; break;
        case 'D':delta = {1,0}; break;
      }
      for (int i = 0; i < steps; ++i) {
        std::cout << NL << "pos : " << std::get<0>(pos) << T << std::get<1>(pos) << " delta : " << std::get<0>(delta) << T << std::get<1>(delta);
        std::get<0>(pos) += std::get<0>(delta);
        std::get<1>(pos) += std::get<1>(delta);
        std::cout << " --> pos : " << std::get<0>(pos) << T << std::get<1>(pos);
        grid.insert(pos);
      }
    }
    print_grid(grid);
    return result;
  }
}

namespace part2 {
  Result solve_for(Model& model) {
    Result result{};
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