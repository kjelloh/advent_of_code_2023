// This project is a study on parsing the input of advent of code 2023 puzzles
// created in visual studio 2022 professional

#include <experimental/generator>
#include <iostream>
#include <ranges>
#include <sstream>
#include <iomanip>
#include <optional>
#include <algorithm>
#include <cctype>
#include <utility>
#include <map>
#include <vector>
#include <numeric>

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

template <int DAY>
class Parser {
public:
  auto parse() {
    using Model = int;
    Model result{};
    std::cerr << NL << "No parser defined for DAY " << DAY;
    return result;
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

  Answer solve() {
    Answer answer{-1};
    std::istringstream in{ input };
    auto model = parse(in);

    for (auto const& digits : model) {
      if (digits.size() > 0) {
        auto [first, last] = std::pair<Digit, Digit>({ digits[0],digits.back() });
        std::cout << "\n\t== " << "[" << first << "," << last << "]";
        Answer calibration_value = (first - '0') * 10 + (last - '0');
        std::cout << " calibration value " << calibration_value;
        answer += calibration_value;
      }
      else {
        std::cerr << "\nNo digits in entry ";
        break;
      }
    }
    return answer;
  }

};




int main(int argc, char* argv[])
{
  std::cout << NL << "Called with argc=" << argc << " arguments";
  for (int i = 0; i < argc; ++i) {
    std::cout << NT << "argv[" << i << "] : " << std::quoted(argv[i]);
  }
  auto model = Parser<1>{}.parse();
}
