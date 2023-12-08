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

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = std::int64_t; // 16 bit int: ± 3.27 · 10^4, 32 bit int: ± 2.14 · 10^9, 64 bit int: ± 9.22 · 10^18
using Result = Integer;
using Answers = std::vector<std::pair<std::string,Result>>;

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
  Result solve_for(auto& in) {
      Result result{};
      auto data_model = parse(in);
      return result;
  }
}

namespace part2 {
  Result solve_for(auto& in) {
      Result result{};
      auto data_model = parse(in);
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  std::cout << NL << "argc : " << argc;
  for (int i=0;i<argc;++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
    if (i>0) {
      std::ifstream in{argv[i]};
      if (in) answers.push_back({argv[i],part1::solve_for(in)});
      else answers.push_back({std::string{"Failed to open file "} + argv[i],-1});
    }
  }
  std::cout << "\n\n------------ REPORT----------------";
  for (auto const& answer : answers) {
    if (answer.second!=0) std::cout << "\nanswer[" << answer.first << "] " << answer.second;
    else std::cout << "\nanswer[" << answer.first << "] " << " NO OPERATION ";
  }
  std::cout << "\n\n";
  return 0;
}