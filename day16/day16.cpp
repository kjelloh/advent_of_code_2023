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

char const* example = R"(.|...\....
|.-.\.....
.....|-...
........|.
..........
.........\
..../.\\..
.-.-/..|..
.|....-|.\
..//.|....)";

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

using Grid = std::vector<std::string>;
using Model = Grid;

Model parse(auto& in) {
  Model result{};
  std::string line{};
  while (std::getline(in,line)) {
      result.push_back(line);
  }
  return result;
}

void print_model(Model const& model) {
  std::cout << NL << NL << "Model:";
  for (auto const& line : model) {
      std::cout << NL << line;
  }
}

using Beam = std::tuple<int, int, int, int>;

// Breath first search according to navigation rules of beams
std::set<std::pair<int, int>> find_energized_tiles(Model const& grid,Beam const& beam = {0, -1, 0, 1}) {

  std::deque<std::tuple<int, int, int, int>> q{beam};
  std::set<std::tuple<int, int, int, int>> seen{};

  while (!q.empty()) {
    auto [r, c, dr, dc] = q.front();
    q.pop_front();

    r += dr;
    c += dc;

    if (r < 0 || r >= grid.size() || c < 0 || c >= grid[0].size()) {
      continue;
    }

    char ch = grid[r][c];
    std::cout << NL << "ch=" << ch << " at " << r << "," << c << " with dr=" << dr << " and dc=" << dc;

    if (ch == '.' || (ch == '-' && dc != 0) || (ch == '|' && dr != 0)) {
      std::cout << NT << "Treat as empty at " << r << "," << c << " with dr=" << dr << " and dc=" << dc;
      if (seen.insert({r, c, dr, dc}).second) {
        q.push_back({r, c, dr, dc});
        std::cout << NT << "Pushing " << r << "," << c << " with dr=" << dr << " and dc=" << dc;
      }
    } else if (ch == '/') {
      std::cout << NT << "'/' at " << r << "," << c << " with dr=" << dr << " and dc=" << dc;
      if (dr == 0 && dc == 1) {
          dr = -1;
          dc = 0;
      } else if (dr == 0 && dc == -1) {
          dr = 1;
          dc = 0;
      } else if (dr == -1 && dc == 0) {
          dr = 0;
          dc = 1;
      } else if (dr == 1 && dc == 0) {
          dr = 0;
          dc = -1;
      }
      if (seen.insert({r, c, dr, dc}).second) {
        q.push_back({r, c, dr, dc});
        std::cout << NT << "Pushing " << r << "," << c << " with dr=" << dr << " and dc=" << dc;
      }
    } else if (ch == '\\') {
      std::cout << NT << "'\\' at " << r << "," << c << " with dr=" << dr << " and dc=" << dc;
      if (dr == 0 && dc == 1) {
          // RIGHT to DOWN
          dr = 1;
          dc = 0;
      } else if (dr == 0 && dc == -1) {
          // LEFT to UP
          dr = -1;
          dc = 0;
      } else if (dr == -1 && dc == 0) {
          // UP to LEFT
          dr = 0;
          dc = -1;
      } else if (dr == 1 && dc == 0) {
          // DOWN to RIGHT
          dr = 0;
          dc = 1;
      }
      if (seen.insert({r, c, dr, dc}).second) {
        q.push_back({r, c, dr, dc});
        std::cout << NT << "Pushing " << r << "," << c << " with dr=" << dr << " and dc=" << dc;
      }
    } else {
      // '|' or '-'
      std::cout << NT << "'|' or '-' at " << r << "," << c << " with dr=" << dr << " and dc=" << dc;
      for (auto [dr, dc] : (ch == '|' ? std::vector<std::pair<int, int>>{{1, 0}, {-1, 0}} : std::vector<std::pair<int, int>>{{0, 1}, {0, -1}})) {
        if (seen.insert({r, c, dr, dc}).second) {
          q.push_back({r, c, dr, dc});
          std::cout << NT << "Pushing " << r << "," << c << " with dr=" << dr << " and dc=" << dc;
        }
      }
    }
  }

  std::set<std::pair<int, int>> coords;
  for (const auto& [r, c, _, __] : seen) {
    coords.insert({r, c});
  }

  return coords;
}


namespace part1 {

  using Positions = std::set<std::pair<int, int>>;
  void print_energized(Positions const& positions) {
    auto [upper_left_r, upper_left_c] = *std::min_element(positions.begin(), positions.end());
    auto [lower_right_r, lower_right_c] = *std::max_element(positions.begin(), positions.end());
    for (int r = upper_left_r; r <= lower_right_r; ++r) {
      std::cout << NL;
      for (int c = upper_left_c; c <= lower_right_c; ++c) {
        if (positions.find({ r, c }) != positions.end()) {
          std::cout << '#';
        } else {
          std::cout << '.';
        }
      }
    }
  }

  Result solve_for(Model& model) {
    Result result{};
    /*

    With the beam of light completely focused somewhere, the reindeer leads you deeper still into the Lava Production Facility. 
    At some point, you realize that the steel facility walls have been replaced with cave, 
    and the doorways are just cave, and the floor is cave, and you're pretty sure this is actually just a giant cave.

    Finally, as you approach what must be the heart of the mountain, you see a bright light in a cavern up ahead. 
    There, you discover that the beam of light you so carefully focused is emerging from the cavern wall closest to the facility 
    and pouring all of its energy into a contraption on the opposite side.

    Upon closer inspection, the contraption appears to be a flat, two-dimensional square grid 
    containing empty space (.), mirrors (/ and \), and splitters (| and -).

    The contraption is aligned so that most of the beam bounces around the grid, 
    but each tile on the grid converts some of the beam's light into heat to melt the rock in the cavern.

    You note the layout of the contraption (your puzzle input). For example:

    .|...\....
    |.-.\.....
    .....|-...
    ........|.
    ..........
    .........\
    ..../.\\..
    .-.-/..|..
    .|....-|.\
    ..//.|....

    The beam enters in the top-left corner from the left and heading to the right. 
    Then, its behavior depends on what it encounters as it moves:

    If the beam encounters empty space (.), it continues in the same direction.
    If the beam encounters a mirror (/ or \), the beam is reflected 90 degrees depending on the angle of the mirror. 
      For instance, a rightward-moving beam that encounters a / mirror would continue upward in the mirror's column, 
      while a rightward-moving beam that encounters a \ mirror would continue downward from the mirror's column.
    If the beam encounters the pointy end of a splitter (| or -), the beam passes through the splitter 
      as if the splitter were empty space. For instance, a rightward-moving beam that encounters a - splitter 
      would continue in the same direction.
    If the beam encounters the flat side of a splitter (| or -), the beam is split into two beams 
      going in each of the two directions the splitter's pointy ends are pointing. 
      For instance, a rightward-moving beam that encounters a | splitter would split into two beams: 
      one that continues upward from the splitter's column and one that continues downward from the splitter's column.
    Beams do not interact with other beams; a tile can have many beams passing through it at the same time. 
    
    A tile is energized if that tile has at least one beam pass through it, reflect in it, or split in it.

    In the above example, here is how the beam of light bounces around the contraption:

    >|<<<\....
    |v-.\^....
    .v...|->>>
    .v...v^.|.
    .v...v^...
    .v...v^..\
    .v../2\\..
    <->-/vv|..
    .|<<<2-|.\
    .v//.|.v..
    
    Beams are only shown on empty tiles; arrows indicate the direction of the beams. 
    If a tile contains beams moving in multiple directions, the number of distinct directions is shown instead. 
    Here is the same diagram but instead only showing whether a tile is energized (#) or not (.):

    ######....
    .#...#....
    .#...#####
    .#...##...
    .#...##...
    .#...##...
    .#..####..
    ########..
    .#######..
    .#...#.#..

    Ultimately, in this example, 46 tiles become energized.

    The light isn't energizing enough tiles to produce lava; to debug the contraption, you need to start 
    by analyzing the current situation. With the beam starting in the top-left heading right, how many tiles end up being energized?
    */
    auto energized_tiles = find_energized_tiles(model);
    for (auto const& [r, c] : energized_tiles) {
      std::cout << NL << "Energized tile at " << r << "," << c;
    }
    print_energized(energized_tiles);
    result = energized_tiles.size();
    return result; // 6921
  }
}

namespace part2 {
  Result solve_for(Model& model) {
    Result result{};
    for (int r = 0; r < model.size(); ++r) {
      auto energized_tiles = find_energized_tiles(model, { r, -1, 0, 1 });
      result = std::max(result, static_cast<Result>(energized_tiles.size()));
      energized_tiles = find_energized_tiles(model, { r, model[0].size(), 0, -1 });
      result = std::max(result, static_cast<Result>(energized_tiles.size()));
    }

    for (int c = 0; c < model[0].size(); ++c) {
      auto energized_tiles = find_energized_tiles(model, { -1, c, 1, 0 });
      result = std::max(result, static_cast<Result>(energized_tiles.size()));
      energized_tiles = find_energized_tiles(model, { model.size(), c, -1, 0 });
      result = std::max(result, static_cast<Result>(energized_tiles.size()));
    }    
    return result; // 7594
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