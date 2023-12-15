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


char const* example = R"(#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#)";

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

using Pattern = std::vector<std::string>;
using Model = std::vector<Pattern>;

Model parse(auto& in) {
  Model result{};
  std::string line{};
  Pattern pattern{};
  while (std::getline(in, line)) {
    if (line.empty()) {
      if (!pattern.empty()) {
        result.push_back(pattern);
        pattern.clear();
      }
    } else {
      pattern.push_back(line);
    }
  }
  if (!pattern.empty()) {
    result.push_back(pattern);
  }
  return result;
}

void print_pattern(Pattern const& pattern) {
  std::cout << NL << "Pattern:";
  for (auto const& line : pattern) {
    std::cout << NL << std::quoted(line);
  }
}

void print_model(Model const& model) {
  for (auto const& pattern : model) {
    print_pattern(pattern);
  }
}

Pattern rotate_right(Pattern const& pattern) {
  Pattern result{};
  for (std::size_t c = 0; c < pattern[0].size(); ++c) {
    std::string line(pattern.size(), ' ');
    for (std::size_t r = 0; r < pattern.size(); ++r) {
      line[line.size() - r - 1] = (pattern[r][c]);
    }
    result.push_back(line);
  }
  return result;
}


namespace part1 {


  std::optional<std::size_t> to_horizontal_mirror(const std::vector<std::string>& grid) {
    for (std::size_t r = 1; r < grid.size(); ++r) {
      auto above = std::vector<std::string>(grid.begin(), grid.begin() + r);
      auto below = std::vector<std::string>(grid.begin() + r, grid.end());

      std::reverse(above.begin(), above.end());

      above.resize(std::min(above.size(), below.size()));
      below.resize(std::min(above.size(), below.size()));

      if (above == below) {
        return r;
      }
    }

    return std::nullopt;
  }

  std::optional<std::size_t> to_vertical_mirror(const std::vector<std::string>& grid) {
    auto row_pattern = rotate_right(grid);
    return to_horizontal_mirror(row_pattern);
  }

  Result solve_for(Model& model) {
    Result result{};
    std::cout << NL << "part1::solve_for(model)";
    print_model(model);
    /*

    With your help, the hot springs team locates an appropriate spring which launches you neatly and precisely up to the edge of Lava Island.

    There's just one problem: you don't see any lava.

    You do see a lot of ash and igneous rock; there are even what look like gray mountains scattered around. 
    After a while, you make your way to a nearby cluster of mountains only to discover that the valley between them 
    is completely full of large mirrors. Most of the mirrors seem to be aligned in a consistent way; 
    perhaps you should head in that direction?

    As you move through the valley of mirrors, you find that several of them have fallen from the large metal frames keeping them in place. 
    The mirrors are extremely flat and shiny, and many of the fallen mirrors have lodged into the ash at strange angles. 
    Because the terrain is all one color, it's hard to tell where it's safe to walk or where you're about to run into a mirror.

    You note down the patterns of ash (.) and rocks (#) that you see as you walk (your puzzle input); 
    perhaps by carefully analyzing these patterns, you can figure out where the mirrors are!

    For example:

    #.##..##.
    ..#.##.#.
    ##......#
    ##......#
    ..#.##.#.
    ..##..##.
    #.#.##.#.

    #...##..#
    #....#..#
    ..##..###
    #####.##.
    #####.##.
    ..##..###
    #....#..#

    To find the reflection in each pattern, you need to find a perfect reflection across either a horizontal line between two rows 
    or across a vertical line between two columns.

    In the first pattern, the reflection is across a vertical line between two columns; arrows on each of the two columns point at the line between the columns:

    123456789
        ><   
    #.##..##.
    ..#.##.#.
    ##......#
    ##......#
    ..#.##.#.
    ..##..##.
    #.#.##.#.
        ><   
    123456789
    */
    /*     
    In this pattern, the line of reflection is the vertical line between columns 5 and 6. Because the vertical line is not perfectly in the middle of the pattern, part of the pattern (column 1) has nowhere to reflect onto and can be ignored; every other column has a reflected column within the pattern and must match exactly: column 2 matches column 9, column 3 matches 8, 4 matches 7, and 5 matches 6.

    The second pattern reflects across a horizontal line instead:

    1 #...##..# 1
    2 #....#..# 2
    3 ..##..### 3
    4v#####.##.v4
    5^#####.##.^5
    6 ..##..### 6
    7 #....#..# 7
    This pattern reflects across the horizontal line between rows 4 and 5. Row 1 would reflect with a hypothetical row 8, but since that's not in the pattern, row 1 doesn't need to match anything. The remaining rows match: row 2 matches row 7, row 3 matches row 6, and row 4 matches row 5.

    To summarize your pattern notes, add up the number of columns to the left of each vertical line of reflection; to that, also add 100 multiplied by the number of rows above each horizontal line of reflection. In the above example, the first pattern's vertical line has 5 columns to its left and the second pattern's horizontal line has 4 rows above it, a total of 405.

    Find the line of reflection in each of the patterns in your notes. What number do you get after summarizing all of your notes?

    */      
    for (auto const& pattern : model) {
      std::cout << NL << "Pattern:";
      if (auto split = to_vertical_mirror(pattern)) {
        std::cout << NL << "Vertical mirror position: " << *split;
        result += *split;
      }
      else if (auto split = to_horizontal_mirror(pattern)) {
        std::cout << NL << "Horizontal mirror position: " << *split;
        result += 100 * *split;
      }
      else {
        std::cout << NL << "No mirror position found";
      }
    }

    return result;
  }
}

namespace part2 {
  std::optional<std::size_t> to_horizontal_mirror(const std::vector<std::string>& grid) {
    std::cout << NL << "part2::to_horizontal_mirror(grid)" << std::flush;
      for (std::size_t r = 1; r < grid.size(); ++r) {
          auto above = std::vector<std::string>(grid.begin(), grid.begin() + r);
          auto below = std::vector<std::string>(grid.begin() + r, grid.end());

          std::reverse(above.begin(), above.end());

          // std::cout << NL << "above:" << std::flush;
          // print_pattern(above);
          // std::cout << NL << "below:" << std::flush;
          // print_pattern(below);

          above.resize(std::min(above.size(), below.size()));
          below.resize(std::min(above.size(), below.size()));

          // lambda to Count number of differences between a and b strings
          auto diff_count = [](const std::string& a, const std::string& b) {
            std::cout << NL << "diff_count(" << std::quoted(a) << "," << std::quoted(b) << ")" << std::flush;
            return std::inner_product(
               a.begin()
              ,a.end()
              ,b.begin()
              ,0
              ,std::plus<>()
              ,[](char x, char y) { return x == y ? 0 : 1; });
          };


          // Find a mirror position where the above and below differs in only one position
          auto diff = std::transform_reduce(
             above.begin()
            ,above.end()
            ,below.begin()
            ,0
            ,std::plus<>() // reduce / accumulate with plus
            ,diff_count); // transform with diff_count

          // Consider only mirror positions that result from single smudge fix (exactly one pattern difference)
          if (diff == 1) {
            // std::cout << NL << "Horizontal mirror position: " << r << std::flush;
            return r;
          }
      }

      return std::nullopt;
  }

  std::optional<std::size_t> to_vertical_mirror(const std::vector<std::string>& grid) {
    auto row_pattern = rotate_right(grid);
    return to_horizontal_mirror(row_pattern);
  }

  Result solve_for(Model& model) {
      Result result{};
      std::cout << NL << "part2::solve_for(model)" << std::flush;
      for (auto const& pattern : model) {
          std::cout << NL << "Pattern:" << std::flush;
          if (auto split = to_vertical_mirror(pattern)) {
              std::cout << NL << "Vertical mirror position: " << *split << std::flush;
              result += *split;
          }
          else if (auto split = to_horizontal_mirror(pattern)) {
              std::cout << NL << "Horizontal mirror position: " << *split << std::flush;
              result += 100 * *split;
          }
          else {
              std::cout << NL << "No mirror position found";
          }
      }
      return result; // 27587
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
    // part1_answer = { "Example",part1::solve_for(model) };
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