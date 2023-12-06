// This project is a study on parsing the input of advent of code 2023 puzzles
// created in visual studio 2022 professional

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

namespace coroutes {
  // How can we parse with coroutines?
}

namespace ranges {
  // How can we parse with ranges?
}

namespace streams {
  // how can we parse with streams
}


template <int DAY>
class Parser {
public:
  char const* input = R"()";
  using Model = int;
  auto parse(auto& in) {
    Model result{};
    std::cerr << NL << "No parser defined for DAY " << DAY;
    return result;
  }
  auto parse() {
    std::istringstream in{ input };
    return parse(in);
  }
};

template <>
class Parser<1> {
public:
  using Answer = int;
  using Digit = char;
  using Entry = std::vector<Digit>;
  using Model = std::vector<Entry>;
  char const* input = R"(char const* example = R"(two1nine
eightwothree
abcone2threexyz
xtwone3four
4nineeightseven2
zoneight234
7pqrstsixteen)";
  auto parse() {
    std::istringstream in{input};
    return parse(in);
  }

  auto parse(auto& in) {
    Model result{};
    std::cout << NL << "Parse advent of code 2023 day02";
    std::string entry{};
    Answer answer{};
    auto to_digit = [](std::string_view const& sv) ->std::optional<Digit> {
      std::optional<Digit> result{};
      std::map<std::string, Digit> digit_map{
          {"one",'1'}
          ,{"two",'2'}
          ,{"three",'3'}
          ,{"four",'4'}
          ,{"five",'5'}
          ,{"six",'6'}
          ,{"seven",'7'}
          ,{"eight",'8'}
          ,{"nine",'9'}
      };
      auto iter = std::find_if(digit_map.begin(), digit_map.end(), [&sv](auto const& entry) {
        return sv.starts_with(entry.first);
        });
      if (iter != digit_map.end()) {
        result = iter->second;
      }
      return result;
      };

    while (std::getline(in, entry) and (entry.size() > 0)) {
      std::cout << "\n" << std::quoted(entry);
      std::vector<Digit> digits{};
      for (int i=0;i<entry.size();++i) {
        auto ch = entry[i];
        if (std::isdigit(ch)) {
          std::cout << NT << "ch '" << ch << "' is digit " << ch;
          digits.push_back(ch);
        }
        else {
          std::string_view at_i{entry.begin() + i,entry.end()};
          if (auto digit = to_digit(at_i)) {
            std::cout << NT << std::quoted(at_i) << " is digit " << *digit;
            digits.push_back(*digit);
          }
        }
      }
    }
    return result;
  }

};

template <>
class Parser<2> {
private:

  using IndexedToken = std::pair<int, std::string>;

  std::experimental::generator<IndexedToken> lines(auto& in)
  {
    std::string line{};
    int index{};
    while (std::getline(in, line)) {
      co_yield{ index,line };
      ++index;
    }
  }

  std::experimental::generator<IndexedToken> tokens(auto& token, char delimiter) {
    std::string split{};
    int index{};
    for (char ch : token) {
      if (ch == delimiter and split.size()>0) {
        co_yield{ index,split };
        ++index;
        split.clear();
      }
      else {
        if (ch != delimiter) split += ch;
      }
    }
    co_yield{ index,split };
  }

  std::optional<int> to_number(auto const& s) {
    std::optional<int> result{};
    if (!s.empty() and std::ranges::all_of(s, [](char ch) {return std::isdigit(ch); })) {
      result = std::stoi(s);
    }
    return result;
  }

public:
  char const* input = R"(Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green)";
  using Model = std::vector<std::pair<int, std::vector<std::vector<std::pair<int, std::string>>>>>;
  auto parse(auto& in) {
    Model result{};
    std::string indent{ NL };
    for (auto const& token : lines(in)) {
      std::cout << indent << token.first << " : " << std::quoted(token.second);
      indent += T;
      for (auto const& token : tokens(token.second, ':')) {
        std::cout << indent << token.first << " : " << std::quoted(token.second);
        indent += '\t';
        switch (token.first) {
        case 0: {
          int game_no{};
          indent += '\t';
          for (auto const& token : tokens(token.second, ' ')) {
            std::cout << indent << token.first << " : " << std::quoted(token.second);
            switch (token.first) {
            case 0: {/*skip*/} break;
            case 1: {
              if (auto number = to_number(token.second)) {
                game_no = *number;
                std::cout << indent << "==> Identified Game " << game_no;
                result.push_back({});
                result.back().first = game_no;
              }
              else {
                std::cerr << indent << "Failed to identify game number from " << std::quoted(token.second);
                exit(-1);
              }
            } break;
            default: {
              std::cerr << indent << " extraneous game metadata " << std::quoted(token.second);
              exit(-1);
            } break;
            }
          }
          indent.erase(indent.end() - 1);
        } break;
        case 1: {
          for (auto const& token : tokens(token.second, ';')) {
            result.back().second.push_back({}); // new outcome
            std::cout << indent << token.first << " : " << std::quoted(token.second);
            indent += '\t';
            for (auto const& token : tokens(token.second, ',')) {
              result.back().second.back().push_back({}); // new cube
              std::cout << indent << token.first << " : " << std::quoted(token.second);
              indent += '\t';
              int count{};
              for (auto const& token : tokens(token.second, ' ')) {
                std::cout << indent << token.first << " : " << std::quoted(token.second);
                if (auto number = to_number(token.second)) {
                  count = *number;
                }
                else if (!token.second.empty()) {
                  std::cout << indent << "==> " << count << " " << token.second << " cube(s)";
                  result.back().second.back().back().first = count;
                  result.back().second.back().back().second = token.second;
                }
              }
              indent.erase(indent.end() - 1);
            }
            indent.erase(indent.end() - 1);
          }
        } break;
        default: std::cerr << "\nFailed to parse line " << token.first << " i.e., " << std::quoted(token.second); exit(-1);  break;
        }
        indent.erase(indent.end() - 1);
      }
      indent.erase(indent.end() - 1);
    }

    return result;
  }
  auto parse() {
    std::istringstream in{ input };
    return parse(in);
  }
};

template <>
class Parser<4> {
private:
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
    if (split.size() > 0) co_yield{ split };
  }
public:
  char const* input = R"(Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11)";
  using Model = std::vector<std::pair<std::string, std::pair<std::set<std::string>, std::set<std::string>>>>;

  Model parse(auto& in) {
    Model result{};
    std::string line{};
    for (auto const& line : lines(in)) {
      result.push_back({});
      std::cout << NL << std::quoted(line);
      for (auto const& [index, token] : std::views::enumerate(tokens(line, ':'))) {
        switch (index) {
        case 0: {
          // card x
          std::cout << NT << std::quoted(token);
          result.back().first = token;
        } break;
        default: {
          std::set<int> numbers{};
          for (auto const& [index, token] : std::views::enumerate(tokens(token, '|'))) {
            std::cout << NT << T << std::quoted(token);
            for (auto const& number : tokens(token, ' ')) {
              if (number.size() > 0) {
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
  auto parse() {
    std::istringstream in{ input };
    return parse(in);
  }
};

template <>
class Parser<5> {
private:
  using Integer = std::int64_t; // 16 bit int: ± 3.27 · 10^4, 32 bit int: ± 2.14 · 10^9, 64 bit int: ± 9.22 · 10^18

  enum Category {
    undefined
    , seed
    , soil
    , fertilizer
    , water
    , light
    , temperature
    , humidity
    , location
    , unknown

  };
  struct Value {
    Category category{ undefined };
    Integer value{};
    bool operator<(Value const& other) const { return (category == other.category) ? value < other.value : category < other.category; }
  };
  using Values = std::set<Value>;
  struct MapEntry {
    Integer target_begin{};
    Integer source_begin{};
    Integer length{};
  };
  using MapEntries = std::vector<MapEntry>;
  class Map {
  public:
    Map(Category target_category, MapEntries entries)
      : target_category{ target_category }
      , entries{ entries } {}
    Map() = default;
    Values operator[](Integer source) const {
      Values result{};
      bool has_mapping{ false };
      for (auto const& entry : entries) {
        Integer offset = (source - entry.source_begin);
        if (offset >= 0 and offset < entry.length) {
          // defined mapping
          has_mapping = true;
          result.insert({ target_category,entry.target_begin + offset });
          // std::cout << NT << "entry.target_begin:" << entry.target_begin << "+" << offset << "=" << entry.target_begin + offset;
        }
      }
      if (!has_mapping) result.insert({ target_category,source });
      return result;
    }
  private:
    Category target_category;
    MapEntries entries{};
  };
  using Maps = std::map<Category, Map>;
  struct Model {
    std::vector<Integer> seeds;
    Maps maps;
  };

public:
  char const* input = R"(seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4)";
  Model parse(auto& in) {
    Model result{};
    std::string line{};
    while (std::getline(in, line)) {
      std::cout << NL << "parsing:" << std::quoted(line);
      std::pair<Category, Category> mapping{ undefined,undefined };
      if (line.starts_with("seeds:")) {
        auto s = line.substr(std::strlen("seeds:") + 1);
        std::cout << " as seeds " << std::quoted(s) << "-->";
        std::istringstream ss{ s };
        std::string n;
        while (ss >> n) {
          std::cout << " value:" << n;
          result.seeds.push_back(std::stoll(n));
        }
      }
      else if (line.starts_with("seed-to-soil map:")) mapping = { seed,soil };
      else if (line.starts_with("soil-to-fertilizer map:")) mapping = { soil,fertilizer };
      else if (line.starts_with("fertilizer-to-water map:")) mapping = { fertilizer,water };
      else if (line.starts_with("water-to-light map:")) mapping = { water,light };
      else if (line.starts_with("light-to-temperature map:")) mapping = { light,temperature };
      else if (line.starts_with("temperature-to-humidity map:")) mapping = { temperature,humidity };
      else if (line.starts_with("humidity-to-location map:")) mapping = { humidity,location };

      if (mapping.first != undefined and mapping.second != undefined) {
        MapEntries entries{};
        while (std::getline(in, line)) {
          std::cout << NT << "entry:" << line;
          if (line.empty()) break;
          MapEntry entry{};
          if (std::istringstream{ line } >> entry.target_begin >> entry.source_begin >> entry.length) {
            std::cout << " push";
            entries.push_back(entry);
          }
        }
        result.maps[mapping.first] = Map{ mapping.second,entries };
      }
    }
    return result;
  }
  auto parse() {
    std::istringstream in{ input };
    return parse(in);
  }
};

template <>
class Parser<6> {
public:
  char const* input = R"(Time:      7  15   30
Distance:  9  40  200)";
  using Integer = std::int64_t; // 16 bit int: ± 3.27 · 10^4, 32 bit int: ± 2.14 · 10^9, 64 bit int: ± 9.22 · 10^18
  using Result = Integer;

  using Part1Model = std::vector<std::pair<Integer, Integer>>;
  using Part2Model = std::pair<Integer, Integer>;
  struct Model {
    Part1Model p1{};
    Part2Model p2{};
  };

  Model parse(auto& in) {
    Model result{};
    std::vector<std::string> sp2{};
    for (int i = 0; i < 2; ++i) {
      sp2.push_back({});
      std::string line{};
      std::getline(in, line);
      std::cout << NL << "entry:" << std::quoted(line);
      std::istringstream ss{ line };
      std::string token{};
      for (int j = 0; ss >> token; ++j) {
        std::cout << NT << "token:" << std::quoted(token);
        if (j > 0) {
          sp2.back().append(token);
          if (i == 0) {
            std::cout << j - 1 << " first " << std::stoi(token);
            result.p1.push_back({});
            result.p1[j - 1].first = std::stoi(token);
          }
          else {
            std::cout << j - 1 << " second " << std::stoi(token);
            result.p1[j - 1].second = std::stoi(token);
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
  auto parse() {
    std::istringstream in{ input };
    return parse(in);
  }
};

int main(int argc, char* argv[])
{
  std::cout << NL << "Called with argc=" << argc << " arguments";
  for (int i = 0; i < argc; ++i) {
    std::cout << NT << "argv[" << i << "] : " << std::quoted(argv[i]);
  }
  auto model = Parser<6>{}.parse();
}
