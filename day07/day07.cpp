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

using Integer = std::int_fast64_t; // 16 bit int: � 3.27 � 10^4, 32 bit int: � 2.14 � 10^9, 64 bit int: � 9.22 � 10^18
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
using Card = char;
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
      // Every hand is exactly one type. From strongest to weakest, they are:
      ,Five_of_a_kind // where all five cards have the same label: AAAAA
      ,Four_of_a_kind // where four cards have the same label and one card has a different label: AA8AA
      ,Full_house // where three cards have the same label, and the remaining two cards share a different label: 23332
      ,Three_of_a_kind // where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
      ,Two_pair // where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
      ,One_pair // where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
      ,High_card // where all cards' labels are distinct: 23456
    , unknown
  };
  auto to_counted_cards(Hand const& hand) {
    std::map<Card, Integer> result;
    for (auto const& card : hand) {
      ++result[card];
    }
    return result;
  }
  bool Five_of_a_kind(Hand const& hand) // where all five cards have the same label: AAAAA
  {
    return to_counted_cards(hand).size()==1;
  }
  bool Four_of_a_kind(Hand const& hand) // where four cards have the same label and one card has a different label: AA8AA
  {
    auto card_counts = to_counted_cards(hand);
    return std::any_of(hand.begin(), hand.end(), [&card_counts](Card card) {return card_counts[card] == 4; });
  }
  bool Full_house(Hand const& hand) // where three cards have the same label, and the remaining two cards share a different label: 23332
  {
    auto card_counts = to_counted_cards(hand);
    return std::any_of(hand.begin(), hand.end(), [&card_counts](Card card) {return card_counts[card] == 3; }) and card_counts.size()==2;
  }
  bool Three_of_a_kind(Hand const& hand) // where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
  {
    auto card_counts = to_counted_cards(hand);
    return std::any_of(hand.begin(), hand.end(), [&card_counts](Card card) {return card_counts[card] == 3; }) and card_counts.size() == 3;
  }
  bool Two_pair(Hand const& hand) // where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
  {
    auto card_counts = to_counted_cards(hand);
    std::map<Card, Integer> pair_counts{};
    for (auto const& card : hand) {
      if (card_counts[card] == 2) ++pair_counts[card];
    }
    return pair_counts.size() == 2;
  }
  bool One_pair(Hand const& hand) // where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
  {
    auto card_counts = to_counted_cards(hand);
    std::map<Card, Integer> pair_counts{};
    for (auto const& card : hand) {
      if (card_counts[card] == 2) ++pair_counts[card];
    }
    auto result =  (pair_counts.size() == 1) and card_counts.size()==4; // only one pair but three differect card types
    // std::cout << NL << std::format("One_Pair({}) = {}. pair_counts.size():{} card_counts.size():{}",hand,result, pair_counts.size(), card_counts.size());
    return result;
  }
  bool High_card(Hand const& hand) // where all cards' labels are distinct: 23456
  {
    auto card_counts = to_counted_cards(hand);
    return card_counts.size()==5;
  }

  auto to_type(Hand const& hand) {
    RankType result{ RankType::undefined };
    /*
      Every hand is exactly one type. From strongest to weakest, they are:

      Five of a kind, where all five cards have the same label: AAAAA
      Four of a kind, where four cards have the same label and one card has a different label: AA8AA
      Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
      Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
      Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
      One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
      High card, where all cards' labels are distinct: 23456
    */
    if (Five_of_a_kind(hand)) result = RankType::Five_of_a_kind;
    else if (Four_of_a_kind(hand)) result = RankType::Four_of_a_kind;
    else if (Full_house(hand)) result = RankType::Full_house;
    else if (Three_of_a_kind(hand)) result = RankType::Three_of_a_kind;
    else if (Two_pair(hand)) result = RankType::Two_pair;
    else if (One_pair(hand)) result = RankType::One_pair;
    else if (High_card(hand)) result = RankType::High_card;
    else {
      std::cerr << NL << "ERROR: Failed to recognise type of hand " << std::quoted(hand);
    }
    return (result!=RankType::undefined)?result:RankType::unknown;
   };

  auto to_weight(char card) {
    return CARDS.find(card);
  }
  auto hand_weight_compare(Hand const& first, Hand const& second) {
    // If two hands have the same type, a second ordering rule takes effect.
    std::pair<int, int> rank{};
    for (int i = 0; i < first.size(); ++i) {
      rank = { to_weight(first[i]),to_weight(second[i]) };
      if (rank.first != rank.second) return (rank.first > rank.second);
    }
    std::cerr << NL << std::format("card_rank_compare({},{}) failed - Hands have equal ranking {}", first, second, rank.first);
  };

  Result solve_for(Model const& model) {
    std::cout << NL << "<part1>";
    Result result{};
    Model sorted = model;
    auto hand_compare = [](Hand const& first, Hand const& second) {
      std::pair<RankType,RankType> type{ to_type(first),to_type(second) };
      // Hands are primarily ordered based on type;
      // If two hands have the same type, a second ordering rule takes effect.
      return (type.first == type.second) ? hand_weight_compare(first,second) : type.first > type.second;
      };
    std::ranges::sort(sorted, hand_compare, [](Game const& game) {return game.hand;});
    std::cout << NL << std::format("Games sorted on hand ranking and card ranking {}",CARDS);    
    for (auto const& [index,game] : sorted | std::views::enumerate) {
      auto rank = index + 1;
      auto winning = rank * game.bid;
      std::cout << NT << std::format("hand:{} type:{} rank:{} bid:{} = winning:{}", game.hand, static_cast<int>(to_type(game.hand)),rank,game.bid,winning);
      result += winning;
    }
    return result; // puzzle answer 251106089
  }
}

namespace part2 {
  std::string const CARDS{ "AKQT98765432J" };

  // generate_permutations based on sugestion from ChatGPT 3
  template <typename BidirIt>
  std::vector<std::string> generate_permutations(BidirIt first, BidirIt last, int length) {
    std::vector<std::string> result;

    // Base case: empty string
    if (length == 0) {
      result.push_back("");
      return result;
    }

    // Recursive case
    for (BidirIt it = first; it != last; ++it) {
      auto remaining_permutations = generate_permutations(first, last, length - 1);

      for (const auto& perm : remaining_permutations) {
        result.push_back(*it + perm);
      }
    }

    return result;
  }

  enum class RankType {
    undefined
    // Every hand is exactly one type. From strongest to weakest, they are:
    , Five_of_a_kind // where all five cards have the same label: AAAAA
    , Four_of_a_kind // where four cards have the same label and one card has a different label: AA8AA
    , Full_house // where three cards have the same label, and the remaining two cards share a different label: 23332
    , Three_of_a_kind // where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
    , Two_pair // where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
    , One_pair // where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
    , High_card // where all cards' labels are distinct: 23456
    , unknown
  };
  auto to_counted_cards(Hand const& hand) {
    std::map<Card, Integer> result;
    for (auto const& card : hand) {
      ++result[card];
    }
    return result;
  }
  bool Five_of_a_kind(Hand const& hand) // where all five cards have the same label: AAAAA
  {
    return to_counted_cards(hand).size() == 1;
  }
  bool Four_of_a_kind(Hand const& hand) // where four cards have the same label and one card has a different label: AA8AA
  {
    auto card_counts = to_counted_cards(hand);
    return std::any_of(hand.begin(), hand.end(), [&card_counts](Card card) {return card_counts[card] == 4; });
  }
  bool Full_house(Hand const& hand) // where three cards have the same label, and the remaining two cards share a different label: 23332
  {
    auto card_counts = to_counted_cards(hand);
    return std::any_of(hand.begin(), hand.end(), [&card_counts](Card card) {return card_counts[card] == 3; }) and card_counts.size() == 2;
  }
  bool Three_of_a_kind(Hand const& hand) // where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
  {
    auto card_counts = to_counted_cards(hand);
    return std::any_of(hand.begin(), hand.end(), [&card_counts](Card card) {return card_counts[card] == 3; }) and card_counts.size() == 3;
  }
  bool Two_pair(Hand const& hand) // where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
  {
    auto card_counts = to_counted_cards(hand);
    std::map<Card, Integer> pair_counts{};
    for (auto const& card : hand) {
      if (card_counts[card] == 2) ++pair_counts[card];
    }
    return pair_counts.size() == 2;
  }
  bool One_pair(Hand const& hand) // where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
  {
    auto card_counts = to_counted_cards(hand);
    std::map<Card, Integer> pair_counts{};
    for (auto const& card : hand) {
      if (card_counts[card] == 2) ++pair_counts[card];
    }
    auto result = (pair_counts.size() == 1) and card_counts.size() == 4; // only one pair but three differect card types
    // std::cout << NL << std::format("One_Pair({}) = {}. pair_counts.size():{} card_counts.size():{}",hand,result, pair_counts.size(), card_counts.size());
    return result;
  }
  bool High_card(Hand const& hand) // where all cards' labels are distinct: 23456
  {
    auto card_counts = to_counted_cards(hand);
    return card_counts.size() == 5;
  }

  auto to_type(Hand const& hand) {
    RankType result{ RankType::undefined };
    /*
      Every hand is exactly one type. From strongest to weakest, they are:

      Five of a kind, where all five cards have the same label: AAAAA
      Four of a kind, where four cards have the same label and one card has a different label: AA8AA
      Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
      Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
      Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
      One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
      High card, where all cards' labels are distinct: 23456
    */
    if (Five_of_a_kind(hand)) result = RankType::Five_of_a_kind;
    else if (Four_of_a_kind(hand)) result = RankType::Four_of_a_kind;
    else if (Full_house(hand)) result = RankType::Full_house;
    else if (Three_of_a_kind(hand)) result = RankType::Three_of_a_kind;
    else if (Two_pair(hand)) result = RankType::Two_pair;
    else if (One_pair(hand)) result = RankType::One_pair;
    else if (High_card(hand)) result = RankType::High_card;
    else {
      std::cerr << NL << "ERROR: Failed to recognise type of hand " << std::quoted(hand);
    }
    result = (result != RankType::undefined) ? result : RankType::unknown;
    // std::cout << std::format(" to_type({}) = {}", hand, static_cast<int>(result));
    return result;
  };

  auto to_weight(char card) {
    return CARDS.find(card);
  }
  bool hand_weight_compare(Hand const& first, Hand const& second) {
    // true if first is less-than second
    // If two hands have the same type, a second ordering rule takes effect.
    std::pair<int, int> rank{};
    for (int i = 0; i < first.size(); ++i) {
      rank = { to_weight(first[i]),to_weight(second[i]) };
      if (rank.first != rank.second) return (rank.first > rank.second); // Note that enums increase in value with lower "weight"
    }
    return false; // equal is possible outcome when applying jokers
  };
  struct JokerHand {
    Hand given{};
    Hand best{};
  };
  bool hand_compare(JokerHand const& first, JokerHand const& second) {
    // return true if first is less-than second
    // For hands with jokers the hand type is based on the best possible hand
    std::pair<RankType, RankType> type{ to_type(first.best),to_type(second.best) };
    // Hands are primarily ordered based on type;
    // If two hands have the same type, a second ordering rule takes effect.
    // For hands with jokers the hand weight is still based on jokers being themselves
    return (type.first == type.second) ? hand_weight_compare(first.given, second.given) : type.first > type.second;
  };

  Hand to_best_hand(Hand const& hand) {
    Hand result{ hand };
    // search hand for jokers and the permutate the jokers to get the best hand possible
    std::vector<int> joker_indicies{};
    for (int i = 0; i < hand.size(); ++i) {
      if (hand[i] == 'J') joker_indicies.push_back(i);
    }
    if (joker_indicies.size()>0) {
      // std::cout << NL << std::format("Hand {} contains {} joker(s)",hand, joker_indicies.size());
      if (joker_indicies.size() == 5) {
        // Simply pick the known best hand!
        result = "AAAAA";
      }
      else if (joker_indicies.size() == 4) {
        // Simply make the hand into a five-of-a-kind = the best hand!
        Card non_joker{};
        for (auto card : result) {
          if (card != 'J') {
            non_joker = card;
            break;
          }
        }
        for (int i = 0; i < joker_indicies.size(); ++i) {
          result[joker_indicies[i]] = non_joker;
        }
      }
      else {
        auto permutations = generate_permutations(CARDS.begin(), CARDS.end(), joker_indicies.size());
        Hand candidate{ hand };
        // Apply the generated permutations to the hand
        for (const auto& perm : permutations) {
          // std::cout << NT << "Candiate Jokers:" << std::quoted(perm);
          for (int i = 0; i < perm.size(); ++i) {
            candidate[joker_indicies[i]] = perm[i];
          }
          // std::cout << " candidate hand:" << std::quoted(candidate);
          if (hand_compare(JokerHand{ hand,result }, JokerHand{ hand,candidate })) {
            // std::cout << " BETTER!";
            result = candidate;
          }
        }
      }
    }
    return result;
  }
  struct JokerGame {
    JokerHand hand{};
    Bid bid{};
  };
  Result solve_for(Model const& model) {
    std::cout << NL << "<part2>";
    Result result{};
    std::vector<JokerGame> sorted{};
    std::transform(model.begin(), model.end(), std::back_inserter(sorted), [](Game const& game) {
      return JokerGame{ JokerHand{game.hand,to_best_hand(game.hand)},game.bid};
      });
    std::ranges::sort(sorted, hand_compare, [](JokerGame const& game) {return game.hand; });
    std::cout << NL << std::format("Games sorted on hand ranking and card ranking {}", CARDS);
    for (auto const& [index, game] : sorted | std::views::enumerate) {
      auto rank = index + 1;
      auto winning = rank * game.bid;
      std::cout << NT << std::format("given:{} best:{} type:{} rank:{} bid:{} = winning:{}", game.hand.given,game.hand.best, static_cast<int>(to_type(game.hand.best)), rank, game.bid, winning);
      result += winning;
    }
    return result; // 249620106
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
    answers.push_back({ "Example",part2::solve_for(model) });
  }
  else for (int i = 0; i < argc; ++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
    if (i > 0) {
      std::ifstream in{ argv[i] };
      if (in) {
        auto model = parse(in);
        answers.push_back({ argv[i],part1::solve_for(model) });
        answers.push_back({ argv[i],part2::solve_for(model) });
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