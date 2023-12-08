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

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = std::int64_t; // 16 bit int: ± 3.27 · 10^4, 32 bit int: ± 2.14 · 10^9, 64 bit int: ± 9.22 · 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
struct Solution {
  Answers part1{};
  Answers part2{};
};
char const* example = R"(RL

AAA = (BBB, CCC)
BBB = (DDD, EEE)
CCC = (ZZZ, GGG)
DDD = (DDD, DDD)
EEE = (EEE, EEE)
GGG = (GGG, GGG)
ZZZ = (ZZZ, ZZZ))";
using Node = std::string;
struct LRNodes {
  Node left;
  Node right;
};
using AdjList = std::map<Node, LRNodes>;
struct Model {
  std::string turns{};
  AdjList adj{};
};
auto parse(auto& in) {
  Model result{};
  std::string line;
  std::getline(in, line);
  result.turns = line;
  std::cout << NL << "turns:" << std::quoted(result.turns);

  // Regular expression pattern for parsing adjacent node list
  std::regex pattern(R"(\s*(\w+)\s*=\s*\((\w+),\s*(\w+)\)\s*)");

  // Iterating over matches
  while (std::getline(in, line)) {
    std::smatch match;
    if (std::regex_match(line, match, pattern)) {
      std::string key = match[1].str();
      std::string value1 = match[2].str();
      std::string value2 = match[3].str();

      result.adj[key] = { value1, value2 };
    }
  }

  // Printing the result
  for (const auto& entry : result.adj) {
    std::cout << NL << std::quoted(entry.first) << " --> left:" << std::quoted(entry.second.left) << ", right:" << std::quoted(entry.second.right);
  }    return result;
}

namespace part1 {
  Result solve_for(Model& model) {
    Result result{};
    // Traverse the graph according to the turns string until we reach the ZZZ-node
    Node end{"ZZZ"};
    Node current{ "AAA" };
    int step_ix{ 0 };
    Result count{ 0 };
    while (current != end) {
      ++count;
      auto turn = model.turns[step_ix];
      auto next = (turn == 'R') ? model.adj[current].right : model.adj[current].left;
      std::cout << NT << std::format("step:{} at:{} turn[{}]:{} --> next:{}",count,current,step_ix,turn,next);
      current = next;
      step_ix = (step_ix == model.turns.size()-1) ? 0 : ++step_ix;
    }
    result = count;
    return result; // 12361
  }
}

namespace part2 {
  bool is_start(Node const& node) {
    return node.back() == 'A';
  }
  bool is_end(Node const& node) {
    return node.back() == 'Z';
  }
  struct Ghost {
    static inline int ghost_count{ 0 };
    Ghost(Model const& model, Node start) : model{ model }, current{ start }, ghost_ix{ ghost_count++ } { path.push_back(start); }
    Model const& model;
    Node current;
    int step_ix{ 0 };
    int ghost_ix;
    using State = std::pair<Result, Node>;
    std::map<State,std::vector<Result>> steps{}; // Record the steps each time we reach the same state
    bool full_cycle{false};
    Result step_count{ 0 };
    struct Cycle {
      Result offset{};
      Result length{};
    };
    Cycle cycle{};
    std::vector<Node> path{};
    bool operator++() {
      // We expect to step through one or more potential end nodes.
      // We also expect to come back to one of these at some point (we expect a cycle)
      // But we do not know where the cycle starts if there are several possible end nodes on our path.
      static int call_count;
      if (!full_cycle) {
        ++step_count;
        if (call_count++ % 10000 == 0) std::cout << NL << std::format("step:{} ghost:{}", step_count, ghost_ix);
        auto turn = model.turns[step_ix];
        // Next
        auto next = (turn == 'R') ? model.adj.at(current).right : model.adj.at(current).left;
        // std::cout << std::format(" turn:{} current:{} --> next:{}",ghost_ix,turn,current,next);
        step_ix = (step_ix == model.turns.size() - 1) ? 0 : ++step_ix;
        current = next;
        path.push_back(current);
        State current_state{ step_ix,current };
        if (is_end(current)) {
          std::cout << std::format(" Candidate end state step_ix:{} pushed steps:{}", step_ix,step_count);
          steps[current_state].push_back(step_count);
        }
        full_cycle = steps.contains(current_state) and steps[current_state].size() == 2; // We came here two times
        if (full_cycle) {
          // We now know that we have made a full cycle back to to the current state (which is also and end state)
          // The offset is the number of steps it took us to reach this state the first time
          // The cycle is the number of steps until we came back again!
          cycle.offset = steps[current_state][0];
          cycle.length = steps[current_state][1] - cycle.offset;
        }
      }
      return full_cycle;
    }
  };
  using Ghosts = std::vector<Ghost>;
  
  // Least Common Multiplier of a vector of integer T:s
  template <typename T>
  T lcm(std::vector<T> numbers) {
    if (numbers.empty()) {
      // Handle the case when the vector is empty
      return 0;
    }
    // Use std::accumulate with std::lcm to find the LCM of all numbers
    // NOTE: std::accumulate returns the type of provided initial value (important to provide T{ 1 } and not literal '1' which have a much smaller int type)
    auto result = std::accumulate(numbers.begin(), numbers.end(), T{ 1 }, [](T acc, T val) {
      auto result = std::lcm(acc, val);
      return result;
      });
    return result;
  }

  Result solve_for(Model& model) {
    Result result{};
    Ghosts ghosts{};
    for (auto const& entry : model.adj) {
      if (is_start(entry.first)) {
        ghosts.push_back({model,entry.first});
      }
    }
    while (std::any_of(ghosts.begin(), ghosts.end(), [](Ghost& ghost) {return (ghost.full_cycle==false); })) {
      // For each ghost it takes some initial steps to reach its first end-node. We can call this the initial offset
      // Then, for each ghost it takes some other number of steps to come back to its start state. This is the cycle length for this ghost to wrap-around and repeat its path of nodes
      // Given these two parameters for each ghost we can calculate how many steps is required for all ghosts to reach their end states at the same time.
      std::for_each(ghosts.begin(), ghosts.end(), [](auto& ghost) {++ghost; });
    }

    std::vector<Result> cycleLengths{};
    std::vector<Result> initialOffsets{};
    for (auto const& ghost : ghosts) {
      std::cout << NT << std::format("ghost:{} cycle:(offset:{},length:{})",ghost.ghost_ix,ghost.cycle.offset,ghost.cycle.length);
      // std::cout << NT << "path:";
      // for (auto const& node : ghost.path) std::cout << " " << node;
      initialOffsets.push_back(ghost.cycle.offset);
      cycleLengths.push_back(ghost.cycle.length);
    }

    auto cyclesLCM = lcm(cycleLengths);
    std::cout << NL << std::format("All ghosts share the overall cycle of {} steps", cyclesLCM);
    auto offsetsLCM = lcm(initialOffsets);
    std::cout << NL << std::format("the LCM of all ghost initial offset to cycled end node is {} steps ", offsetsLCM);
    auto overallLCM = std::lcm(offsetsLCM, cyclesLCM);
    std::cout << NL << std::format("the LCM of all offsets and cycles is {} steps ", overallLCM);
    if (offsetsLCM <= overallLCM) {
      std::cout << NL << std::format("all ghosts should be be at an end-node after {} steps", overallLCM);
    }
    else {
      std::cerr << NL << std::format("ERROR: There seems to be no way to have the offsets with LCM {} to line up within overall cycle LCM {}?", offsetsLCM, overallLCM);
    }

    /* For part 2 example
    * 
      Candidate end state step_ix:1 pushed steps:9
              ghost:0 cycle:(offset:2,length:2)
              ghost:1 cycle:(offset:3,length:6)
      All ghosts share the overall cycle of 6 steps
      the LCM of all ghost initial offset to cycled end node is 6 steps
      the LCM of all offsets and cycles is 6 steps
      all ghosts should be be at an end-node after 6 steps
    */

    /* For part puzzle input
              ghost:0 cycle:(offset:12361,length:12361)
              ghost:1 cycle:(offset:20777,length:20777)
              ghost:2 cycle:(offset:16043,length:16043)
              ghost:3 cycle:(offset:19199,length:19199)
              ghost:4 cycle:(offset:18673,length:18673)
              ghost:5 cycle:(offset:15517,length:15517)
      All ghosts share the overall cycle of 18215611419223 steps
      the LCM of all ghost initial offset to cycled end node is 18215611419223 steps
      the LCM of all offsets and cycles is 18215611419223 steps
      all ghosts should be be at an end-node after 18215611419223 steps
    */

    result = overallLCM;

    return result; // 18 215 611 419 223
  }
}

int main(int argc, char *argv[])
{
  Solution solution{};
  std::cout << NL << "argc : " << argc;
  Answer part1_answer{"Failed to obtain any input",0};
  Answer part2_answer{ "Failed to obtain any input",0 };
  if (argc == 1) {
    std::cout << NL << "no data file provided ==> Will use hard coded example input";
    std::istringstream in{ example };
    auto model = parse(in);
    // part1_answer = { "Example",part1::solve_for(model)};
    part2_answer = { "Example",part2::solve_for(model)};
    // solution.part1.push_back(part1_answer);
    solution.part2.push_back(part2_answer);
  }
  else for (int i = 0; i < argc; ++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
    if (i > 0) {
      std::ifstream in{ argv[i] };
      if (in) {
        auto model = parse(in);
        // part1_answer = {argv[i],part1::solve_for(model)};
        part2_answer = { argv[i],part2::solve_for(model) };
      }
      else {
        // part1_answer = { std::string{"Failed to open file "} + argv[i],-1 };
        part2_answer = { std::string{"Failed to open file "} + argv[i],-1 };
      }
      // solution.part1.push_back(part1_answer);
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