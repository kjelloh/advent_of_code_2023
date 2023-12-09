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
#include <experimental/generator> // supported by visual studio 2022 17.8.2 with project setting/compiler switch /std:c++latest
#include <format>
#include <optional>
#include <regex>
#include <iterator>


char const* example = R"(0 3 6 9 12 15
1 3 6 10 15 21
10 13 16 21 30 45)";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = std::int32_t; // 16 bit int: ± 3.27 · 10^4, 32 bit int: ± 2.14 · 10^9, 64 bit int: ± 9.22 · 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
struct Solution {
  Answers part1{};
  Answers part2{};
};

using Model = std::vector<std::vector<Integer>>;

Model parse(std::istream& input) {
  Model result;

  std::string line;
  while (std::getline(input, line)) {
    std::cout << NL << "line:" << std::quoted(line);
    std::istringstream line_stream(line);
    std::vector<Integer> row;

    Integer value;
    while (line_stream >> value) {
      std::cout << NT << "value:" << value;
      row.push_back(value);
    }

    // Check for parsing failure
    if (line_stream.eof()) {
      result.push_back(std::move(row));
    }
    else {
      // You might want to handle the parsing failure here
      // For simplicity, we'll assume the input is well-formed
      throw std::runtime_error("Failed to parse the input.");
    }
  }

  return result;
}

namespace part1 {
  using TriangleRow = std::vector<Integer>;
  using Triangle = std::vector<TriangleRow>;

  void print_triangle(Triangle const& triangle) {
    // Print Pascal's Triangle
    std::string row_indent{ NT };
    for (int row = 0; row < triangle.size(); ++row) {
      std::cout << row_indent;
      auto values = triangle[row];
      for (int col = 0; col < values.size(); ++col) {
        auto value = values[col];
        if (col >= row) std::cout << value << "....";
      }
      row_indent += "   ";
    }
  }

  Triangle generatePascalsTriangle(TriangleRow const& bottomRow) {
    /* Represent a triangle, e.g.,
    0   3   6   9  12  15
      3   3   3   3   3
        0   0   0   0

    As a matrix:
    row matrix
    0   0   3   6   9  12  15
    1   0   3   3   3   3   3 
    2   0   0   0   0   0   0

    Eqach value on row 1 is the difference of "value above it" - "value to the left of above it value"
    If we call the matrix m, then m[row][col] = m[row-1][col] - m[row-1][col-1]
    In effect, each row contains the triangle row alligned to the right.
        
    */
    Triangle triangle;
    triangle.push_back(bottomRow);

    while (std::any_of(triangle.back().begin(), triangle.back().end(), [](Integer value) { return value > 0; })) {
      const auto& prevRow = triangle.back();
      TriangleRow newRow(prevRow.size());
      // Shift the range we traverse in the previous row one to the right
      auto row_ix = triangle.size() - 1;
      auto rangeBegin = prevRow.begin() + row_ix;
      auto rangeEnd = prevRow.end();
      auto newrowBegin = newRow.begin() + row_ix;
      // Always place next row in the triangle to the left (newRow.begin())
      std::adjacent_difference(rangeBegin, rangeEnd, newrowBegin);
      *newrowBegin = 0; // remove the value copied from previoues row by adjacent_difference *sigh*
      triangle.push_back(newRow);
    }

    return triangle;
  }

  Triangle to_expanded(Triangle const& triangle) {
    Triangle result{ triangle };
    /*
    We are to expand the triangle with a longer bottom row - one more element to the right
    So for,
    0   3   6   9  12  15
      3   3   3   3   3
        0   0   0   0

    We want to get

    0   3   6   9  12  15  ?
      3   3   3   3   3   ?
        0   0   0   0   ?

    We do this with the contraint that the "top row" must still contain onlye zeroes.

    So 1) Extend the last row with a zero

    As a matrix:
    row matrix
    0   0   3   6   9  12  15
    1   0   3   3   3   3   3
    2   0   0   0   0   0   0 0

    2) Add the required value to the row above it (contrained by the value being a pascals-triangle-value)

    As a matrix:
    row matrix
    0   0   3   6   9  12  15   15+3
    1   0   3   3   3   3   3   0+3
    2   0   0   0   0   0   0   0

    */
    result.back().push_back(0);
    for (int row = result.size() - 2; row >= 0; --row) {
      auto value_below = result[row + 1].back();
      auto value_left = result[row].back();
      auto value = value_left + value_below;
      result[row].push_back(value);
      std::cout << NT << std::format("row:{} push value:{}+{}={}",row,value_left,value_below,value);
    }
    std::cout << NT << "new triangle>";
    print_triangle(result);
    return result;
  }

  Result solve_for(Model& model) {
      Result result{};
      using Triangles = std::vector<Triangle>;
      Triangles expanded_triangles{};
      for (auto const& entry : model) {
        std::cout << NL << "<Triangle>";
        auto triangle = generatePascalsTriangle(entry);
        // Print Pascal's Triangle
        std::string row_indent{ NT };
        for (int row = 0; row < triangle.size();++row) {
          std::cout << row_indent;
          auto values = triangle[row];
          for (int col = 0; col < values.size();++col) {
            auto value = values[col];
            if (col >= row) std::cout << value << "....";
          }
          row_indent += "   ";
        }
        auto expanded_triangle = to_expanded(triangle);
        expanded_triangles.push_back(expanded_triangle);
      }
      result = std::accumulate(expanded_triangles.begin(), expanded_triangles.end(), Result{0}, [](auto acc, auto const& triangle) {
        auto extrapolated = triangle[0].back();
        acc += extrapolated;
        std::cout << NT << "extrapolated:" << triangle[0].back() << " sum:" << acc;
        return acc;
        });
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