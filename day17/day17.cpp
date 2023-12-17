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


char const* example = R"(2413432311323
3215453535623
3255245654254
3446585845452
4546657867536
1438598798454
4457876987766
3637877979653
4654967986887
4564679986453
1224686865563
2546548887735
4322674655533)";

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

namespace part1 {
  constexpr int INF = std::numeric_limits<int>::max();

  struct State {
    int row;
    int col;
    int cost;
    int dr;
    int dc;
    int straight_step_count;
  };

  struct Compare {
    bool operator()(const State& a, const State& b) {
      return a.cost > b.cost;
    }
  };

  int dijkstra(const std::vector<std::string>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();

    std::vector<std::vector<int>> dist(rows, std::vector<int>(cols, INF));
    std::priority_queue<State, std::vector<State>, Compare> pq;

    dist[0][0] = 0; // Don't count the starting position heat loss
    pq.push({0, 0, 0, 0, 1, 0}); // start at 0,0 going dc=1 right

    while (!pq.empty()) {
      State curr = pq.top();
      pq.pop();

      int row = curr.row;
      int col = curr.col;
      int cost = curr.cost;
      int dr = curr.dr;
      int dc = curr.dc;
      int straight_step_count = curr.straight_step_count;
      std::cout << NL << "row : " << row << " col : " << col << " cost : " << cost << " dr : " << dr << " dc : " << dc << " straight_step_count : " << straight_step_count;

      if (cost > dist[row][col]) {
        continue; // skip if we have already found a better path
      }

      if (row == rows - 1 && col == cols - 1) {
        return cost;
      }

      for (int i = 0; i < 3; i++) {
        int newDr = 0;
        int newDc = 0;
        int new_straight_step_count = 0;

        if (i == 0) {
          // Go straight
          newDr = dr;
          newDc = dc;
          new_straight_step_count = straight_step_count + 1;
        } else if (i == 1) {
          // Go left
          newDr = -dc;
          newDc = dr;
          new_straight_step_count = 1;
        } else if (i == 2) {
          // Go right
          newDr = dc;
          newDc = -dr;
          new_straight_step_count = 1;
        }

        // Next position
        int newRow = row + newDr;
        int newCol = col + newDc;

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols and new_straight_step_count <= 3) {
            int newCost = cost + grid[newRow][newCol] - '0';
            if (newCost < dist[newRow][newCol]) { // Only update if the new cost is better
                dist[newRow][newCol] = newCost;
                pq.push({newRow, newCol, newCost, newDr, newDc, new_straight_step_count});
                std::cout << NT << "newRow : " << newRow << " newCol : " << newCol << " newCost : " << newCost << " newDr : " << newDr << " newDc : " << newDc << " new_straight_step_count : " << new_straight_step_count;
            }
        }
      }
    }

    return -1; // No path found
  }

  int dijkstra2(const std::vector<std::string>& grid) {
    std::set<std::tuple<int, int, int, int, int>> seen;
    std::priority_queue<std::tuple<int, int, int, int, int, int>, std::vector<std::tuple<int, int, int, int, int, int>>, std::greater<>> pq;
    pq.push({0, 0, 0, 0, 0, 0});

    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    while (!pq.empty()) {
      auto [hl, r, c, dr, dc, n] = pq.top();
      pq.pop();

      if (r == grid.size() - 1 && c == grid[0].size() - 1) {
        std::cout << NL << hl;
        return hl;
      }

      if (seen.count({r, c, dr, dc, n})) {
        continue;
      }

      seen.insert({r, c, dr, dc, n});

      if (n < 3 && (dr != 0 || dc != 0)) {
        int nr = r + dr;
        int nc = c + dc;
        if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[0].size()) {
          pq.push({hl + grid[nr][nc] - '0', nr, nc, dr, dc, n + 1});
        }
      }

      for (auto [ndr, ndc] : directions) {
        if ((ndr != dr || ndc != dc) && (ndr != -dr || ndc != -dc)) {
          int nr = r + ndr;
          int nc = c + ndc;
          if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[0].size()) {
            pq.push({hl + grid[nr][nc] - '0', nr, nc, ndr, ndc, 1});
          }
        }
      }
    }

    return 0;
  }
  Result solve_for(Model& model) {
      Result result{};
/*
The lava starts flowing rapidly once the Lava Production Facility is operational. 
As you leave, the reindeer offers you a parachute, allowing you to quickly reach Gear Island.

As you descend, your bird's-eye view of Gear Island reveals why you had trouble finding anyone on your way up: 
half of Gear Island is empty, but the half below you is a giant factory city!

You land near the gradually-filling pool of lava at the base of your new lavafall. 
Lavaducts will eventually carry the lava throughout the city, but to make use of it immediately, 
Elves are loading it into large crucibles on wheels.

The crucibles are top-heavy and pushed by hand. 
Unfortunately,the crucibles become very difficult to steer at high speeds, 
and so it can be hard to go in a straight line for very long.

To get Desert Island the machine parts it needs as soon as possible, 
you'll need to find the best way to get the crucible from the lava pool to the machine parts factory. 
To do this, you need to minimize heat loss while choosing a route that doesn't require 
the crucible to go in a straight line for too long.

Fortunately, the Elves here have a map (your puzzle input) that uses traffic patterns, 
ambient temperature, and hundreds of other parameters to calculate exactly how much heat loss 
can be expected for a crucible entering any particular city block.

For example:

2413432311323
3215453535623
3255245654254
3446585845452
4546657867536
1438598798454
4457876987766
3637877979653
4654967986887
4564679986453
1224686865563
2546548887735
4322674655533

Each city block is marked by a single digit that represents the amount of heat loss if the crucible enters that block. 
The starting point, the lava pool, is the top-left city block; 
the destination, the machine parts factory, is the bottom-right city block. 
(Because you already start in the top-left block, you don't incur that block's heat loss unless you leave that block and then return to it.)

Because it is difficult to keep the top-heavy crucible going in a straight line for very long, 
it can move at most three blocks in a single direction before it must turn 90 degrees left or right. 
The crucible also can't reverse direction; after entering each city block, it may only turn left, continue straight, or turn right.

One way to minimize heat loss is this path:

2>>34^>>>1323
32v>>>35v5623
32552456v>>54
3446585845v52
4546657867v>6
14385987984v4
44578769877v6
36378779796v>
465496798688v
456467998645v
12246868655<v
25465488877v5
43226746555v>

This path never moves more than three consecutive blocks in the same direction and incurs a heat loss of only 102.

Directing the crucible from the lava pool to the machine parts factory, 
but not moving more than three consecutive blocks in the same direction, 
what is the least heat loss it can incur?

*/    
    auto result1 = dijkstra(model);
    auto result2 = dijkstra2(model);
    return result2;
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