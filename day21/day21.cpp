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

/*
You manage to catch the airship right as it's dropping someone else off 
on their all-expenses-paid trip to Desert Island! 
It even helpfully drops you off near the gardener and his massive farm.

"You got the sand flowing again! Great work! 
Now we just need to wait until we have enough sand to filter the water for Snow Island 
and we'll have snow again in no time."

While you wait, one of the Elves that works with the gardener 
heard how good you are at solving problems and would like your help. 
He needs to get his steps in for the day, and so he'd like to know which garden plots he can reach 
with exactly his remaining 64 steps.

He gives you an up-to-date map (your puzzle input) 
of his starting position (S), garden plots (.), and rocks (#). For example:

...........
.....###.#.
.###.##..#.
..#.#...#..
....#.#....
.##..S####.
.##..#...#.
.......##..
.##.#.####.
.##..##.##.
...........

The Elf starts at the starting position (S) which also counts as a garden plot. 

Then, he can take one step north, south, east, or west, but only onto tiles that are garden plots. 

This would allow him to reach any of the tiles marked O:

...........
.....###.#.
.###.##..#.
..#.#...#..
....#O#....
.##.OS####.
.##..#...#.
.......##..
.##.#.####.
.##..##.##.
...........

Then, he takes a second step. Since at this point he could be at either tile marked O, 
his second step would allow him to reach any garden plot 
that is one step north, south, east, or west of any tile that he could have reached after the first step:

...........
.....###.#.
.###.##..#.
..#.#O..#..
....#.#....
.##O.O####.
.##.O#...#.
.......##..
.##.#.####.
.##..##.##.
...........

After two steps, he could be at any of the tiles marked O above, 
including the starting position (either by going north-then-south or by going west-then-east).

A single third step leads to even more possibilities:

...........
.....###.#.
.###.##..#.
..#.#.O.#..
...O#O#....
.##.OS####.
.##O.#...#.
....O..##..
.##.#.####.
.##..##.##.
...........

He will continue like this until his steps for the day have been exhausted. 

After a total of 6 steps, he could reach any of the garden plots marked O:

...........
.....###.#.
.###.##.O#.
.O#O#O.O#..
O.O.#.#.O..
.##O.O####.
.##.O#O..#.
.O.O.O.##..
.##.#.####.
.##O.##.##.
...........

In this example, if the Elf's goal was to get exactly 6 more steps today, 
he could use them to reach any of 16 garden plots.

However, the Elf actually needs to get 64 steps today, 
and the map he's handed you is much larger than the example map.

Starting from the garden plot marked S on your map, 
how many garden plots could the Elf reach in exactly 64 steps?

*/

char const* example = R"(...........
.....###.#.
.###.##..#.
..#.#...#..
....#.#....
.##..S####.
.##..#...#.
.......##..
.##.#.####.
.##..##.##.
...........)";

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

using Model = std::vector<std::string>;

Model parse(auto& in) {
  Model result{};
  std::string line{};
  while (std::getline(in,line)) {
    result.push_back(line);
  }
  return result;
}

void print_model(Model const& model) {
  std::cout << NL;
  for (auto const& line : model) {
    std::cout << NL << line;
  }
}

namespace part1 {
  namespace hyperneutrino {

    int main(Model const& grid, int steps = 64) {

      // Find the starting position
      std::pair<int, int> start;
      for (int r = 0; r < grid.size(); ++r) {
        for (int c = 0; c < grid[r].size(); ++c) {
          if (grid[r][c] == 'S') {
            start = {r, c};
          }
        }
      }

      std::set<std::pair<int, int>> ans;
      auto const& [sr, sc] = start;
      std::set<std::tuple<int, int, int>> seen = {};
      std::deque<std::tuple<int, int, int>> q = {{start.first, start.second, steps}};

      // Breath-first search for all reachable positions until steps remaining (s) are 0
      while (!q.empty()) {
        auto [r, c, s] = q.front();
        q.pop_front();

        // if (s % 2 == 0) {
        //   ans.insert({r, c});
        // }

        // We have walked all the steps allowed
        if (s == 0) {
          ans.insert({r, c});
          continue;
        }

        // try all 4 directions for candidate steps
        for (auto [dr, dc] : std::vector<std::pair<int, int>>{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}) {
          int nr = r + dr;
          int nc = c + dc;

          // skip invalid or seen steps
          if (nr < 0 || nr >= grid.size() || nc < 0 || nc >= grid[0].size() || grid[nr][nc] == '#' || seen.contains({nr, nc,s+1})) {
            continue;
          }
          seen.insert({nr, nc,s});
          q.push_back({nr, nc, s - 1});
        }
      }

      std::cout << NL << "hyperneutrino says:" << ans.size();

      return 0;
    }    
  } // namespace hyperneutrino

  Result solve_for(Model& model,int steps = 64) {
    Result result{};
    std::cout << NL << NL << "part1";
    print_model(model);
    hyperneutrino::main(model,steps);
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
  for (int i = 0; i < argc; ++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
  }
  // day21 part file steps
  int part{1};
  std::string file{"example.txt"};
  int steps = file=="example.txt" ? 6 : 64;
  if (argc > 1 ) {
    part = std::stoi(argv[1]);
    if (argc > 2) {
      file = argv[2];
      if (argc > 3) {
        steps = std::stoi(argv[3]);
      }
    }
  }
  switch (part) {
  case 1: {
    std::cout << NL << "Part 1";
    std::ifstream in{ file };
    auto model = parse(in);
    auto answer = part1::solve_for(model,steps);
    solution.part1.push_back({ file,answer });
    break;
  }
  case 2: {
    std::cout << NL << "Part 2";
    std::ifstream in{ file };
    auto model = parse(in);
    auto answer = part2::solve_for(model);
    solution.part2.push_back({ file,answer });
    break;
  }
  default:
    std::cout << NL << "No part " << part << " only part 1 and 2";
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