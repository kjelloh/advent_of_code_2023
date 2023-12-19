#include <cctype>
#include <iostream>
#include <iomanip> // E.g., std::quoted
#include <string>
#include <string_view>
#include <sstream> // E.g., std::istringstream, std::ostringstream
#include <vector>
#include <set>
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

using Rule = std::tuple<char, char, int, std::string>;
using Rules = std::vector<Rule>;
using Workflow = std::pair<Rules, std::string>;
using Workflows = std::map<std::string, Workflow>;
using Item = std::vector<std::tuple<char, int>>;
using Items = std::vector<Item>;

struct Model {
  Workflows workflows;
  Items items;
};

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
      else {
        rules.push_back({key, cmp, 0, rule.substr(0,rule.size()-1)});
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
    item.push_back({ch,n});
    std::cout << NT << "item[" << ch << "] = " << n << std::flush;
  }
  return item;
}


Model parse(auto& in) {
  Model result{};
  std::string line{};
  std::string block1, block2;
  while (std::getline(in, line) && !line.empty()) {
    block1 += line + "\n";
  }
  while (std::getline(in, line)) {
    block2 += line + "\n";
  }
  Workflows workflows = parseWorkflows(block1);
  Items items{};
  std::istringstream iss(block2);
  int total = 0;
  while (std::getline(iss, line)) {
    auto item = parseItem(line);
    items.push_back(item);  
  }
  return {workflows, items};
}

void print_workflow(const std::string& name, const Workflow& workflow) {
  std::cout << NL << "workflow[" << name << "]";
  const auto& [rules, fallback] = workflow;
  for (const auto& [key, cmp, n, target] : rules) {
    std::cout << " if: " << key;
    std::cout << " " << cmp;
    std::cout << " " << n;
    std::cout << " then " << target << ";";
  }
  std::cout << " else:" << fallback;
}

namespace part1 {


  // On the learning streak this day, following hyperneutrion solution for insight
  // https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day19p1.py
  namespace hyperneutrino {

    // Workflows parseWorkflows(const std::string& block) {
    //   Workflows workflows;
    //   std::istringstream iss(block);
    //   std::string line;
    //   while (std::getline(iss, line)) {
    //     std::cout << NL << "line:" << std::quoted(line) << std::flush;
    //     std::string name = line.substr(0, line.find("{"));
    //     std::string rest = line.substr(line.find("{") + 1, line.size() - 2);
    //     std::istringstream iss2(rest);
    //     std::string rule;
    //     std::vector<std::tuple<char, char, int, std::string>> rules;
    //     while (std::getline(iss2, rule, ',')) {
    //       char key = rule[0];
    //       char cmp = rule[1];
    //       if (auto at = rule.find(":"); at != std::string::npos) {
    //         auto n_str = rule.substr(2, at - 2);
    //         int n = std::stoi(n_str);
    //         std::string target = rule.substr(at + 1);
    //         rules.push_back({key, cmp, n, target});
    //         std::cout << NT << " key:" << key << " cmp:" << cmp << " n:" << n << " target:" << target << std::flush;
    //       }
    //       else {
    //         rules.push_back({key, cmp, 0, rule.substr(0,rule.size()-1)});
    //       }
    //     }
    //     std::string fallback = std::get<3>(rules.back());
    //     rules.pop_back();
    //     workflows[name] = {rules, fallback};
    //     std::cout << NT << " workflows[" << name << "] = fallback:" << fallback << std::flush;
    //   }
    //   return workflows;
    // }

    // Item parseItem(const std::string& line) {
    //   std::cout << NL << "line:" << std::quoted(line) << std::flush;
    //   Item item;
    //   std::istringstream iss(line.substr(1, line.size() - 2));
    //   std::string segment;
    //   while (std::getline(iss, segment, ',')) {
    //     std::cout << NT << "segment:" << std::quoted(segment) << std::flush;
    //     char ch = segment[0];
    //     auto n_str = segment.substr(2);
    //     std::cout << NT << T << " 2:n_str:" << std::quoted(n_str) << std::flush;
    //     int n = std::stoi(segment.substr(2));
    //     item.push_back({ch,n});
    //     std::cout << NT << "item[" << ch << "] = " << n << std::flush;
    //   }
    //   return item;
    // }

    bool accept(const Item& item, const std::string& name, const Workflows& workflows) {
      if (name != "in") std::cout << " ->";
      std::cout << " " << name;

      if (name == "R") {
        return false;
      }
      if (name == "A") {
        return true;
      }
      const auto& [rules, fallback] = workflows.at(name);
      print_workflow(name, workflows.at(name));
      for (const auto& [key, cmp, n, target] : rules) {
        auto iter = std::find_if(item.begin(), item.end(), [key=key](const auto& pair) {
          auto [ch, n] = pair; // 
          return ch == key;
        });
        auto [ch, n2] = *iter;
        if ((cmp == '>' && n2 > n) || (cmp == '<' && n2 < n)) {
          std::cout << " !" << key;
          return accept(item, target, workflows);
        }
      }
      return accept(item, fallback, workflows);
    }

    // Result main() {
    //   // std::ifstream file("example.txt"); // replace with your file name
    //   std::ifstream file("puzzle.txt"); // replace with your file name
    //   std::string block1, block2, line;
    //   while (std::getline(file, line) && !line.empty()) {
    //     block1 += line + "\n";
    //   }
    //   while (std::getline(file, line)) {
    //     block2 += line + "\n";
    //   }
    //   Workflows workflows = parseWorkflows(block1);
    //   std::istringstream iss(block2);
    //   int total = 0;
    //   while (std::getline(iss, line)) {
    //     auto item = parseItem(line);
    //     std::cout << NL;
    //     for (const auto& [key, value] : item) {
    //       std::cout << "," << key << ":" << value;
    //     }
    //     if (accept(item, "in", workflows)) {
    //       total += std::accumulate(item.begin(), item.end(), 0, [](int sum, const auto& pair) {
    //         auto [ch, n] = pair; 
    //         return sum + n;
    //       });
    //     }
    //   }
    //   std::cout << NL << "hyperneutrino says: " << total;
    //   return total;
    // }

  }

  Result solve_for(Model& model) {
    Result result{};
    std::cout << NL << NL << "part1";
      /*
      The Elves of Gear Island are thankful for your help and send you on your way. 
      They even have a hang glider that someone stole from Desert Island; 
      since you're already going that direction, it would help them a lot 
      if you would use it to get down there and return it to them.

      As you reach the bottom of the relentless avalanche of machine parts, 
      you discover that they're already forming a formidable heap. 
      Don't worry, though - a group of Elves is already here organizing the parts, and they have a system.

      To start, each part is rated in each of four categories:

      x: Extremely cool looking
      m: Musical (it makes a noise when you hit it)
      a: Aerodynamic
      s: Shiny
      
      Then, each part is sent through a series of workflows that will ultimately accept or reject the part. 
      Each workflow has a name and contains a list of rules; 
      each rule specifies a condition and where to send the part if the condition is true. 
      The first rule that matches the part being considered is applied immediately, 
      and the part moves on to the destination described by the rule. 
      (The last rule in each workflow has no condition and always applies if reached.)

      Consider the workflow ex{x>10:one,m<20:two,a>30:R,A}. 
      
      This workflow is named ex and contains four rules. 
      If workflow ex were considering a specific part, it would perform the following steps in order:

      Rule "x>10:one": 
        If the part's x is more than 10, send the part to the workflow named one.
      Rule "m<20:two": 
        Otherwise, if the part's m is less than 20, send the part to the workflow named two.
      Rule "a>30:R": 
        Otherwise, if the part's a is more than 30, the part is immediately rejected (R).
      Rule "A": 
        Otherwise, because no other rules matched the part, the part is immediately accepted (A).
      
      If a part is sent to another workflow, 
      it immediately switches to the start of that workflow instead and never returns. 
      If a part is accepted (sent to A) or rejected (sent to R), the part immediately stops any further processing.

      The system works, but it's not keeping up with the torrent of weird metal shapes. 
      The Elves ask if you can help sort a few parts 
      and give you the list of workflows and some part ratings (your puzzle input). For example:

      px{a<2006:qkq,m>2090:A,rfg}
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
      {x=2127,m=1623,a=2188,s=1013}
      
      The workflows are listed first, followed by a blank line, 
      then the ratings of the parts the Elves would like you to sort. 
      All parts begin in the workflow named in. 
      In this example, the five listed parts go through the following workflows:

      {x=787,m=2655,a=1222,s=2876}: in -> qqz -> qs -> lnx -> A
      {x=1679,m=44,a=2067,s=496}: in -> px -> rfg -> gd -> R
      {x=2036,m=264,a=79,s=2244}: in -> qqz -> hdj -> pv -> A
      {x=2461,m=1339,a=466,s=291}: in -> px -> qkq -> crn -> R
      {x=2127,m=1623,a=2188,s=1013}: in -> px -> rfg -> A
      
      Ultimately, three parts are accepted. 
      Adding up the x, m, a, and s rating for each of the accepted parts 
      gives 7540 for the part with x=787, 
      4623 for the part with x=2036, 
      and 6951 for the part with x=2127. 
      
      Adding all of the ratings for all of the accepted parts gives the sum total of 19114.

      Sort through all of the parts you've been given; what do you get if you add together all of the rating numbers for all of the parts that ultimately get accepted?
      */    
    // result = hyperneutrino::main();
    auto const& [workflows, items] = model;
    for (auto const& item : items) {
      std::cout << NL;
      for (const auto& [key, value] : item) {
        std::cout << "," << key << ":" << value;
      }
      if (hyperneutrino::accept(item, "in", workflows)) {
        result += std::accumulate(item.begin(), item.end(), 0, [](int sum, const auto& pair) {
          auto [ch, n] = pair; 
          return sum + n;
        });
      }
    }
    return result; // 397134
  }
}

namespace part2 {
  using Range = std::pair<int, int>;
  using Ranges = std::map<char, Range>;

  // Based on https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day19p2.py
  // Thanks!
  namespace hyperneutrino {

    // Workflows parseWorkflows(const std::string& block) {
    //   Workflows workflows;
    //   std::istringstream iss(block);
    //   std::string line;
    //   while (std::getline(iss, line)) {
    //     std::cout << NL << "line:" << std::quoted(line) << std::flush;
    //     std::string name = line.substr(0, line.find("{"));
    //     std::string rest = line.substr(line.find("{") + 1, line.size() - 2);
    //     std::istringstream iss2(rest);
    //     std::string rule;
    //     std::vector<std::tuple<char, char, int, std::string>> rules;
    //     while (std::getline(iss2, rule, ',')) {
    //       char key = rule[0];
    //       char cmp = rule[1];
    //       if (auto at = rule.find(":"); at != std::string::npos) {
    //         auto n_str = rule.substr(2, at - 2);
    //         int n = std::stoi(n_str);
    //         std::string target = rule.substr(at + 1);
    //         rules.push_back({key, cmp, n, target});
    //         std::cout << NT << " key:" << key << " cmp:" << cmp << " n:" << n << " target:" << target << std::flush;
    //       }
    //       else {
    //         rules.push_back({key, cmp, 0, rule.substr(0,rule.size()-1)}); // fallback is the tail with '}' removed (size-1 length)
    //       }
    //     }
    //     std::string fallback = std::get<3>(rules.back());
    //     rules.pop_back();
    //     workflows[name] = {rules, fallback};
    //     std::cout << NT << " workflows[" << name << "]";
    //     for (const auto& [key, cmp, n, target] : rules) {
    //       std::cout << " if " << key;
    //       std::cout << " " << cmp;
    //       std::cout << " " << n;
    //       std::cout << " then " << std::quoted(target);
    //     }
    //     std:: cout << " fallback:" << fallback << std::flush;
    //   }
    //   return workflows;
    // }

    long long count(Workflows const& workflows, Ranges ranges, const std::string& name = "in") {
      std::cout << NL << "count(" << name << ")" << std::flush;
      if (name == "R") {
        return 0;
      }
      if (name == "A") {
        long long product = 1;
        for (const auto& [key, range] : ranges) {
          product *= range.second - range.first + 1;
        }
        return product;
      }

      const auto& [rules, fallback] = workflows.at(name);

      long long total = 0;

      for (const auto& [key, cmp, n, target] : rules) {
        std::cout << NL << "rule: " << key << " " << cmp << " " << n << " " << target << std::flush;
        auto [lo, hi] = ranges[key];
        Range T, F;
        if (cmp == '<') {
          T = {lo, n - 1};
          F = {n, hi};
        } else {
          T = {n + 1, hi};
          F = {lo, n};
        }
        if (T.first <= T.second) {
          Ranges copy = ranges;
          copy[key] = T;
          total += count(workflows,copy, target);
        }
        if (F.first <= F.second) {
          ranges[key] = F;
        } else {
          break;
        }
      }

      total += count(workflows,ranges, fallback);

      return total;
    }

    // Result main() {
    //   // std::ifstream file("example.txt");
    //   std::ifstream file("puzzle.txt");
    //   std::string block1, line;
    //   while (std::getline(file, line) && !line.empty()) {
    //     block1 += line + "\n";
    //   }
    //   Workflows workflows = parseWorkflows(block1);  

    //   Ranges ranges = {{'x', {1, 4000}}, {'m', {1, 4000}}, {'a', {1, 4000}}, {'s', {1, 4000}}};
    //   auto result = count(workflows,ranges);

    //   std::cout << NL << "hyperneutrino says: " << result;

    //   return result;
    // }

  }


  Result solve_for(Model& model) {
    Result result{};
    std::cout << NL << NL << "part2";
    /*
    Even with your help, the sorting process still isn't fast enough.

    One of the Elves comes up with a new plan: rather than sort parts individually through all of these workflows,
    maybe you can figure out in advance which combinations of ratings will be accepted or rejected.

    Each of the four ratings (x, m, a, s) can have an integer value 
    ranging from a minimum of 1 to a maximum of 4000.
    */

    Ranges ranges = {{'x', {1, 4000}}, {'m', {1, 4000}}, {'a', {1, 4000}}, {'s', {1, 4000}}};

   /*
    
    Of all possible distinct combinations of ratings, your job is to figure out which ones will be accepted.

    In the above example, there are 167409079868000 distinct combinations of ratings that will be accepted.

    Consider only your list of workflows; 
    the list of part ratings that the Elves wanted you to sort is no longer relevant. 
    
    How many distinct combinations of ratings will be accepted by the Elves' workflows?
    */
    // result = hyperneutrino::main();

    auto const& [workflows, items] = model;
    result = hyperneutrino::count(workflows,ranges);

    return result; // 127517902575337
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