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

char const* example = R"(px{a<2006:qkq,m>2090:A,rfg}
pv{a>1716:R,A}
lnx{m>1548:A,A}
rfg{s<537:gd,x>2440:R,A}
qs{s>3448:A,lnx}
qkq{x<1416:A,crn}
crn{x>2662:A,R}
in{s<1351:px,qqz}
qqz{s>2770:qs,m<1801:hdj,R}
gd{a>3333:R,R}
hdj{m>838:A,pv}

{x=787,m=2655,a=1222,s=2876}
{x=1679,m=44,a=2067,s=496}
{x=2036,m=264,a=79,s=2244}
{x=2461,m=1339,a=466,s=291}
{x=2127,m=1623,a=2188,s=1013})";

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

using Workflow = std::pair<std::vector<std::tuple<char, char, int, std::string>>, std::string>;
using Workflows = std::map<std::string, Workflow>;
using Item = std::map<char, int>;

Workflows parseWorkflows(const std::string& block) {
  Workflows workflows;
  std::istringstream iss(block);
  std::string line;
  while (std::getline(iss, line)) {
    std::cout << NL << "line:" << std::quoted(line) << std::flush;
    std::string name = line.substr(0, line.find("{"));
    std::string rest = line.substr(line.find("{") + 1, line.size() - 2);
    std::istringstream iss2(rest);
    std::string rule;
    std::vector<std::tuple<char, char, int, std::string>> rules;
    while (std::getline(iss2, rule, ',')) {
      char key = rule[0];
      char cmp = rule[1];
      if (auto at = rule.find(":"); at != std::string::npos) {
        auto n_str = rule.substr(2, at - 2);
        int n = std::stoi(n_str);
        std::string target = rule.substr(at + 1);
        rules.push_back({key, cmp, n, target});
        std::cout << NT << " key:" << key << " cmp:" << cmp << " n:" << n << " target:" << target << std::flush;
      }
    }
    std::string fallback = std::get<3>(rules.back());
    rules.pop_back();
    workflows[name] = {rules, fallback};
    std::cout << NT << " workflows[" << name << "] = fallback:" << fallback << std::flush;
  }
  return workflows;
}

Item parseItem(const std::string& line) {
  std::cout << NL << "line:" << std::quoted(line) << std::flush;
  Item item;
  std::istringstream iss(line.substr(1, line.size() - 2));
  std::string segment;
  while (std::getline(iss, segment, ',')) {
    std::cout << NT << "segment:" << std::quoted(segment) << std::flush;
    char ch = segment[0];
    auto n_str = segment.substr(2);
    std::cout << NT << T << " 2:n_str:" << std::quoted(n_str) << std::flush;
    int n = std::stoi(segment.substr(2));
    item[ch] = n;
    std::cout << NT << "item[" << ch << "] = " << n << std::flush;
  }
  return item;
}

bool accept(const Item& item, const std::string& name, const Workflows& workflows) {
  if (name == "R") {
    return false;
  }
  if (name == "A") {
    return true;
  }
  const auto& [rules, fallback] = workflows.at(name);
  for (const auto& [key, cmp, n, target] : rules) {
    if ((cmp == '>' && item.at(key) > n) || (cmp == '<' && item.at(key) < n)) {
      return accept(item, target, workflows);
    }
  }
  return accept(item, fallback, workflows);
}

int main() {
  std::ifstream file("example.txt"); // replace with your file name
  std::string block1, block2, line;
  while (std::getline(file, line) && !line.empty()) {
    block1 += line + "\n";
  }
  while (std::getline(file, line)) {
    block2 += line + "\n";
  }
  Workflows workflows = parseWorkflows(block1);
  std::istringstream iss(block2);
  int total = 0;
  while (std::getline(iss, line)) {
    Item item = parseItem(line);
    if (accept(item, "in", workflows)) {
      total += std::accumulate(item.begin(), item.end(), 0, [](int sum, const auto& pair) {
        return sum + pair.second;
      });
    }
  }
  std::cout << NL << "answer2: " << total;
  return total;
}

  Result solve_for(Model& model) {
    Result result{};
    std::cout << NL << NL << "part1";
    main();
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