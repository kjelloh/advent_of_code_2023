// day02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <experimental/generator>
#include <iostream>
#include <ranges>
#include <sstream>
#include <iomanip>
#include <optional>
#include <algorithm>
#include <cctype>
#include <utility>

auto example = R"(Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue
Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red
Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red
Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green)";

std::experimental::generator<char> letters(char first)
{
  for (;; co_yield first++);
}

using IndexedToken = std::pair<int, std::string>;

std::experimental::generator<IndexedToken> lines(auto& in)
{
    std::string line{};
    int index{};
    while (std::getline(in,line)) {
        co_yield{ index,line };
        ++index;
    }
}

std::experimental::generator<IndexedToken> tokens(auto& token,char delimiter) {
    std::string split{};
    int index{};
    for (char ch : token) {
        if (ch == delimiter) {
            co_yield {index,split};
            ++index;
            split.clear();
        }
        else {
            split += ch;
        }
    }
    co_yield{ index,split };
}

std::optional<int> to_number(auto const& s) {
    std::optional<int> result{};
    if (!s.empty() and std::ranges::all_of(s, [](char ch) {return std::isdigit(ch);})) {
        result = std::stoi(s);
    }
    return result;
}


int main() {
  for (const char ch : letters('a') | std::views::take(26)) std::cout << ch << ' ';
  std::istringstream in{ example };
  std::string indent{ "\n" };
  for (auto const& token : lines(in)) {
      std::cout << indent << token.first << " : " << std::quoted(token.second) ;
      indent += '\t';
      for (auto const& token : tokens(token.second, ':')) {
          std::cout << indent << token.first << " : " << std::quoted(token.second);
          indent += '\t';
          switch (token.first) {
          case 0: {
              int count{};
              indent += '\t';
              for (auto const& token : tokens(token.second, ' ')) {
                  std::cout << indent << token.first << " : " << std::quoted(token.second);
                  switch (token.first) {
                  case 0: {/*skip*/} break;
                  case 1: {
                      if (auto number = to_number(token.second)) {
                          count = *number;
                          std::cout << indent << "==> Identified Game " << count;
                      }
                      else {
                          std::cerr << indent << "Failed to identify game index from " << std::quoted(token.second);
                      }
                  } break;
                  default: {
                      std::cerr << indent << " extraneous game metadata " << std::quoted(token.second);
                  } break;
                  }
              }
              indent.erase(indent.end() - 1);
          } break;
          case 1: {
              for (auto const& token : tokens(token.second, ';')) {
                  std::cout << indent << token.first << " : " << std::quoted(token.second);
                  indent += '\t';
                  for (auto const& token : tokens(token.second, ',')) {
                      std::cout << indent << token.first << " : " << std::quoted(token.second);
                      indent += '\t';
                      int count{};
                      for (auto const& token : tokens(token.second, ' ')) {
                          std::cout << indent << token.first << " : " << std::quoted(token.second);
                          if (auto number = to_number(token.second)) {
                              count = *number;
                          }
                          else if (!token.second.empty()) {
                              std::cout << indent << "==> " <<  count << " " << token.second << " cube(s)";
                          }
                      }
                      indent.erase(indent.end() - 1);
                  }
                  indent.erase(indent.end() - 1);
              }
          } break;
          default: std::cerr << "\nFailed to parse line " << token.first << " i.e., " << std::quoted(token.second); break;
          }
          indent.erase(indent.end() - 1);
      }
      indent.erase(indent.end() - 1);
  }
  std::cout << '\n';
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
