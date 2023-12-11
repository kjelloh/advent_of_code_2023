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


char const* example = R"()";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = long long; // 16 bit int: � 3.27 � 10^4, 32 bit int: � 2.14 � 10^9, 64 bit int: � 9.22 � 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
struct Solution {
  Answers part1{};
  Answers part2{};
};

using Model = std::vector<std::string>;
using Galaxy = std::string;

Model parse(auto& in) {
    Model result{};
    std::string line{};
    while (std::getline(in,line)) {
        result.push_back(line);
    }
    return result;
}

using ExpandedUniverse = Model;
ExpandedUniverse expand(Model& model) {
    ExpandedUniverse result{};
    // It seems "twice as big" means each empty rows gets duplicated and each column gets duplicated.
    // duplicate empty rows
    for (auto const& line : model) {
        if (std::all_of(line.begin(),line.end(),[](auto c){return c == '.';})) {
            result.push_back(line);
            result.push_back(line);
        }
        else {
            result.push_back(line);
        }
    }
    std::vector<bool> is_empty_column{};
    for (int column = 0; column < result[0].size(); ++column) {
        is_empty_column.push_back(std::all_of(result.begin(),result.end(),[column](auto line){return line[column] == '.';}));
    }
    // duplicate empty columns
    for (int column = 0; column < is_empty_column.size(); ++column) { // loop grows with is_empty_column size
        if (is_empty_column[column]) {
            for (auto& line : result) {
                line.insert(column,1,line[column]);
            }
            is_empty_column.insert(is_empty_column.begin()+column,is_empty_column[column]);
            ++column; // skip the newly inserted column to prevent recursive expansion
        }
    }
    return result;
}

void print_model(Model& model) {
    std::cout << NL << "Model:";
    for (auto const& line : model) {
        std::cout << NL << line;
    }
}

namespace part1 {
  struct Position {
    int row{};
    int column{};

    Position operator-(const Position& other) const {
      return {row - other.row, column - other.column};
    }
    // Required to use Position as a key in a std::map
    bool operator<(const Position& other) const {
        return std::tie(row, column) < std::tie(other.row, other.column);
    }
  };

  using Galaxies = std::vector<Position>;

  Result solve_for(Model& model) {
    bool debug = true;
    Result result{};
    std::cout << NL << "Part 1";
    std::cout << NL << "------";
    auto expanded_universe = expand(model);

    Galaxies galaxies{};
    for (int row = 0; row < expanded_universe.size(); ++row) {
      for (int column = 0; column < expanded_universe[row].size(); ++column) {
        if (expanded_universe[row][column] == '#') {
          galaxies.push_back({row,column});
        }
      }
    }

    using GalaxyPair = std::pair<Position,Position>;
    using Distances = std::map<GalaxyPair,Result>;
    Distances distances{};
    int pair_count = 0;
    // find all unique pairs of galaxies
    for (size_t i = 0; i < galaxies.size(); ++i) {
        for (size_t j = i + 1; j < galaxies.size(); ++j) {
            auto delta_v = galaxies[j] - galaxies[i];
            Result distance = std::abs(delta_v.row) + std::abs(delta_v.column);
            distances[{galaxies[i], galaxies[j]}] = distance;
        }
    }
    result = std::accumulate(distances.begin(),distances.end(),Result{0},[](auto sum,auto const& entry){
      return sum + entry.second;});
    return result; // 9370588
  }
}

long long sum_distances(const std::vector<std::pair<int, int>>& points, const std::set<int>& empty_rows, const std::set<int>& empty_cols, int scale) {
  long long total = 0;

  // Loop over unique pairs i=0..n, j=0..i-1 (i.e., (0,1), (0,2), (1,2), (0,3), (1,3), (2,3), ... i.e., all unique pairs)
  for (int i = 0; i < points.size(); ++i) {
    for (int j = 0; j < i; ++j) {
      // Use min and max to ensure we loop over the rows between galaxy pair from low to high.
      auto [r1, r2] = std::minmax(points[i].first, points[j].first);
      for (int r = r1; r < r2; ++r) {
        total += empty_rows.contains(r) ? scale : 1; // Empty row counts as "scale" while all others count as itself
      }

      // Use min and max to ensure we loop over the columns between galaxy pair from low to high.
      auto [c1, c2] = std::minmax(points[i].second, points[j].second);
      for (int c = c1; c < c2; ++c) {
        total += empty_cols.contains(c) ? scale : 1; // Empty column counts as "scale" while all others count as itself
      }
    }
  }

  return total;
}

namespace part2 {

  Result solve_for(Model& grid) {
    std::cout << NL << "Part 2";
    Result result{};
    // Thanks to https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day11p2.py and Github copilot chat!
    // I took the chance of learning rather than solving part 2 myself.
    // A way to get to terse code that I want to be able to write.
    // The comments below are mine.

    // I made the following C++ adaptions.
    // 1. I used a set instead of a list to store the empty rows and columns.
    // 2. I used std::minmax to ensure we loop in the right direction )low to high).

    // Create a vector of the indices of the empty columns and the empty rows.
    std::set<int> empty_rows{};
    for (int r = 0; r < grid.size(); ++r) {
        if (std::all_of(grid[r].begin(), grid[r].end(), [](char ch) { return ch == '.'; })) {
            empty_rows.insert(r);
        }
    }

    // Create a vector of the indices of the empty columns.
    std::set<int> empty_cols{};
    for (int c = 0; c < grid[0].size(); ++c) {
        if (std::all_of(grid.begin(), grid.end(), [c](const std::string& row) { return row[c] == '.'; })) {
            empty_cols.insert(c);
        }
    }

    // Represent the map as the positions of the found galaxies.
    std::vector<std::pair<int, int>> points;
    for (int r = 0; r < grid.size(); ++r) {
        for (int c = 0; c < grid[r].size(); ++c) {
            if (grid[r][c] == '#') {
                points.emplace_back(r, c);
            }
        }
    }
    return sum_distances(points, empty_rows, empty_cols, 1000000);
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