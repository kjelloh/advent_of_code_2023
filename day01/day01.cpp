//
//  main.cpp
//  day01
//
//  Created by Kjell-Olov Högdal on 2023-12-01.
//

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <utility>
#include <optional>
#include <map>
#include <algorithm>
#include <string_view>
#include <fstream>
#include <cctype>
#include <numeric>

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

namespace part2 {
  int to_solution(std::string file_name) {
    std::cout << NL << "part2::to_solution(" << file_name << ")";

    std::ifstream file(file_name);
    std::string line;
  
    std::vector<std::string> numbers  {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    std::vector<int> calibration_values;
    while(std::getline(file, line)) {
      std::cout << NT << std::quoted(line);
      std::vector<int> digits;
      for (int i = 0; i < line.size(); i++) {
          const auto c = line[i];
          if (c >= '0' && c <= '9') {
              digits.push_back(c - '0');
          }
          for (int j = 0; j < numbers.size(); j++) {
              const auto& n = numbers[j];
              if (line.size() >= i + n.size()) {
                  if (line.substr(i, n.size()) == n) {
                      digits.push_back(j+1);
                  }
              }
          }
      }
      std::cout << NT << "digits:" << digits.size() << " first:" << digits[0] << " last:" << digits.back();
      calibration_values.push_back(digits[0] * 10 + digits.back());  
    }

    const auto sum = std::accumulate(std::begin(calibration_values), std::end(calibration_values), 0);
    std::cout << NL << NL << "part2:" << sum << std::endl;
    return sum; // 53515
  }
}

int main(int argc, const char * argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
    return 1;
  }
  else {
    std::cout << "Input file: " << argv[1] << std::endl;
  }

  std::ifstream file(argv[1]);
  std::string line;
  int t = 0;

  while (std::getline(file, line)) {
    std::string digits;
    for (char ch : line) {
      if (std::isdigit(ch)) {
        digits.push_back(ch);
      }
    }
    if (!digits.empty()) {
      t += 10*(digits[0] - '0') + (digits[digits.size() - 1] - '0');
    }
  }

  std::cout <<  "part1:" << t << std::endl; // 54388
  part2::to_solution(argv[1]); // 53515

  return 0;
}