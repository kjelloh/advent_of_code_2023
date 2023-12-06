#include <iostream>
#include <iomanip> // E.g., std::quoted
#include <string>
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
#include <cstring>
#include <functional>

auto const NL = "\n";
auto const NT = "\n\t";

using Integer = std::int64_t; // 16 bit int: ± 3.27 · 10^4, 32 bit int: ± 2.14 · 10^9, 64 bit int: ± 9.22 · 10^18
using Result = Integer;
using Answers = std::vector<std::pair<std::string,Result>>;

using Part1Model = std::vector<std::pair<Integer,Integer>>;
using Part2Model = std::pair<Integer,Integer>; 
struct Model {
  Part1Model p1{};
  Part2Model p2{};
};

Model parse(auto& in) {
    Model result{};
    std::vector<std::string> sp2{};
    for (int i=0;i<2;++i) {
      sp2.push_back({});
      std::string line{};
      std::getline(in,line);
      std::cout << NL << "entry:" << std::quoted(line);
      std::istringstream ss{line};
      std::string token{};
      for (int j=0;ss >> token;++j) {
        std::cout << NT << "token:" << std::quoted(token);
        if (j>0) {
          sp2.back().append(token);
          if (i==0) {
            std::cout << j-1 << " first " << std::stoi(token);
            result.p1.push_back({});
            result.p1[j-1].first = std::stoi(token);
          }
          else {
            std::cout << j-1 << " second " << std::stoi(token);
            result.p1[j-1].second = std::stoi(token);
          }
        }
      }      
    }
    std::cout << NL << "sp2[0]:" << sp2[0] << std::flush;
    std::cout << NL << "sp2[1]:" << sp2[1] << std::flush;

    result.p2.first = std::stoll(sp2[0]);
    result.p2.second = std::stoll(sp2[1]);
    return result;
}

namespace part1 {
  Result solve_for(Part1Model const& model) {
      // Result result{std::numeric_limits<Result>::max()};
      Result result{};
      std::vector<Result> candidate_count{};
      for (auto const& entry : model) {
        candidate_count.push_back({});
        for (Integer button_hold_time=0;button_hold_time <= entry.first;++button_hold_time) {
          auto race_time = entry.first-button_hold_time;
          std::cout << NT << "hold:" << button_hold_time << " ms race:" << race_time << " ms";
          auto speed = button_hold_time;
          auto distance = speed*race_time;
          std::cout << " speed:" << speed << " ==> distance:" << distance;
          if (distance>entry.second) {
            std::cout << " *CANDIDATE*";
            ++candidate_count.back();
          }
        }
      }
      result = std::accumulate(candidate_count.begin(),candidate_count.end(),Result{1},std::multiplies{});
      return result; // 1413720
  }
}

namespace part2 {
  Result solve_for(Part2Model const& model) {
      // Result result{std::numeric_limits<Result>::max()};
      Result result{};
      std::cout << NL << "part2 - race time:" << model.first << " record distance:" << model.second;
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
      if (in) {
        auto model = parse(in);
        // answers.push_back({argv[i],part1::solve_for(model.p1)}); // 57075758
        answers.push_back({argv[i],part2::solve_for(model.p2)});
      }
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