// Guard for incompatible compiler and c++ library, see https://en.cppreference.com/w/cpp/coroutine
#if (__cpp_impl_coroutine	< 201902L) // 	(C++20)	Coroutines (compiler support)
  // For gnu gcc support see https://gcc.gnu.org/wiki/cxx-coroutines
// #error insufficient coroutine support by compiler
#endif
#if (__cpp_lib_coroutine	< 201902L) // 	(C++20)	Coroutines (library support)
// For gnu/gcc libstdc++ support status see https://gcc.gnu.org/onlinedocs/libstdc++/manual/status.html#status.iso.2020
// #error insufficient coroutine support by c++ library
#else
#include <coroutine>
#endif
#if (__cpp_lib_generator < 202207L) //	(C++23)	std::generator: synchronous coroutine generator for ranges
// #error C++ library does not support std::generator
#else
#include <generator>
#endif


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
#include <experimental/generator> // supported by visual studio 2022 17.8.2 with project setting/compiler switch /std:c++latest
#include <format>

std::experimental::generator<std::string> lines(auto& in)
{
  std::string line{};
  while (std::getline(in, line)) {
    std::cout << "\nlines - " << line;
    co_yield{ line };
  }
}

std::experimental::generator<std::string> tokens(auto& token, char delimiter) {
  std::string split{};
  for (char ch : token) {
    if (ch == delimiter) {
      co_yield{ split };
      split.clear();
    }
    else {
      split += ch;
    }
  }
  if (split.size()>0) co_yield{ split };
}

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int; // int: 843 253 387 long int: 32 762 853 787 275 long long int: 32 762 853 787 275
using Result = Integer;
using Answers = std::vector<std::pair<std::string,Result>>;

using Model = std::vector<std::pair<std::string,std::pair<std::set<std::string>,std::set<std::string>>>>;

Model parse(auto& in) {
    Model result{};
    std::string line{};
    for (auto const& line : lines(in)) {
      result.push_back({});
      std::cout << NL << std::quoted(line);
      for (auto const& [index,token] : std::views::enumerate(tokens(line,':'))) {
        switch (index) {
        case 0: {
          // card x
          std::cout << NT << std::quoted(token);
          result.back().first = token;
        } break;
        default: {
          std::set<int> numbers{};
          for (auto const& [index,token] : std::views::enumerate(tokens(token, '|'))) {
            std::cout << NT << T << std::quoted(token);
            for (auto const& number : tokens(token, ' ')) {
              if (number.size()>0) {
                std::cout << NT << T << T << number;
                switch (index) {
                case 0: result.back().second.first.insert(number); break;
                case 1: result.back().second.second.insert(number); break;
                default: std::cerr << NL << "Failed to parse input - More than one set of numbers found in card entry";
                }
              }
            }
          }
        } break;
        }
      }
    }
    return result;
}

namespace part1 {
  Result solve_for(auto& in) {
      Result result{};
      auto model = parse(in);
        result = std::accumulate(model.begin(), model.end(), Result{ 0 }, [](auto acc, auto const& entry) {
          std::set<std::string> intersection{};
          std::set_intersection(
              entry.second.first.begin(), entry.second.first.end()
            , entry.second.second.begin(), entry.second.second.end()
            , std::inserter(intersection, intersection.begin()));
          std::cout << NL << T << "Intersection " << entry.first << " [";
          auto result = std::accumulate(intersection.begin(), intersection.end(), Result{ 0 }, [](auto acc, auto const& entry) {
            std::cout << " " << entry;
            acc = (acc==0)?1:acc*2;
            return acc;
            });
          std::cout << "] ==> Points:" << result;
          acc += result;
          return acc;
        });
      return result;
  }
}

namespace part2 {
  Result solve_for(auto& in) {
      Result result{};
      auto model = parse(in);
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