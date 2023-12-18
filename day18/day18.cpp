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
#include <cmath> // std::abs


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

using Vector = std::tuple<Integer,Integer>;

namespace part1 {
  using Grid = std::set<Vector>;

  std::tuple<Vector,Vector> to_bounds(Grid const& grid) {
    auto bounds = std::accumulate(grid.begin(), grid.end(), std::make_tuple(*grid.begin(), *grid.begin()),
        [](auto acc, auto val) {
            auto [min, max] = acc;
            auto [min_row, min_col] = min;
            auto [max_row, max_col] = max;
            auto [row, col] = val;
            return std::make_tuple(
              std::make_tuple(std::min(min_row, row), std::min(min_col, col))
              ,std::make_tuple(std::max(max_row, row), std::max(max_col, col))
            );
        }
    );
    return bounds;
  }

  void print_grid(Grid const& grid) {
    auto bounds = to_bounds(grid);
    auto [upper_left,lower_right] = bounds;
    auto [min_row, min_col] = upper_left;
    auto [max_row, max_col] = lower_right;
    std::cout << NL << NL << "grid:";
    std::cout << " upper_left : " << min_row << "," << min_col;
    std::cout << " lower_right : " << max_row << "," << max_col;
    std::cout << " width: " << max_col - min_col + 1;
    std::cout << " height: " << max_row - min_row + 1;

    std::cout << NL;
    for (int row = min_row; row <= max_row; ++row) {
      std::cout << NL;
      for (int col = min_col; col <= max_col; ++col) {
        Vector pos{row, col};
        if (grid.find(pos) != grid.end())
          std::cout << '#';
        else
          std::cout << '.';
      }
    }
  }

  Grid floodFill(const Grid& grid, Vector start) {
    Grid result{grid};
    auto bounds = to_bounds(grid);
    auto [upper_left,lower_right] = bounds;
    auto [min_row, min_col] = upper_left;
    auto [max_row, max_col] = lower_right;

    std::stack<Vector> stack;
    stack.push(start);

    while (!stack.empty()) {
      Vector current = stack.top();
      stack.pop();
      auto [row, col] = current;

      // If the current position is not in the grid or has already been visited, skip it
      if (row < min_row || row > max_row || col < min_col || col > max_col || result.find(current) != result.end()) {
        continue;
      }
      // Add the current position to the result
      result.insert(current);

      // Add the neighboring positions to the stack
      std::vector<Vector> neighbors = {
        {row - 1, col},
        {row + 1, col},
        {row, col - 1},
        {row, col + 1}
      };
      for (const Vector& neighbor : neighbors) {
        stack.push(neighbor);
      }
    }

    return result;
  }

  Vector to_inside(Grid const& grid) {
    auto bounds = to_bounds(grid);
    auto [upper_left,lower_right] = bounds;
    auto [min_row, min_col] = upper_left;
    auto [max_row, max_col] = lower_right;
    // find a row where we cross the path outside to inside to outside
    for (int row = min_row; row <= max_row; ++row) {
      int count = 0;
      std::vector<Vector> inside{};
      for (int col = min_col; col <= max_col; ++col) {
        Vector pos{row, col};
        if (grid.find(pos) != grid.end()) {
          ++count;
        }
        if (count % 2 == 1 and grid.find(pos) == grid.end()) {
          // odd count and free position == inside :)
          inside.push_back(pos);
        }
      }
      if (count == 2) {
        return inside[0]; // we found a clear inside position (avoid rows with horizontal going path)
      }
    }
    throw std::runtime_error("Failed to find inside position");
  }

  Result solve_for(Model& model) {
    Result result{};
    std::set<Vector> grid{};
    Vector pos{0,0};
    for (auto const& [dir,steps,colour] : model) {
      std::cout << NL << dir << " " << steps << " " << colour;
      using Vector = std::tuple<int,int>;
      Vector delta{0,0};
      switch (dir) {
        case 'R':delta = {0,1};break;
        case 'L':delta = {0,-1}; break;
        case 'U':delta = {-1,0}; break;
        case 'D':delta = {1,0}; break;
      }
      for (int i = 0; i < steps; ++i) {
        std::cout << NT << "pos : " << std::get<0>(pos) << "," << std::get<1>(pos) << " delta : " << std::get<0>(delta) << "," << std::get<1>(delta);
        std::get<0>(pos) += std::get<0>(delta);
        std::get<1>(pos) += std::get<1>(delta);
        std::cout << " --> pos : " << std::get<0>(pos) << "," << std::get<1>(pos);
        grid.insert(pos);
      }
    }
    print_grid(grid);
    auto inside = to_inside(grid);
    std::cout << NL << "inside : " << std::get<0>(inside) << "," << std::get<1>(inside);
    auto result_grid = floodFill(grid, inside);
    print_grid(result_grid);
    result = result_grid.size(); // the flooded area is the result
    return result; // 52055
  }
}

using Path = std::vector<Vector>; // path of straight lines between

// Thanks to,
// https://github.com/thejan14/adventofcode2023/blob/master/day18/part2.cpp
// https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day18p2.py
// For code I used to debug my own solution :)

namespace part2 {
  std::tuple<Vector,Vector> to_bounds(Path const& path) {
    auto bounds = std::accumulate(path.begin(), path.end(), std::make_tuple(*path.begin(), *path.begin()),
        [](auto acc, auto val) {
            auto [min, max] = acc;
            auto [min_row, min_col] = min;
            auto [max_row, max_col] = max;
            auto [row, col] = val;
            return std::make_tuple(
              std::make_tuple(std::min(min_row, row), std::min(min_col, col))
              ,std::make_tuple(std::max(max_row, row), std::max(max_col, col))
            );
        }
    );
    return bounds;
  }
  Model to_transformed(Model const& model) {
    Model result{};
    for (auto const& [_,__,instruction] : model) {
      std::cout << NL << instruction;
      std::cout << NL << instruction;
      auto steps = std::stoi(instruction.substr(2,5),nullptr,16);
      char step{};
      switch (instruction[5+2]-'0') {
        // 0 means R, 1 means D, 2 means L, and 3 means U.
        case 0: step = 'R'; break;
        case 1: step = 'D'; break;
        case 2: step = 'L'; break;
        case 3: step = 'U'; break;
      }
      result.push_back(std::make_tuple(step,steps,""));
    }    
    return result;
  }

  using Path = std::vector<Vector>; // path of straight lines between

  void print_path(Path const& path) {
    auto bounds = to_bounds(path);
    auto [upper_left,lower_right] = bounds;
    auto [min_row, min_col] = upper_left;
    auto [max_row, max_col] = lower_right;
    std::cout << NL << NL << "path:";
    std::cout << " upper_left : " << min_row << "," << min_col;
    std::cout << " lower_right : " << max_row << "," << max_col;
    std::cout << " width: " << max_col - min_col + 1;
    std::cout << " height: " << max_row - min_row + 1;
    for (auto it = path.begin(); it != std::prev(path.end()); ++it) {
      auto [row1,col1] = *it;
      auto [row2,col2] = *std::next(it);
      std::cout << NL << "line : " << row1 << "," << col1 << " --> " << row2 << "," << col2;
    }    
  }

  // Shoelace algorithm (or Gauss's Area formula)
  // This algorithm assumes that the vertices are ordered either clockwise or counterclockwise.
  Integer calculateArea(const Path& points) {
    Integer A = 0;
    Integer n = points.size();

    for (Integer i = 0; i < n; i++) {
      auto [x,y] = points[i];
      auto [x1,y1] = points[(i + 1) % n]; // wraps around to points[0] for i == n-1
      A += (y + y1)*(x-x1); // https://en.wikipedia.org/wiki/Shoelace_formula
    }

    return std::abs(A) / 2;
  }

  // The boundary of a polygon with only horizontal and vertical lines is just the sum of the lengths of the line segments.
  Integer calculateBoundary(const Path& points) {
    Integer B = 0;
    Integer n = points.size();

    for (int i = 0; i < n; i++) {
      auto [x,y] = points[i];
      auto [x1,y1] = points[(i + 1) % n]; // wraps around to points[0] for i == n-1
      Integer dx = std::abs(x - x1);
      Integer dy = std::abs(y - y1);
      B += dx + dy; // Works for us because all our lines are either horizontal or vertical
    }

    return B;
  }

  Result solve_for(Model& model) {
    std::cout << NL << NL << "part2";
    Result result{};
    auto transformed = to_transformed(model);
    // walk the path according the transformed model and generate the path "corners"
    Vector pos{0,0};
    Path path{};
    for (auto const& [dir,steps,colour] : transformed) {
      std::cout << NL << dir << " " << steps << " " << colour;
      using Vector = std::tuple<int,int>;
      Vector delta{0,0};
      switch (dir) {
        case 'R':delta = {0,1};break;
        case 'L':delta = {0,-1}; break;
        case 'U':delta = {-1,0}; break;
        case 'D':delta = {1,0}; break;
      }
      auto new_row = std::get<0>(pos) + std::get<0>(delta) * steps;
      auto new_col = std::get<1>(pos) + std::get<1>(delta) * steps;
      pos = {new_row,new_col};
      path.push_back(pos);
    }
    print_path(path);
    auto inside = calculateArea(path); // Shoelace algorithm (or Gauss's Area formula)
    auto boundary = calculateBoundary(path);
    result = inside + boundary / 2 + 1; // Picks algorithm actually states A + B/2 - 1 (https://en.wikipedia.org/wiki/Pick%27s_theorem)?
    return result; // 67622758357096
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