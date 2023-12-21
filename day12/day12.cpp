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

using Nums = std::vector<int>;
struct Entry {
  std::string cfg{};
  Nums nums{};  
};

using Model = std::vector<Entry>;

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

  Model parse(std::string file_name) {
      Model result{};
      std::ifstream file{file_name};
      for (std::string line; std::getline(file, line);) {
          auto split_pos = line.find(' ');
          std::string cfg = line.substr(0, split_pos);
          std::string nums_str = line.substr(split_pos + 1);
          Nums nums;
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

  Integer count(const std::string& cfg, Nums nums, bool flag = false) {
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

      auto nums_copy = nums;
      if (nums_copy[0] > 0) {
          --nums_copy[0];
      }
      return count(cfg.substr(1), nums, false) + count(cfg.substr(1), nums_copy, true);
  }

  Result solve_for(Model const& model) {
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

  /*
  To unfold the records, on each row, replace the list of spring conditions with five copies of itself (separated by ?) 
  and replace the list of contiguous groups of damaged springs with five copies of itself (separated by ,).

  So, this row: ".# 1"

  Would become: ".#?.#?.#?.#?.# 1,1,1,1,1"

  entry:        "???.### 1,1,3"
  would become: "???.###????.###????.###????.###????.### 1,1,3,1,1,3,1,1,3,1,1,3,1,1,3"
  */
  Model parse(std::string file_name) {
      Model result{};
      std::ifstream file{file_name};
      for (std::string line; std::getline(file, line);) {
        std::istringstream iss(line);
        std::string cfg;
        std::string nums_str;
        iss >> cfg >> nums_str;
        std::cout << NL << "cfg : " << std::quoted(cfg) << " nums_str : " << std::quoted(nums_str) << std::flush; 

        Nums nums{};
        std::stringstream ss(nums_str);
        for (Integer i; ss >> i;) {
            nums.push_back(i);
            std::cout << NT << "nums += " << i << std::flush;
            if (ss.peek() == ',')
                ss.ignore();
        }

        // Repeat cfg and nums 5 times
        std::string cfg_repeated;
        for (Integer i = 0; i < 5; ++i) {
            cfg_repeated += cfg + "?";
            std::cout << NT << "cfg_repeated : " << cfg_repeated;
        }
        cfg_repeated.pop_back(); // '?' is a separator so remove the ending one

        Nums nums_repeated;
        for (int i = 0; i < 5; ++i) {
            nums_repeated.insert(nums_repeated.end(), nums.begin(), nums.end());
        }
        result.push_back({ cfg_repeated,nums_repeated });
      }
      return result;
  }

  using Key = std::tuple<std::string, std::vector<int>>;
  struct KeyHash {
    std::size_t operator()(const Key& k) const {
      std::size_t h1 = std::hash<std::string>{}(std::get<0>(k));
      std::size_t h2 = 0;
      for (const auto& num : std::get<1>(k)) {
        h2 ^= std::hash<int>{}(num) + 0x9e3779b9 + (h2 << 6) + (h2 >> 2);
      }
      return h1 ^ (h2 << 1);
    }
  };
  std::unordered_map<Key, Integer, KeyHash> cache;

  // recursively count the number of ways to make the cfg match the list of numbers
  // The line: "?###???????? 3,2,1"
  //  is actually consistent with ten different arrangements! 
  // Because the first number is 3, the first and second ? must both be . 
  // (if either were #, the first number would have to be 4 or higher). 
  // However, the remaining run of unknown spring conditions have many different ways 
  // they could hold groups of two and one broken springs: (10 ways to map '?' '#' and '.' to conform with 3,2 an 1 broken springs)
  // 
  // .###.##.#...
  // .###.##..#..
  // .###.##...#.
  // .###.##....#
  // .###..##.#..
  // .###..##..#.
  // .###..##...#
  // .###...##.#.
  // .###...##..#
  // .###....##.#  

  Integer count(const std::string& cfg, Nums nums) {
      std::cout << NL << "count(cfg:" << std::quoted(cfg) << ",nums:";
      for (auto const& num : nums) {
          std::cout << " " << num << std::flush;
      }
      // base case - no cfg and no nums is one arrangement (full match)
      if (cfg.empty()) {
          return nums.empty() ? 1 : 0;
      }

      // base case - No nums but also no '#' in cfg is one arrangement (full match)
      if (nums.empty()) {
          return cfg.find('#') != std::string::npos ? 0 : 1;
      }

      // base case - return cached result if available
      Key key = std::make_tuple(cfg, nums);
      if (cache.find(key) != cache.end()) {
          return cache[key];
      }

      Integer result = 0;

      // add count for cfg stripped of head '.' or '?'
      if (cfg[0] == '.' || cfg[0] == '?') {
          std::cout << " heads: " << cfg[0] << " " << nums[0] << std::flush;
          result += count(cfg.substr(1), nums);
      }

      // match num[0] number of '#' or '?' from cfg front and with those and front number removed (matched / consumed)
      if (cfg[0] == '#' || cfg[0] == '?') {
        if (nums[0] <= cfg.size() && cfg.substr(0, nums[0]).find('.') == std::string::npos && (nums[0] == cfg.size() || cfg[nums[0]] != '#')) {
            nums.erase(nums.begin());
            // clamp argument to substr so that it does not overrun if all of cfg is consumed
            result += count(cfg.substr(std::min(nums[0] + 1,static_cast<int>(cfg.size()))), nums);
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
      auto n = count(entry.cfg, entry.nums);
      std::cout << NL << n << " arrangements";
      result += n;
    }
    return result; //     18 661 118 985 too low
                   //    233 409 483 785 Also too low
                   // 31 964 627 866 633 Too high...
                   // Hm... I wonder if my cache does not work?
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
    part1_answer = { "Example",part1::solve_for(part1::parse("example.txt")) };
    part2_answer = { "Example",part2::solve_for(part2::parse("example.txt")) };
    solution.part1.push_back(part1_answer);
    solution.part2.push_back(part2_answer);
  }
  else for (int i = 0; i < argc; ++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
    if (i > 0) {
      std::ifstream in{ argv[i] };
      if (in) {
        part1_answer = { argv[i],part1::solve_for(part1::parse(argv[i])) };
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