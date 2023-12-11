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
    std::cout << NL;
    std::transform(is_empty_column.begin(),is_empty_column.end(),std::ostream_iterator<char>(std::cout, ""),[](auto b){return b?'1':'0';});
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
  void debug() {
    bool debug = true;
      /*
      --- Day 11: Cosmic Expansion ---

      You continue following signs for "Hot Springs" and eventually come across an observatory. The Elf within turns out to be a researcher studying cosmic expansion using the giant telescope here.

      He doesn't know anything about the missing machine parts; he's only visiting for this research project. However, he confirms that the hot springs are the next-closest area likely to have people; he'll even take you straight there once he's done with today's observation analysis.

      Maybe you can help him with the analysis to speed things up?

      The researcher has collected a bunch of data and compiled the data into a single giant image (your puzzle input). The image includes empty space (.) and galaxies (#). For example:
      */
      if (debug) {
  char const* example = R"(...#......
.......#..
#.........
..........
......#...
.#........
.........#
..........
.......#..
#...#.....)";
        std::istringstream in{ example };
        auto model = parse(in);
        print_model(model);
      }     
      /*      
      The researcher is trying to figure out the sum of the lengths of the shortest path between every pair of galaxies. 
      However, there's a catch: the universe expanded in the time it took the light from those galaxies to reach the observatory.

      Due to something involving gravitational effects, only some space expands. 
      In fact, the result is that any rows or columns that contain no galaxies should all actually be twice as big.
      */
     if (debug) {
      /*

        In the above example, three columns and two rows contain no galaxies:

          v  v  v
        ...#......
        .......#..
        #.........
        >..........<
        ......#...
        .#........
        .........#
        >..........<
        .......#..
        #...#.....
          ^  ^  ^
        These rows and columns need to be twice as big; the result of cosmic expansion therefore looks like this:

        ....#........
        .........#...
        #............
        .............
        .............
        ........#....
        .#...........
        ............#
        .............
        .............
        .........#...
        #....#.......
        */

        std::cout << NL << "Expanded universe:";
  char const* example = R"(...#......
.......#..
#.........
..........
......#...
.#........
.........#
..........
.......#..
#...#.....)";
        std::istringstream in{ example };
        auto model = parse(in);
        auto expanded_universe = expand(model);
        print_model(expanded_universe);
  char const* expected = R"(....#........
.........#...
#............
.............
.............
........#....
.#...........
............#
.............
.............
.........#...
#....#.......)";
        auto in_expected = std::istringstream{expected};
        if (expanded_universe == parse(in_expected)) std::cout << NL << "PASS";
        else std::cout << NL << "FAIL";
     }

     if (debug) {
  char const* expanded = R"(....#........
.........#...
#............
.............
.............
........#....
.#...........
............#
.............
.............
.........#...
#....#.......)";
        std::istringstream in{ expanded };
        auto expanded_universe = parse(in);

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
        Galaxies galaxies{};
        for (int row = 0; row < expanded_universe.size(); ++row) {
          for (int column = 0; column < expanded_universe[row].size(); ++column) {
            if (expanded_universe[row][column] == '#') {
              galaxies.push_back({row,column});
            }
          }
        }

      /*
        Equipped with this expanded universe, the shortest path between every pair of galaxies can be found. It can help to assign every galaxy a unique number:

        ....1........
        .........2...
        3............
        .............
        .............
        ........4....
        .5...........
        ............6
        .............
        .............
        .........7...
        8....9.......
        In these 9 galaxies, there are 36 pairs. Only count each pair once; order within the pair doesn't matter. 
        For each pair, find any shortest path between the two galaxies using only steps that move up, down, left, or right exactly one . or # at a time. 
        (The shortest path between two galaxies is allowed to pass through another galaxy.)

        For example, here is one of the shortest paths between galaxies 5 and 9:

        ....1........
        .........2...
        3............
        .............
        .............
        ........4....
        .5...........
        .##.........6
        ..##.........
        ...##........
        ....##...7...
        8....9.......
        
        This path has length 9 because it takes a minimum of nine steps to get from galaxy 5 to galaxy 9
        (the eight locations marked # plus the step onto galaxy 9 itself). Here are some other example shortest path lengths:

        Between galaxy 1 and galaxy 7: 15
        Between galaxy 3 and galaxy 6: 17
        Between galaxy 8 and galaxy 9: 5
        In this example, after expanding the universe, the sum of the shortest path between all 36 pairs of galaxies is 374.
        */
        std::cout << NL << "Shortest paths:";
        using GalaxyPair = std::pair<Position,Position>;
        using Distances = std::map<GalaxyPair,Result>;
        Distances distances{};
        int pair_count = 0;
        for (size_t i = 0; i < galaxies.size(); ++i) {
            for (size_t j = i + 1; j < galaxies.size(); ++j) {
                auto delta_v = galaxies[j] - galaxies[i];
                Result distance = std::abs(delta_v.row) + std::abs(delta_v.column);
                distances[{galaxies[i], galaxies[j]}] = distance;
                std::cout << "\n" << std::format("pair {} : Between galaxy {} and galaxy {}: {}",pair_count++, i+1, j+1, distance);
            }
        }
        auto result = std::accumulate(distances.begin(),distances.end(),Result{0},[](auto sum,auto const& entry){
          return sum + entry.second;});
        std::cout << NL << "Result: " << result;
     }    
  }
  Result solve_for(Model& model) {
    bool debug = true;
    Result result{};
    std::cout << NL << "Part 1";
    std::cout << NL << "------";
    // Expand the universe, then find the length of the shortest path between every pair of galaxies. What is the sum of these lengths?
    auto expanded_universe = expand(model);

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
    Galaxies galaxies{};
    for (int row = 0; row < expanded_universe.size(); ++row) {
      for (int column = 0; column < expanded_universe[row].size(); ++column) {
        if (expanded_universe[row][column] == '#') {
          galaxies.push_back({row,column});
        }
      }
    }

    std::cout << NL << "Shortest paths:";
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
            std::cout << "\n" << std::format("pair {} : Between galaxy {} and galaxy {}: {}",pair_count++, i+1, j+1, distance);
        }
    }
    result = std::accumulate(distances.begin(),distances.end(),Result{0},[](auto sum,auto const& entry){
      return sum + entry.second;});
    return result; // 9370588
  }
}

namespace part2 {

  Result solve_for(Model& grid) {
      Result result{};
      // Thanks to https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day11p2.py and Github copilot chat!
      // I took the chance of learning rather than solving part 2 myself.
      // A way to get to terse code that I want to be able to write.
      // The comments below are mine.

      // Create a vector of the indices of the empty columns and the empty rows.
      std::vector<int> empty_rows{};
      for (int r = 0; r < grid.size(); ++r) {
          if (std::all_of(grid[r].begin(), grid[r].end(), [](char ch) { return ch == '.'; })) {
              empty_rows.push_back(r);
          }
      }

      // Create a vector of the indices of the empty columns.
      std::vector<int> empty_cols{};
      for (int c = 0; c < grid[0].size(); ++c) {
          if (std::all_of(grid.begin(), grid.end(), [c](const std::string& row) { return row[c] == '.'; })) {
              empty_cols.push_back(c);
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

      long long total = 0;
      const int scale = 1000000;

      // Loop over unique pairs i=0..n, j=0..i-1 (i.e., (0,1), (0,2), (1,2), (0,3), (1,3), (2,3), ... i.e., all unique pairs)
      for (int i = 0; i < points.size(); ++i) {
          for (int j = 0; j < i; ++j) {
              // Use min and max to ensure we loop over the rows between galaxy pair from low to high.
              for (int r = std::min(points[i].first, points[j].first); r < std::max(points[i].first, points[j].first); ++r) {
                  total += std::count(empty_rows.begin(), empty_rows.end(), r) == 1 ? scale : 1;
              }
              // Use min and max to ensure we loop over the columns between galaxy pair from low to high.
              for (int c = std::min(points[i].second, points[j].second); c < std::max(points[i].second, points[j].second); ++c) {
                  total += std::count(empty_cols.begin(), empty_cols.end(), c) == 1 ? scale : 1;
              }
          }
      }
      result = total;
      return result; // 746207878188                     
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