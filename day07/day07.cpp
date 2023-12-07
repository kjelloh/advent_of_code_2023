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
#include <type_traits> // E.g., std::is_convertible

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int; // 16 bit int: ± 3.27 · 10^4, 32 bit int: ± 2.14 · 10^9, 64 bit int: ± 9.22 · 10^18
template <typename Int>
auto to_int(auto const& s) {
  if constexpr (std::is_convertible<Int, int>::value == true) {
    return std::stoi(std::string{s});
    }
}
char const* example = R"(32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483)";
using Hand = std::string;
using Bid = Integer;
struct Game {
  Hand hand{};
  Bid bid{};
};
using Model = std::vector<Game>;
template <class P = std::pair<std::string_view, std::string_view>>
auto to_pair(std::string_view const& text, auto const& delimiter) {
  std::cout << NL << "to_pair(" << std::quoted(text) << ")";
  auto mid = text.find(delimiter);
  return P(text.substr(0, mid), text.substr(mid + 1)); // pair-constructable required ;, i.e., two member intiliazier list)
}
auto parse(auto& in) {
  Model result{};
  auto to_game = [](auto const& p) {
    auto const& [first, second] = p;
    return Game{ std::string{first},to_int<Bid>(std::string{second}) };
    };
  std::string line{};
  while (std::getline(in, line)) {
    if (true) {
      // can we make a fancier parse?
      auto game = to_game(to_pair<>(line, ' '));
      std::cout << T << std::format("hand:{} bid:{}", game.hand, game.bid);
      result.push_back(game);
    }
    else {
      result.push_back({});
      auto mid = line.find(" ");
      result.back().hand = line.substr(0, mid);
      result.back().bid = std::stoi(line.substr(mid + 1));
      std::cout << NL << std::format("hand:{} bid:{}", result.back().hand, result.back().bid);
    }
  }
  return result;
}

using Result = Integer;
using Answers = std::vector<std::pair<std::string, Result>>;


namespace part1 {
  std::string const CARDS{"AKQJT98765432"};
  enum class RankType {
    undefined
    , unknown
  };
  auto to_rank(Hand const& hand) {
    /*
    *
    */
    return RankType::undefined;
   };

  auto to_rank(char card) {
    return CARDS.find(card);
  }
  auto card_rank_compare(Hand const& first, Hand const& second) {
    // If two hands have the same type, a second ordering rule takes effect.
    std::pair<int, int> rank{};
    for (int i = 0; i < first.size(); ++i) {
      rank = { to_rank(first[i]),to_rank(second[i]) };
      if (rank.first != rank.second) return (rank.first < rank.second);
    }
    std::cerr << NL << std::format("card_rank_compare({},{}) failed - Hands have equal ranking {}", first, second, rank.first);
  };

  Result solve_for(Model const& model) {
    std::cout << NL << "<part1>";
    Result result{};
    Model sorted = model;
    auto rank_compare = [](Hand const& first, Hand const& second) {
      std::pair<RankType,RankType> rank{ to_rank(first),to_rank(second) };
      // Hands are primarily ordered based on type;
      // If two hands have the same type, a second ordering rule takes effect.
      return (rank.first == rank.second) ? card_rank_compare(first,second) : rank.first < rank.second;
      };
    std::ranges::sort(sorted, rank_compare, [](Game const& game) {return game.hand;});
    std::cout << NL << "<sorted games>";
    for (auto const& game : sorted) {
      std::cout << NT << std::format("hand:{} rank:{}", game.hand, static_cast<int>(to_rank(game.hand)));
    }
    return result;
  }
}

namespace part2 {
  Result solve_for(Model const& model) {
    Result result{};
    return result;
  }
}

int main(int argc, char* argv[])
{
  Answers answers{};
  std::cout << NL << "argc : " << argc;
  if (argc == 1) {
    std::cout << NL << "no data file provided ==> WIll use hard coded example input";
    std::istringstream in{example};
    auto model = parse(in);
    answers.push_back({ "Example",part1::solve_for(model)});
  }
  else for (int i = 0; i < argc; ++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
    if (i > 0) {
      std::ifstream in{ argv[i] };
      if (in) {
        auto model = parse(in);
        if (in) answers.push_back({ argv[i],part1::solve_for(model) });
      }
      else answers.push_back({ std::string{"Failed to open file "} + argv[i],-1 });
    }
  }
  std::cout << "\n\n------------ REPORT----------------";
  for (auto const& answer : answers) {
    if (answer.second != 0) std::cout << "\nanswer[" << answer.first << "] " << answer.second;
    else std::cout << "\nanswer[" << answer.first << "] " << " NO OPERATION ";
  }
  std::cout << "\n\n";
  return 0;
}