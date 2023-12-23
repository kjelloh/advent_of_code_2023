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


char const* example = R"(#.#####################
#.......#########...###
#######.#########.#.###
###.....#.>.>.###.#.###
###v#####.#v#.###.#.###
###.>...#.#.#.....#...#
###v###.#.#.#########.#
###...#.#.#.......#...#
#####.#.#.#######.#.###
#.....#.#.#.......#...#
#.#####.#.#.#########v#
#.#...#...#...###...>.#
#.#.#v#######v###.###v#
#...#.>.#...>.>.#.###.#
#####v#.#.###v#.#.###.#
#.....#...#...#.#.#...#
#.#########.###.#.#.###
#...###...#...#...#.###
###.###.#.###v#####v###
#...#...#.#.>.>.#.>.###
#.###.###.#.###.#.#v###
#.....###...###...#...#
#####################.#)";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int64_t; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22 x 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
using Solution = std::map<int, Answers>; // Puzzle part -> Answers

/*
--- Day 23: A Long Walk ---

The Elves resume water filtering operations! 
Clean water starts flowing over the edge of Island Island.

They offer to help you go over the edge of Island Island, too! 
Just hold on tight to one end of this impossibly long rope 
and they'll lower you down a safe distance from the massive waterfall you just created.

As you finally reach Snow Island, 
you see that the water isn't really reaching the ground: 
it's being absorbed by the air itself. 
It looks like you'll finally have a little downtime 
while the moisture builds up to snow-producing levels. 
Snow Island is pretty scenic, even without any snow; why not take a walk?

There's a map of nearby hiking trails (your puzzle input) 
that indicates paths (.), forest (#), and steep slopes (^, >, v, and <).

For example:

#.#####################
#.......#########...###
#######.#########.#.###
###.....#.>.>.###.#.###
###v#####.#v#.###.#.###
###.>...#.#.#.....#...#
###v###.#.#.#########.#
###...#.#.#.......#...#
#####.#.#.#######.#.###
#.....#.#.#.......#...#
#.#####.#.#.#########v#
#.#...#...#...###...>.#
#.#.#v#######v###.###v#
#...#.>.#...>.>.#.###.#
#####v#.#.###v#.#.###.#
#.....#...#...#.#.#...#
#.#########.###.#.#.###
#...###...#...#...#.###
###.###.#.###v#####v###
#...#...#.#.>.>.#.>.###
#.###.###.#.###.#.#v###
#.....###...###...#...#
#####################.#

You're currently on the single path tile in the top row; 
your goal is to reach the single path tile in the bottom row. 
Because of all the mist from the waterfall, the slopes are probably quite icy; 
if you step onto a slope tile, 
your next step must be downhill (in the direction the arrow is pointing). 

To make sure you have the most scenic hike possible, 
never step onto the same tile twice. What is the longest hike you can take?

In the example above, 
the longest hike you can take is marked with O, and your starting position is marked S:

#S#####################
#OOOOOOO#########...###
#######O#########.#.###
###OOOOO#OOO>.###.#.###
###O#####O#O#.###.#.###
###OOOOO#O#O#.....#...#
###v###O#O#O#########.#
###...#O#O#OOOOOOO#...#
#####.#O#O#######O#.###
#.....#O#O#OOOOOOO#...#
#.#####O#O#O#########v#
#.#...#OOO#OOO###OOOOO#
#.#.#v#######O###O###O#
#...#.>.#...>OOO#O###O#
#####v#.#.###v#O#O###O#
#.....#...#...#O#O#OOO#
#.#########.###O#O#O###
#...###...#...#OOO#O###
###.###.#.###v#####O###
#...#...#.#.>.>.#.>O###
#.###.###.#.###.#.#O###
#.....###...###...#OOO#
#####################O#

This hike contains 94 steps. 
(The other possible hikes you could have taken were 90, 86, 82, 82, and 74 steps long.)

Find the longest hike you can take through the hiking trails listed on your map. 

How many steps long is the longest hike?

*/

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
  std::cout << NL << "Model";
  for (auto const& line : model) {
    std::cout << NT << line;
  }
}

namespace part1 {
  using Vector = std::tuple<int,int>;
  Vector operator+(Vector const& lhs, Vector const& rhs) {
    auto [row1, col1] = lhs;
    auto [row2, col2] = rhs;
    return { row1 + row2, col1 + col2 };
  }

  struct Walker {
    Vector pos{};
    Vector dir{};
    bool operator<(Walker const& rhs) const {
      return std::tie(pos,dir) < std::tie(rhs.pos,rhs.dir);
    }
    bool operator!=(Walker const& rhs) const {
      return std::tie(pos,dir) != std::tie(rhs.pos,rhs.dir);
    }
  };

  struct State {
    Walker walker{};
    Result walked{};
    bool operator<(State const& rhs) const {
      return std::tie(walker,walked) < std::tie(rhs.walker,rhs.walked);
    }
  };

  bool on_grid(Vector pos,Model const& grid) {
    auto [row,col] = pos;
    return row >= 0 and row < grid.size() and col >= 0 and col < grid[row].size();
  }

  // if you step onto a slope tile, 
  // your next step must be downhill (in the direction the arrow is pointing).
  Walker to_slipped(Walker walker,Model const& grid) {
    Walker result{walker};
    auto [pos,dir] = walker;
    if (on_grid(pos,grid)) {
      auto [row,col] = pos;
      switch (grid[row][col]) {
      case '^': dir = {row-1,col};break; // slip up
      case '>': dir = {row,col+1};break; // slip right
      case 'v': dir = {row+1,col};break; // slip down
      case '<': dir =  {row,col-1};break; // slip left
      default: dir = {0,0};break; // no slip
      }
    }
    result = {pos + dir,dir}; // apply any slip
    return (on_grid(result.pos,grid)) ? result : walker;
  }

  // max number of steps to reach end from provided state
  Result max_to(Vector start,Vector end,Result max_steps,Model& grid) {
    Result result{};
    auto const& [srow,scol] = start;
    auto const& [erow,ecol] = end;
    std::cout << NL << "max_to(start:" << srow << "," << scol << " end:" << erow << "," << ecol << " max_steps:" << max_steps << ")" << std::flush;
    // 1) Get the walking working and flood fill in n steps.
    // 2) Find any path to end
    // 3) Find the longest path to seen positions (break at max_steps)
    // 4) Find the longest path to end (break at max_int of result)
    // 5) Walk with heuristics? (are there any preferred steps to try before others?)
    std::queue<State> q{};
    State down_state{ {start,{1,0}},0 };
    State right_state{ {start,{0,1}},0 };
    q.push({down_state});
    q.push({right_state});
    std::set<Walker> dont_visit_again{};
    std::map<Vector,Result> best_from_start{};
    while (!q.empty()) {
      auto current = q.front();
      q.pop();
      auto& [walker,walked] = current;
      if (dont_visit_again.contains(walker)) continue;
      dont_visit_again.insert(walker); // block walking here again
      if (walker.pos == end) {
        // Did we get here and walked more steps than previously known?
        if (walked > result) {
          result = walked;
          best_from_start[walker.pos] = result;
        }
        std::cout << NL << "END! walked " << walked << " best_from_start " << best_from_start[walker.pos] << " still in queue:" << q.size() << std::flush;
      }
      else if (walked > max_steps) {
        // update best walking up to the limit of steps
        if (walked > result) {
          result = walked;
          best_from_start[walker.pos] = result;
        }
        std::cout << NL << "MAX walked " << walked << " max_steps " << max_steps << " still in queue are:" << q.size() << std::flush;
      }
      else {
        // try next possible steps
        std::vector<Vector> dirs{{0,1},{1,0},{0,-1},{-1,0}};
        for (auto const& dir : dirs) {
          Walker candidate{walker.pos + dir,dir};
          if (!on_grid(candidate.pos,grid)) continue; // can't walk off the grid ;
          candidate = to_slipped(candidate,grid);
          auto [nr,nc] = candidate.pos;
          if (grid[nr][nc] == '#') continue; // can't walk into the forrest ;)
          if (candidate != walker) {
            // Queue an actual step
            q.push({candidate,walked+1}); // push next to examine later
            std::cout << NL << "pushed " << nr << "," << nc << " walked " << walked+1 << std::flush; 
          }
        }
      }
    }
    auto walked_grid = grid;
    for (auto const& walker : dont_visit_again) {
      auto [row,col] = walker.pos;
      if (grid[row][col] == '.') walked_grid[row][col] = 'O';
    }
    print_model(walked_grid);
    return result;
  }

  // Refactored from python solution 
  namespace hyperneutrino {

    using Graph = std::map<Vector, std::map<Vector, int>>;
    template <typename State>
    using Seen = std::set<State>;

    // Recursively travel the compressed graph to find the longest path from start to end
    // Remember, we have stored the max number of steps to reach each junction.
    // So all left to do is to find a way to travel between junctions the most inefficient way possible (longest path) ;)
    int dfs(Vector pt,Graph& graph, Seen<Vector>& seen,Vector const& end) {
      if (pt == end) {
        return 0;
      }

      int m = std::numeric_limits<int>::min();

      seen.insert(pt);
      for (auto nx : graph[pt]) {
        if (seen.find(nx.first) == seen.end()) {
          m = std::max(m, dfs(nx.first,graph,seen,end) + nx.second);
        }
      }
      seen.erase(pt);

      return m;
    }


    Result main(Vector start,Vector end,Result max_steps,Model& grid) {
      Result result{};
      // Vertices in "compressed" graph between junctions in the order merge junction, split junction, merge junction.
      // In our puzzle we only have splits into two and merge two into one.
      // We also regard start and end as junctions, only the graph has no incoming to start and no outgoings from end.
      std::vector<Vector> points = {start, end}; 

      // Find the junctions (tiles that have three "outs" = not blocked by '#')
      for (int r = 0; r < grid.size(); r++) {
        for (int c = 0; c < grid[0].size(); c++) {
          if (grid[r][c] == '#') {
            continue;
          }
          int neighbors = 0;
          for (auto [nr, nc] : std::vector<Vector>{{r - 1, c}, {r + 1, c}, {r, c - 1}, {r, c + 1}}) {
            if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[0].size() && grid[nr][nc] != '#') {
              // non blocked neighbour
              neighbors += 1;
            }
          }
          if (neighbors >= 3) {
            // a junction one-to-two or two-to-one or start or end.
            points.push_back({r, c});
          }
        }
      }

      // adjacency list for our compressed graph
      std::map<Vector, std::map<Vector, int>> graph;

      // directions to consider from a tile based on what the tile is
      std::map<char, std::vector<Vector>> dirs = {
        {'^', {{-1, 0}}},
        {'v', {{1, 0}}},
        {'<', {{0, -1}}},
        {'>', {{0, 1}}},
        {'.', {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}}
      };

      // walk the graph (adjacency list)
      for (auto [sr, sc] : points) {
        std::stack<std::tuple<int, int, int>> stack; // step count, row, col
        Seen<Vector> seen; // We don't care about the direction with which we reach a tile (as we are walking a labyrinth and are not allowed to back track)
        stack.push({0, sr, sc}); // push start with zero step count
        seen.insert({sr, sc}); // Block coming back to seen.

        while (!stack.empty()) {
          auto [n, r, c] = stack.top(); // step count,row,column
          stack.pop();

          if (n != 0 && std::find(points.begin(), points.end(), Vector{r, c}) != points.end()) {
            // we have gone steps and reached a junction (n>0 skips start)
            graph[{sr, sc}][{r, c}] = n; // add edge from start to the pair ((r,c),n steps)
            continue; // don't consider any more steps from this junction
          }

          // Consider where to go next based on the tile grid[r][c] we stand on
          for (auto [dr, dc] : dirs[grid[r][c]]) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[0].size() && grid[nr][nc] != '#' && seen.find({nr, nc}) == seen.end()) {
              // We reached a new valid position, that is on grid, not seen and not a forrest tile
              stack.push({n + 1, nr, nc}); // push an increment step count and the new position
              seen.insert({nr, nc}); // dont allow coming back to this position again
            }
          }
        }
      }

      Seen<Vector> seen;

      result = dfs(start,graph, seen,end);

      std::cout << NL << "hyperneutrino says:" << result << std::endl;

      return result;

    } // main

  } // namespace hyperneutrino

  Result solve_for(Model& model,auto args) {
    Result result{};
    std::cout << NL << NL << "part1";
    auto const& [part,file,max_steps] = args;
    // find start and end
    auto sit = std::find(model[0].begin(),model[0].end(),'.');
    auto eit = std::find(model.back().begin(),model.back().end(),'.');
    Vector start = {0,std::distance(model[0].begin(),sit)};
    Vector end = {model.size()-1,std::distance(model.back().begin(),eit)};
    // result = max_to(start,end,max_steps,model);
    result = hyperneutrino::main(start,end,max_steps,model);
    std::cout << NL << "result : " << result;
    return result;
  }
}

namespace part2 {
  Result solve_for(Model& model,auto args) {
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
  // day23 part file max_steps
  std::tuple<int,std::string,int> args{1,"example.txt",10};
  auto& [part,file,max_steps] = args;
  if (argc > 1 ) {
    part = std::stoi(argv[1]);
    if (argc > 2) {
      file = argv[2];
    }
    if (argc > 3) {
      max_steps = std::stoi(argv[3]);
    }
  }
  std::cout << NL << "Part : " << part << " file : " << file << " max_steps : " << max_steps;
  std::ifstream in{ file };
  auto model = parse(in);

  switch (part) {
  case 1: {
    auto answer = part1::solve_for(model,args);
    solution[part].push_back({ file,answer });
  } break;
  case 2: {
    auto answer = part2::solve_for(model,args);
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