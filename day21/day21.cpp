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
#include <cassert>


/*

--- Part One ---

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

/*

--- Part Two ---

The Elf seems confused by your answer until he realizes his mistake: 
he was reading from a list of his favorite numbers 
that are both perfect squares and perfect cubes, not his step counter.

The actual number of steps he needs to get today is exactly 26501365.

He also points out that the garden plots and rocks are set up so 
that the map repeats infinitely in every direction.

So, if you were to look one additional map-width or map-height out 
from the edge of the example map above, you would find that it keeps repeating:

.................................
.....###.#......###.#......###.#.
.###.##..#..###.##..#..###.##..#.
..#.#...#....#.#...#....#.#...#..
....#.#........#.#........#.#....
.##...####..##...####..##...####.
.##..#...#..##..#...#..##..#...#.
.......##.........##.........##..
.##.#.####..##.#.####..##.#.####.
.##..##.##..##..##.##..##..##.##.
.................................
.................................
.....###.#......###.#......###.#.
.###.##..#..###.##..#..###.##..#.
..#.#...#....#.#...#....#.#...#..
....#.#........#.#........#.#....
.##...####..##..S####..##...####.
.##..#...#..##..#...#..##..#...#.
.......##.........##.........##..
.##.#.####..##.#.####..##.#.####.
.##..##.##..##..##.##..##..##.##.
.................................
.................................
.....###.#......###.#......###.#.
.###.##..#..###.##..#..###.##..#.
..#.#...#....#.#...#....#.#...#..
....#.#........#.#........#.#....
.##...####..##...####..##...####.
.##..#...#..##..#...#..##..#...#.
.......##.........##.........##..
.##.#.####..##.#.####..##.#.####.
.##..##.##..##..##.##..##..##.##.
.................................

This is just a tiny three-map-by-three-map slice 
of the inexplicably-infinite farm layout; 
garden plots and rocks repeat as far as you can see. 
The Elf still starts on the one middle tile marked S, 
though - every other repeated S is replaced with a normal garden plot (.).

Here are the number of reachable garden plots in this new infinite version 
of the example map for different numbers of steps:

In exactly 6 steps, he can still reach 16 garden plots.
In exactly 10 steps, he can reach any of 50 garden plots.
In exactly 50 steps, he can reach 1594 garden plots.
In exactly 100 steps, he can reach 6536 garden plots.
In exactly 500 steps, he can reach 167004 garden plots.
In exactly 1000 steps, he can reach 668697 garden plots.
In exactly 5000 steps, he can reach 16733044 garden plots.

However, the step count the Elf needs is much larger! 
Starting from the garden plot marked S on your infinite map, 
how many garden plots could the Elf reach in exactly 26501365 steps?
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
using Solution = std::map<int, Answers>; // Puzzle part -> Answers

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

using Position = std::pair<int, int>;

Result count(Model const& grid,Position start,int steps) {
  Result result{};
  std::set<std::pair<int, int>> ans;
  auto const& [sr, sc] = start;
  std::set<std::tuple<int, int, int>> seen = {}; // we have been here with the same number of steps remaining
  std::deque<std::tuple<int, int, int>> q = {{start.first, start.second, steps}};

  // Breath-first search for all reachable positions until steps remaining (s) are 0
  while (!q.empty()) {
    auto [r, c, s] = q.front();
    q.pop_front();

    if (s == 0) {
      // We have walked all the steps allowed
      ans.insert({r, c}); // reachable :)
      continue;
    }

    // try all 4 directions for candidate steps
    for (auto [dr, dc] : std::vector<std::pair<int, int>>{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}) {
      int nr = r + dr;
      int nc = c + dc;
      auto next = std::make_tuple(nr, nc, s - 1); // next pos with one less remaining steps to go
      if (nr < 0 || nr >= grid.size() || nc < 0 || nc >= grid[0].size() || grid[nr][nc] == '#' || seen.contains(next)) {
        continue; // skip invalid or seen next step
      }
      q.push_back(next);
      seen.insert(next); // all next steps on queue are seen
    }
  }
  result = ans.size();
  std::cout << NL << NL << "part1 say " << result << " reachable positions";

  return result;

}

namespace part1 {
  Result solve_for(Model& grid,int steps = 64) {
    Result result{};
    std::cout << NL << NL << "part1";
    print_model(grid);
    // Find the starting position
    std::pair<int, int> start;
    for (int r = 0; r < grid.size(); ++r) {
      for (int c = 0; c < grid[r].size(); ++c) {
        if (grid[r][c] == 'S') {
          start = {r, c};
        }
      }
    }
    result = count(grid,start,steps);
    std::cout << NL << NL << "part1 say " << result << " reachable positions";

    return result; // 3731
  }
}

namespace part2 {
  // Refactored from https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day21p2.py
  // Thanks!
  // Needed something to hold on to for my own exploration into this problem...
  namespace hyperneutrino {

    Integer fill(Model const& grid, Integer sr, Integer sc, Integer ss) {
      Result result = count(grid, {sr, sc}, ss);

      std::cout << NL << NL << "hyperneutrino::fill say " << result << " reachable positions";

      return result;
    }

    Integer main(Model const& grid,Integer steps = 26501365) {

      Integer sr, sc;
      for (Integer r = 0; r < grid.size(); ++r) {
        for (Integer c = 0; c < grid[r].size(); ++c) {
          if (grid[r][c] == 'S') {
            sr = r;
            sc = c;
          }
        }
      }

      Integer size = grid.size();

      std::cout << NL << "assert(sr:" << sr << " == sc:" << sc << " && sr:" << sr << " == size:" << size << " / 2 : "  << size/2 << ")";
      assert(sr == sc && sr == size / 2);
      std::cout << NL << "assert(steps:" << steps << " % size:" << size << " : " << steps%size << " == size:" << size << " / 2 : " << size / 2 << ")" << std::flush;
      assert(steps % size == size / 2);

      Integer grid_width = steps / size - 1;

      Integer odd = (grid_width / 2 * 2 + 1) * (grid_width / 2 * 2 + 1);
      Integer even = ((grid_width + 1) / 2 * 2) * ((grid_width + 1) / 2 * 2);

      Integer odd_points = fill(grid, sr, sc, size * 2 + 1);
      Integer even_points = fill(grid, sr, sc, size * 2);

      Integer corner_t = fill(grid, size - 1, sc, size - 1);
      Integer corner_r = fill(grid, sr, 0, size - 1);
      Integer corner_b = fill(grid, 0, sc, size - 1);
      Integer corner_l = fill(grid, sr, size - 1, size - 1);

      Integer small_tr = fill(grid, size - 1, 0, size / 2 - 1);
      Integer small_tl = fill(grid, size - 1, size - 1, size / 2 - 1);
      Integer small_br = fill(grid, 0, 0, size / 2 - 1);
      Integer small_bl = fill(grid, 0, size - 1, size / 2 - 1);

      Integer large_tr = fill(grid, size - 1, 0, size * 3 / 2 - 1);
      Integer large_tl = fill(grid, size - 1, size - 1, size * 3 / 2 - 1);
      Integer large_br = fill(grid, 0, 0, size * 3 / 2 - 1);
      Integer large_bl = fill(grid, 0, size - 1, size * 3 / 2 - 1);

      Integer result = odd * odd_points +
            even * even_points +
            corner_t + corner_r + corner_b + corner_l +
            (grid_width + 1) * (small_tr + small_tl + small_br + small_bl) +
            grid_width * (large_tr + large_tl + large_br + large_bl);

      std::cout << NL << NL << "hyperneutrino::main say " << result << " reachable positions";

      return result;
    }    
  } // namespace hyperneutrino
  Result solve_for(Model& grid,int steps = 26501365) {
    Result result{};
    std::cout << NL << NL << "part2";
    print_model(grid);
    result = hyperneutrino::main(grid,steps);
    std::cout << NL << NL << "part2 say " << result << " reachable positions";
    return result; // 617565692567199 (thanks hyperneutrino!)
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
      else {
        steps = part == 2? 26501365 : 64;
      }
    }
  }
  std::cout << NL << "Part : " << part << " file : " << file << " steps : " << steps;
  std::ifstream in{ file };
  auto model = parse(in);

  switch (part) {
  case 1: {
    auto answer = part1::solve_for(model,steps);
    solution[part].push_back({ file,answer });
  } break;
  case 2: {
    auto answer = part2::solve_for(model,steps);
    solution[part].push_back({ file,answer });
  } break;
  default:
    std::cout << NL << "No part " << part << " only part 1 and 2";
  }

  std::cout << NL << NL << "------------ REPORT----------------";
  for (auto const& [part, answers] : solution) {
    std::cout << NL << "Part " << part << " answers";
    for (auto const& [heading, answer] : answers) {
      if (answer != 0) std::cout << NT << "answer[" << heading << "] " << answer;
      else std::cout << NT << "answer[" << heading << "] " << " NO OPERATION ";
    }
  }
  std::cout << NL << NL;
  return 0;
}