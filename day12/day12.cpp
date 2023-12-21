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
#include <span>

char const* example = R"(???.### 1,1,3
.??..??...?##. 1,1,3
?#?#?#?#?#?#?#? 1,3,1,6
????.#...#... 4,1,1
????.######..#####. 1,6,5
?###???????? 3,2,1)";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int64_t; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22 x 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
using Solution = std::map<int, Answers>; // Puzzle part -> Answers

using Nums = std::vector<int>;
struct Entry {
  std::string cfg{};
  Nums nums{};
  bool operator==(const Entry& other) const {
    return cfg == other.cfg && nums == other.nums;
  }
};

using Model = std::vector<Entry>;

Model parse(auto& in) {
    Model result{};
    for (std::string line; std::getline(in, line);) {
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

void print_model(Model const& model) {
  std::cout << NL << "model";
    for (auto const& entry : model) {
        std::cout << NT << entry.cfg << " : ";
        for (auto const& num : entry.nums) {
            std::cout << num << " ";
        }
    }
}

namespace part1 {

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

  Result solve_for(Model const& model,auto args) {
      Result result{};
    for (auto const& entry : model) {
        result += count(entry.cfg, entry.nums);
    }
    return result;
  }
}

namespace part2 {

  Model to_expanded(Model const& model) {
    Model result{};
    for (auto const& [cfg, nums] : model) {
      Entry expanded_entry{};
      auto& [expanded_cfg, expanded_nums] = expanded_entry;
      for (int i=0;i<5;++i) {
        if (i>0) expanded_cfg += '?';
        expanded_cfg += cfg;
        expanded_nums.insert(expanded_nums.end(),nums.begin(),nums.end());
      }
      result.push_back(expanded_entry);
    }
    return result;
  }

  struct EntryHash {
    std::size_t operator()(const Entry& entry) const {
      std::size_t hash = std::hash<std::string>{}(entry.cfg);
      for (const auto& num : entry.nums) {
        hash ^= std::hash<int>{}(num) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
      }
      return hash;
    }
  };
  using Cache = std::unordered_map<Entry, Result, EntryHash>;
  Result to_conf_count(std::span<char const> cfg, std::span<int const> nums,Cache& cache) {

    std::cout << NL << "cfg : " << std::string{cfg.begin(),cfg.end()} << " : ";
    for (auto const& num : nums) {
      std::cout << num << " ";
    }

    if (cfg.empty()) {
      return nums.empty() ? 1 : 0;
    }

    if (nums.empty()) {
      return std::find(cfg.begin(),cfg.end(),'#') != cfg.end() ? 0 : 1;
    }

    Entry key{std::string{cfg.begin(),cfg.end()},Nums{nums.begin(),nums.end()}};

    if (cache.contains(key)) {
      return cache[key];
    }

    Result result{};

    if (cfg[0] == '.' || cfg[0] == '?') {
      result += to_conf_count(cfg.subspan(1),nums,cache);
    }
        
    if (cfg[0] == '#' || cfg[0] == '?') {
      if (nums[0] <= cfg.size() && std::find(cfg.begin(),cfg.begin()+nums[0],'.') == cfg.begin()+nums[0] && (nums[0] == cfg.size() || cfg[nums[0]] != '#')) {
        auto cfg_next = std::min(cfg.size(),nums[0] + std::size_t{1});
        result += to_conf_count(cfg.subspan(cfg_next),nums.subspan(1),cache);
      }
      else {
        std::cout << NL << "NO MATCH";
      }
    }
    cache[key] = result;
    std::cout << NT << " --> " << result;
    return result;
  }
  
  Result solve_for(Model const& model,auto args) {
    Result result{};
    std::cout << NL << "part2::solve_for";
    print_model(model);
    auto expanded_model = to_expanded(model);
    print_model(expanded_model);
    for (auto const& entry : expanded_model) {
      Cache cache{};
      result += to_conf_count(entry.cfg,entry.nums,cache);
    }
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
  // day00 x y
  std::tuple<int,std::string> args{1,"example.txt"};
  auto& [part,file] = args;
  if (argc > 1 ) {
    part = std::stoi(argv[1]);
    if (argc > 2) {
      file = argv[2];
    }
  }
  std::cout << NL << "Part : " << part << " file : " << file;
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