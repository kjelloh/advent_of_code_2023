#include <cctype>
#include <iostream>
#include <iomanip> // E.g., std::quoted
#include <string>
#include <string_view>
#include <sstream> // E.g., std::istringstream, std::ostringstream
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
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
#include <chrono>
#include <cassert>
#include <functional>


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

/*

--- Part Two ---

As you reach the trailhead, 
you realize that the ground isn't as slippery as you expected; 
you'll have no problem climbing up the steep slopes.

Now, treat all slopes as if they were normal paths (.). 

You still want to make sure you have the most scenic hike possible, 
so continue to ensure that you never step onto the same tile twice. 

What is the longest hike you can take?

In the example above, this increases the longest hike to 154 steps:

#S#####################
#OOOOOOO#########OOO###
#######O#########O#O###
###OOOOO#.>OOO###O#O###
###O#####.#O#O###O#O###
###O>...#.#O#OOOOO#OOO#
###O###.#.#O#########O#
###OOO#.#.#OOOOOOO#OOO#
#####O#.#.#######O#O###
#OOOOO#.#.#OOOOOOO#OOO#
#O#####.#.#O#########O#
#O#OOO#...#OOO###...>O#
#O#O#O#######O###.###O#
#OOO#O>.#...>O>.#.###O#
#####O#.#.###O#.#.###O#
#OOOOO#...#OOO#.#.#OOO#
#O#########O###.#.#O###
#OOO###OOO#OOO#...#O###
###O###O#O###O#####O###
#OOO#OOO#O#OOO>.#.>O###
#O###O###O#O###.#.#O###
#OOOOO###OOO###...#OOO#
#####################O#

Find the longest hike you can take 
through the surprisingly dry hiking trails listed on your map. 

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
  std::cout << NL << "print_model:";
  for (auto const& line : model) {
    std::cout << NL << line;
  }
}

struct Direction {
  int dr, dc;
};

struct Vector {
  int r, c;
  Vector(auto r,auto c) {
    // Ensure I can create a vector from container size() call expressions (they are are unsigned size_t...)
    this->r = static_cast<decltype(Vector::r)>(r); // decltype makes refactoring of type r and c easier
    this->c = static_cast<decltype(Vector::c)>(c);
  }
  bool operator==(Vector const& other) const {
    return this->r == other.r && this->c == other.c;
  }
  Vector operator+(Direction const& dir) const {
    return {this->r + dir.dr, this->c + dir.dc};
  }
  bool operator<(Vector const& other) const {
    return std::tie(this->r,this->c) < std::tie(other.r,other.c);
  }
};

using Directions = std::vector<Direction>;

namespace std {
  template <>
  struct hash<Vector> {
    std::size_t operator()(const Vector& v) const {
      auto [row, col] = v;
      std::size_t seed = 0;
      // Calculate hash based on vector properties
      // Combine the hash values of row and col using a hash function
      seed ^= std::hash<int>{}(row) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      seed ^= std::hash<int>{}(col) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      return seed;
    }
  };
}

Vector operator+(Vector const& lhs, Vector const& rhs) {
  auto [row1, col1] = lhs;
  auto [row2, col2] = rhs;
  return { row1 + row2, col1 + col2 };
}

namespace std {
  template <>
  struct hash< std::unordered_map<Vector, int>> {
    std::size_t operator()(const std::unordered_map<Vector, int>& map) const {
      std::size_t seed = 0;
      for (const auto& pair : map) {
        seed ^= std::hash<Vector>{}(pair.first) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>{}(pair.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      }
      return seed;
    }
  };
}

// Refactored from python solution https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day23p1.py
// Many thanks to hyperneutrino for the solution!
namespace hyperneutrino {


  // using Graph = std::map<Vector, std::map<Vector, int>>;
  using Graph = std::unordered_map<Vector, std::unordered_map<Vector, int>>; // solved in 107 756ms together with seen as std::unordered_set

  template <typename T>
  // using Seen = std::set<T>; // solved in 233 843 ms when Seen was std::set.
  using Seen = std::unordered_set<T>; // solved in 134 788 ms when Seen was std::unordered_set.

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

  template <bool ALL>
  Result count(Vector start,Vector end,Result max_steps,Model& grid) {
    // Note: Parameter max_steps is ignored (hyperneutrino's may have intially considered a recursive solution?)
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
    Graph graph;

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

        std::vector<Vector> neighborhood{};
        if constexpr (ALL==true) {
          // Consider all directions regardless of what tile we stand on
          neighborhood = std::vector<Vector> { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        }
        else {
          // Consider where to go next based on the tile grid[r][c] we stand on
        // directions to consider from a tile based on what the tile is
          const std::map<char, std::vector<Vector>> dirs = {
            {'^', {{-1, 0}}},
            {'v', {{1, 0}}},
            {'<', {{0, -1}}},
            {'>', {{0, 1}}},
            {'.', {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}}
          };
          neighborhood = dirs.at(grid[r][c]);
        }
        
        for (auto [dr, dc] : neighborhood) {
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

    return result; // part 1 (slippery slopes):2206 part 2 (ignore slopes):6490

  } // main

} // namespace hyperneutrino

namespace part1 {
  namespace mine {

    Directions to_directions(char tile) {
      switch (tile) {
      case '^': return { {-1, 0} }; // next must be up
      case 'v': return { {1, 0} }; // next mist be down
      case '<': return { {0, -1} }; // next must be left
      case '>': return { {0, 1} }; // next must be right
      default: return { {-1, 0}, {1, 0}, {0, -1}, {0, 1} }; // next is any up, down, left, right
      }
    }
    Result to_max_steps(Vector const& start, Vector const& end, Model& grid) {
      Result result{};
      // Idea: Traverse the grid so that all reachable positions is traveled once.
      //       Along the way, keep track of each branching and merging junction along the way.
      //       At each branch, restart a step counter to count the steps to the next junction.
      //       Along the way, store each function-to-junction path and step length in
      //       an adjacency list.
      //       Finally, brute force a way to travel the discovered subsections between junctions
      //       in the most steps possible (longest path).
      // We need:
      // 1. A way to traverse the grid without revisiting a position.
      // 2. A way to identify a branching junction specifically and any junction generally.
      // 3. A way to assemble the adjacency list dfs-stile (possibly recursively).
      // 4. A way to brute force the longest path between junctions. Again dfs-style and "smart" to reuse any already discovered distance to the end.
      return result;
    }


    void test_to_max_steps() {
      // #.#####################
      // #.......#########...###
      // #######.#########.#.###
      // ###.....#.>.>.###.#.###
      // ###v#####.#v#.###.#.###
      // ###.>...#.#.#.....#...#
      // ###v###.#.#.#########.#
      // ###...#.#.#.......#...#
      // #####.#.#.#######.#.###
      // #.....#.#.#.......#...#
      // #.#####.#.#.#########v#
      // #.#...#...#...###...>.#
      // #.#.#v#######v###.###v#
      // #...#.>.#...>.>.#.###.#
      // #####v#.#.###v#.#.###.#
      // #.....#...#...#.#.#...#
      // #.#########.###.#.#.###
      // #...###...#...#...#.###
      // ###.###.#.###v#####v###
      // #...#...#.#.>.>.#.>.###
      // #.###.###.#.###.#.#v###
      // #.....###...###...#...#
      // #####################.#
      std::istringstream in{example}; 
      auto example_grid = parse(in);

      bool more_to_test = true;
      int text_ix = 0;
      while (more_to_test) {
        bool result = false;
        switch (text_ix) {
          default: more_to_test = false; break;
        }
        if (!more_to_test) break;

        if (result) {
          std::cout << NL << "test_to_max_steps[" << text_ix << "] passed";
        }
        else {
          std::cout << NL << "test_to_max_steps[" << text_ix << "] FAILED";
          break;
        }
        ++text_ix;
      }
    }    

  } // namespace mine

  Result solve_for(Model& model,auto args) {
    Result result{};
    std::cout << NL << NL << "part1";
    auto const& [part,file,max_steps] = args;
    // find start and end
    auto sit = std::find(model[0].begin(),model[0].end(),'.');
    auto eit = std::find(model.back().begin(),model.back().end(),'.');
    Vector start = {0,std::distance(model[0].begin(),sit)};
    Vector end = {model.size()-1,std::distance(model.back().begin(),eit)};

    if (true) {
      mine::test_to_max_steps();
      result = mine::to_max_steps(start,end,model);
    }

    result = hyperneutrino::count<false>(start,end,max_steps,model); // see namespace comment on source of solution
    std::cout << NL << "result : " << result;
    return result; // 2206
  }
}

namespace part2 {
  Result solve_for(Model& model,auto args) {
    Result result{};
    std::cout << NL << NL << "part2";
    auto const& [part,file,max_steps] = args;
    auto sit = std::find(model[0].begin(),model[0].end(),'.');
    auto eit = std::find(model.back().begin(),model.back().end(),'.');
    Vector start = {0,std::distance(model[0].begin(),sit)};
    Vector end = {model.size()-1,std::distance(model.back().begin(),eit)};
    result = hyperneutrino::count<true>(start,end,max_steps,model);

    return result; // 6490
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
  std::cout << NL << "Part : " << part << " file : " << file /* << " max_steps : " << max_steps */;
  std::ifstream in{ file };
  auto model = parse(in);
  // Get the start time
  auto start = std::chrono::high_resolution_clock::now();
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
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << NL << NL << "------------ REPORT----------------";
  for (auto const& [part, answers] : solution) {
    std::cout << NL << "Part " << part << " answers";
    for (auto const& [heading, answer] : answers) {
      if (answer != 0) std::cout << NT << "answer[" << heading << "] " << answer;
      else std::cout << NT << "answer[" << heading << "] " << " NO OPERATION ";
    }
  }
    // Calculate and print the difference
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << NT << "solved in: " << duration.count() << "ms" << std::endl;  
  std::cout << NL << NL;
  return 0;
}