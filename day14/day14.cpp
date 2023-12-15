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


char const* example = R"(O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....)";

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

using Column = std::string;
using Row = std::string;
using Columns = std::vector<Column>;
using Rows = std::vector<Row>;
using Model = Columns;

Columns transpose(Rows const& rows) {
    Columns columns{};
    for (auto const& row : rows) {
        for (size_t i = 0; i < row.size(); ++i) {
            if (i >= columns.size()) columns.push_back({});
            columns[i].push_back(row[i]);
        }
    }
    return columns;
}

void print_model(Model const& model) {
  std::cout << NL << "Model:";
  auto rows = transpose(model);
  for (auto const& row : rows) {
    std::cout << NL << row;
  }   
}

Model parse(auto& in) {
    Rows rows{};
    std::string line{};
    while (std::getline(in,line)) {
        rows.push_back(line);      
    }
    return transpose(rows);
}


namespace part1 {
  /*
    You reach the place where all of the mirrors were pointing: 
    a massive parabolic reflector dish attached to the side of another large mountain.

    The dish is made up of many small mirrors, but while the mirrors themselves are roughly in the shape of a parabolic reflector dish, 
    each individual mirror seems to be pointing in slightly the wrong direction. 
    If the dish is meant to focus light, all it's doing right now is sending it in a vague direction.

    This system must be what provides the energy for the lava! If you focus the reflector dish, 
    maybe you can go where it's pointing and use the light to fix the lava production.

    Upon closer inspection, the individual mirrors each appear to be connected via an elaborate system of ropes and pulleys 
    to a large metal platform below the dish. 
    The platform is covered in large rocks of various shapes. 
    Depending on their position, the weight of the rocks deforms the platform, 
    and the shape of the platform controls which ropes move and ultimately the focus of the dish.

    In short: if you move the rocks, you can focus the dish. 
    The platform even has a control panel on the side that lets you tilt it in one of four directions! 
    The rounded rocks (O) will roll when the platform is tilted, while the cube-shaped rocks (#) will stay in place. 
    You note the positions of all of the empty spaces (.) and rocks (your puzzle input). For example:

    O....#....
    O.OO#....#
    .....##...
    OO.#O....O
    .O.....O#.
    O.#..O.#.#
    ..O..#O..O
    .......O..
    #....###..
    #OO..#....

    Start by tilting the lever so all of the rocks will slide north as far as they will go:
    */
  Model to_north_tilted(Model& model) {
    Model result{model};
    for (auto& column : result) {
      for (int row = 0;row < column.size();++row) {
        if (column[row] == 'O') {
          // shift the 'O' left until it hits a 'O', a '#' or begin.
          for (int i = row - 1;i >= 0;--i) {
            if (column[i] == '.') {
              column[i] = 'O';
              column[i + 1] = '.';
            }
            else {
              break;            
            }
          }
        }
      }
    }
    return result;
  }
   /*

    OOOO.#.O..
    OO..#....#
    OO..O##..O
    O..#.OO...
    ........#.
    ..#....#.#
    ..O..#.O.O
    ..O.......
    #....###..
    #....#....
    You notice that the support beams along the north side of the platform are damaged; 
    to ensure the platform doesn't collapse, you should calculate the total load on the north support beams.

    The amount of load caused by a single rounded rock (O) is equal to the number of rows from the rock to the south edge of the platform, 
    including the row the rock is on. (Cube-shaped rocks (#) don't contribute to load.) 
    */
    Result load_on_north_support_beams(Model& model) {
      Result result{};
      for (auto const& column : model) {
        for (int row = 0;row < column.size();++row) {
          if (column[row] == 'O') {
            Result rock_load = column.size() - row;
            result += rock_load;
          }
        }
      }
      return result;
    }
   /*
    
    So, the amount of load caused by each rock in each row is as follows:

    OOOO.#.O.. 10
    OO..#....#  9
    OO..O##..O  8
    O..#.OO...  7
    ........#.  6
    ..#....#.#  5
    ..O..#.O.O  4
    ..O.......  3
    #....###..  2
    #....#....  1
    The total load is the sum of the load caused by all of the rounded rocks. In this example, the total load is 136.

    Tilt the platform so that the rounded rocks all roll north. Afterward, what is the total load on the north support beams?  
  */
  Result solve_for(Model& model) {
    print_model(model);
    Result result{};
    auto north_tilted = to_north_tilted(model);
    print_model(north_tilted);
    result = load_on_north_support_beams(north_tilted);
    return result; // 105461
  }
}

namespace part2 {
  struct Position {
    int column{};
    int row{};
    bool operator<(Position const& other) const {
      return (column < other.column) || (column == other.column && row < other.row );
    }
  };
  struct Map {
    std::map<Position, char> grid{};
    Position upper_left{};
    Position lower_right{};
  };

  std::optional<char> at(Map const& map, Position const& position) {
    auto it = map.grid.find(position);
    if (it != map.grid.end()) {
      return it->second;
    }
    return {};
  }

  void print_map(Map const& map) {
    std::cout << NL << "Map:";
    for (int row = map.upper_left.row; row <= map.lower_right.row; ++row) {
      std::cout << NL;
      for (int column = map.upper_left.column; column <= map.lower_right.column; ++column) {
        if (auto rock = at(map, { column, row })) {
          std::cout << *rock;
        }
        else {
          std::cout << '.';
        }
      }
    }
  }

  Map to_map(Model& model) {
    Map result{};
    for (int column = 0; column < model.size(); ++column) {
      for (int row = 0; row < model[column].size(); ++row) {
        if (model[column][row] != '.') {
          result.grid[{column, row}] = model[column][row]; // store only rocks

          // Update upper_left position
          result.upper_left.column = std::min(result.upper_left.column, column);
          result.upper_left.row = std::min(result.upper_left.row, row);

          // Update lower_right position
          result.lower_right.column = std::max(result.lower_right.column, column);
          result.lower_right.row = std::max(result.lower_right.row, row);
        }
      }
    }
    return result;
  }
  Map rotate_right(Map const& map) {
    Map result{};
    for (auto const& [position, rock] : map.grid) {
      Position new_position{ -position.row, position.column };
      result.grid[new_position] = rock;
    }
    result.upper_left = { -map.lower_right.row, map.upper_left.column };
    result.lower_right = { -map.upper_left.row, map.lower_right.column };
    return result;
  }

  Map to_north_tilted(Map const& map) {
    Map result{};
    result.upper_left = map.upper_left;
    result.lower_right = map.lower_right;

    // transform map into result where all 'O' has rolled "north"
    for (int column = map.upper_left.column; column <= map.lower_right.column; ++column) {
      // std::cout << NL << "column : " << column;
      int row = map.upper_left.row;
      while (row <= map.lower_right.row) {
        Position current_position{ column, row };
        if (auto rock = at(map, current_position)) {
          if (*rock == '#') result.grid[current_position] = *rock; // move to unchanged position
          else {
            // roll 'O' north as long as there is nothing in the way
            int rr = row;
            while (rr > map.upper_left.row && !at(result, { column, rr - 1 })) {
              --rr;
              // std::cout << " --" << rr;
            }
            result.grid[{column,rr}] = *rock; // can't roll any further
          }
        }
        ++row;
        // std::cout << " ++" << row;
      }
    }

    return result;
  }
  Result load_on_north_support_beams(Map const& map) {
    Result result{};
    for (auto const& [position, rock] : map.grid) {
      if (rock == 'O') {
        Result rock_load = map.lower_right.row - position.row + 1;
        result += rock_load;
      }
    }
    return result;
  }
  Result solve_for(Model& model) {
    std::cout << NL << "part2::solve_for";
    Result result{};
    auto map = to_map(model);
    print_map(map);
    for (Result i = 0;i<1000000000;++i) {
      if (i % 1000 == 0) std::cout << NL << "i : " << i;
      // north is north
      map = to_north_tilted(map); // tilt to north
      // std::cout << NL << "north is north";
      // print_map(map);
      map = rotate_right(map); // west is north
      map = to_north_tilted(map); 
      // std::cout << NL << "west is north";
      // print_map(map);
      map = rotate_right(map); // south is north
      map = to_north_tilted(map);
      // std::cout << NL << "south is north";
      // print_map(map);
      map = rotate_right(map); // east is north
      map = to_north_tilted(map);
      // std::cout << NL << "east is north";
      // print_map(map);
    }
    map = rotate_right(map); // north is north
    print_map(map);
    result = load_on_north_support_beams(map);
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