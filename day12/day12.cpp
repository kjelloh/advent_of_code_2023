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
#include <tuple>
#include <unordered_map>

char const* example = R"(???.### 1,1,3
.??..??...?##. 1,1,3
?#?#?#?#?#?#?#? 1,3,1,6
????.#...#... 4,1,1
????.######..#####. 1,6,5
?###???????? 3,2,1)";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int64_t; // 16 bit int: � 3.27 � 10^4, 32 bit int: � 2.14 � 10^9, 64 bit int: � 9.22 � 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
struct Solution {
  Answers part1{};
  Answers part2{};
};

struct Entry {
  std::string cfg{};
  std::vector<int> nums{};  
};

using Model = std::vector<Entry>;

Model parse(auto& file) {
    Model result{};
    for (std::string line; std::getline(file, line);) {
        auto split_pos = line.find(' ');
        std::string cfg = line.substr(0, split_pos);
        std::string nums_str = line.substr(split_pos + 1);
        std::vector<int> nums;
        size_t pos = 0;
        while ((pos = nums_str.find(',')) != std::string::npos) {
            nums.push_back(std::stoi(nums_str.substr(0, pos)));
            nums_str.erase(0, pos + 1);
        }
        nums.push_back(std::stoi(nums_str));
        result.push_back({ cfg,nums }); 
    }
    return result;
}

void print_model(Model const& model) {
    for (auto const& entry : model) {
        std::cout << entry.cfg << " : ";
        for (auto const& num : entry.nums) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}

namespace part1 {

  int count(const std::string& cfg, std::vector<int> nums, bool flag = false) {
      if (cfg.empty()) {
          return std::accumulate(nums.begin(), nums.end(), 0) == 0 ? 1 : 0;
      }

      if (std::accumulate(nums.begin(), nums.end(), 0) == 0) {
          return cfg.find('#') != std::string::npos ? 0 : 1;
      }

      if (cfg[0] == '#') {
          if (flag && nums[0] == 0) {
              return 0;
          }
          --nums[0];
          return count(cfg.substr(1), nums, true);
      }

      if (cfg[0] == '.') {
          if (flag && nums[0] > 0) {
              return 0;
          }
          if (nums[0] == 0) {
              nums.erase(nums.begin());
          }
          return count(cfg.substr(1), nums, false);
      }

      if (flag) {
          if (nums[0] == 0) {
              nums.erase(nums.begin());
              return count(cfg.substr(1), nums, false);
          }
          --nums[0];
          return count(cfg.substr(1), nums, true);
      }

      std::vector<int> nums_copy(nums);
      if (nums_copy[0] > 0) {
          --nums_copy[0];
      }
      return count(cfg.substr(1), nums, false) + count(cfg.substr(1), nums_copy, true);
  }

  int main() {
      std::ifstream file("input.txt");
      int total = 0;

      for (std::string line; std::getline(file, line);) {
          auto split_pos = line.find(' ');
          std::string cfg = line.substr(0, split_pos);
          std::string nums_str = line.substr(split_pos + 1);
          std::vector<int> nums;
          size_t pos = 0;
          while ((pos = nums_str.find(',')) != std::string::npos) {
              nums.push_back(std::stoi(nums_str.substr(0, pos)));
              nums_str.erase(0, pos + 1);
          }
          nums.push_back(std::stoi(nums_str));

          total += count(cfg, nums);
      }

      std::cout << total << std::endl;

      return 0;
  }
  Result solve_for(Model& model) {
      Result result{};
      /*
      In the giant field just outside, the springs are arranged into rows. 
      For each row, the condition records show every spring and whether it is operational (.) or damaged (#). 
      This is the part of the condition records that is itself damaged; for some springs, 
      it is simply unknown (?) whether the spring is operational or damaged.

      However, the engineer that produced the condition records also duplicated some of this information in a different format! 
      After the list of springs for a given row, the size of each contiguous group of damaged springs is listed in the order 
      those groups appear in the row. This list always accounts for every damaged spring, 
      and each number is the entire size of its contiguous group 
      (that is, groups are always separated by at least one operational spring: #### would always be 4, never 2,2).

      So, condition records with no unknown spring conditions might look like this:

      #.#.### 1,1,3
      .#...#....###. 1,1,3
      .#.###.#.###### 1,3,1,6
      ####.#...#... 4,1,1
      #....######..#####. 1,6,5
      .###.##....# 3,2,1

      However, the condition records are partially damaged; some of the springs' conditions are actually unknown (?). For example:
      */

      char const* example = R"(???.### 1,1,3
      .??..??...?##. 1,1,3
      ?#?#?#?#?#?#?#? 1,3,1,6
      ????.#...#... 4,1,1
      ????.######..#####. 1,6,5
      ?###???????? 3,2,1)"; 

      /*

      Equipped with this information, it is your job to figure out how many different arrangements of operational and broken springs fit the given criteria in each row.

      In the first line (???.### 1,1,3), there is exactly one way separate groups of one, one, and three broken springs (in that order) 
      can appear in that row: the first three unknown springs must be broken, then operational, then broken (#.#), making the whole row #.#.###.

      The second line is more interesting: .??..??...?##. 1,1,3 could be a total of four different arrangements. 
      The last ? must always be broken (to satisfy the final contiguous group of three broken springs), 
      and each ?? must hide exactly one of the two broken springs. 
      (Neither ?? could be both broken springs or they would form a single contiguous group of two; if that were true, 
      the numbers afterward would have been 2,3 instead.) Since each ?? can either be #. or .#, there are four possible arrangements of springs.

      The last line is actually consistent with ten different arrangements! Because the first number is 3, the first and second ? must both be . (if either were #, the first number would have to be 4 or higher). However, the remaining run of unknown spring conditions have many different ways they could hold groups of two and one broken springs:

      ?###???????? 3,2,1
      .###.##.#...
      .###.##..#..
      .###.##...#.
      .###.##....#
      .###..##.#..
      .###..##..#.
      .###..##...#
      .###...##.#.
      .###...##..#
      .###....##.#
      In this example, the number of possible arrangements for each row is:

      ???.### 1,1,3 - 1 arrangement
      .??..??...?##. 1,1,3 - 4 arrangements
      ?#?#?#?#?#?#?#? 1,3,1,6 - 1 arrangement
      ????.#...#... 4,1,1 - 1 arrangement
      ????.######..#####. 1,6,5 - 4 arrangements
      ?###???????? 3,2,1 - 10 arrangements
      Adding all of the possible arrangement counts together produces a total of 21 arrangements.

      For each row, count all of the different arrangements of operational and broken springs that meet the given criteria. What is the sum of those counts?

      */
    for (auto const& entry : model) {
        result += count(entry.cfg, entry.nums);
    }
    return result;
  }
}

namespace part2 {

  Model parse(std::string file_name) {
      Model result{};
      std::ifstream file{file_name};
      for (std::string line; std::getline(file, line);) {
          std::cout << NL << "parsing " << std::quoted(line) << std::endl;
          auto split_pos = line.find(' ');
          std::string cfg = line.substr(0, split_pos);
          std::string nums_str = line.substr(split_pos + 1);
          std::vector<int> nums;
          size_t pos = 0;
          while ((pos = nums_str.find(',')) != std::string::npos) {
              nums.push_back(std::stoi(nums_str.substr(0, pos)));
              nums_str.erase(0, pos + 1);
          }
          nums.push_back(std::stoi(nums_str));

          cfg = std::string("?").append(cfg).append("?");
          nums.insert(nums.begin(), 1);
          nums.push_back(1);
          result.push_back({ cfg,nums });
      }      
      return result;
  }

  using Key = std::tuple<std::string, std::vector<int>>;
  struct KeyHash {
      std::size_t operator()(const Key& k) const {
          std::size_t h1 = std::hash<std::string>{}(std::get<0>(k));
          std::size_t h2 = std::accumulate(std::get<1>(k).begin(), std::get<1>(k).end(), 0);
          return h1 ^ (h2 << 1);
      }
  };

  std::unordered_map<Key, int, KeyHash> cache;

  int count(const std::string& cfg, std::vector<int> nums) {
      if (cfg.empty()) {
          return nums.empty() ? 1 : 0;
      }

      if (nums.empty()) {
          return cfg.find('#') != std::string::npos ? 0 : 1;
      }

      Key key = std::make_tuple(cfg, nums);
      if (cache.find(key) != cache.end()) {
          return cache[key];
      }

      int result = 0;

      if (cfg[0] == '.' || cfg[0] == '?') {
          result += count(cfg.substr(1), nums);
      }

      if (cfg[0] == '#' || cfg[0] == '?') {
          if (nums[0] <= cfg.size() && cfg.substr(0, nums[0]).find('.') == std::string::npos && (nums[0] == cfg.size() || cfg[nums[0]] != '#')) {
              nums.erase(nums.begin());
              result += count(cfg.substr(nums[0] + 1), nums);
          }
      }

      cache[key] = result;
      return result;
  }
  
  Result solve_for(Model const& model) {
    std::cout << NL << "part 2 model size : " << model.size();
    Result result{};
    for (auto const& entry : model) {
      std::cout << NL << "entry";
      result += count(entry.cfg, entry.nums);
    }
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
    print_model(model);
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
        part2_answer = { argv[i],part2::solve_for(part2::parse(argv[i])) };
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