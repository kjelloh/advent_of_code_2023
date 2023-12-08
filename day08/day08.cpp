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
    Ghost(Model const& model, Node current) : model{ model }, current{ current }, ghost_ix{++ghost_count} {}
    Model const& model;
    Node current;
    int step_ix{ 0 };
    int ghost_ix;
    using State = std::pair<int, Node>;
    std::set<State> visited{};
    bool at_end{};
    bool operator++() {
      if (visited.contains({ step_ix,current })) {
        at_end = true; // Break on coming back to a previously visitided node
      }
      else {
        visited.insert({ step_ix,current });
        auto turn = model.turns[step_ix];
        auto next = (turn == 'R') ? model.adj.at(current).right : model.adj.at(current).left;
        current = next;
        step_ix = (step_ix == model.turns.size() - 1) ? 0 : ++step_ix;
        at_end = is_end(current);
      }
      return at_end;
    }
  };
  using Ghosts = std::vector<Ghost>;
  Result solve_for(Model& model) {
    Result result{};
    Ghosts ghosts{};
    for (auto const& entry : model.adj) {
      if (is_start(entry.first)) {
        std::cout << NT << std::format("START {}", entry.first);
        ghosts.push_back({model,entry.first});
      }
      else if (is_end(entry.first)) std::cout << NT << std::format("{} END", entry.first);
      else std::cout << NL << std::format("{}", entry.first);
    }
    Result count{ 0 };
    while (std::any_of(ghosts.begin(), ghosts.end(), [&count](Ghost& ghost) {
      auto result = !ghost.at_end;
      if (result) {
        std::cout << NL << std::format("at step:{} Ghost:{} is at node:{} = NOT END", count, ghost.ghost_ix, ghost.current);
      }
      return result;
      })) {
      int ghost_not_at_end_count{};
      std::for_each(ghosts.begin(), ghosts.end(), [&ghost_not_at_end_count](Ghost& ghost) {++ghost; if (!ghost.at_end) ++ghost_not_at_end_count; });
      std::cout << std::format(" ghost_not_at_end_count:{}", ghost_not_at_end_count);
      ++count;
    }
    result = count;
    return result;
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