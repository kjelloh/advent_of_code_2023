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

int main(int argc, const char * argv[]) {
    auto raw_0 = R"(1abc2
pqr3stu8vwx
a1b2c3d4e5f
treb7uchet)";
    auto in = std::istringstream(raw_0);
    std::string entry{};
    while (std::getline(in,entry)) {
        using Digit = char;
        std::vector<Digit> digits{};
        for (auto const& ch : entry) {
            if (std::isdigit(ch)) {
                digits.push_back(ch);
            }
        }
        if (digits.size() != 2) {
            std::cerr << "\nERROR - Found " << digits.size() << " digits in input " << std::quoted(entry);
        }
        else {
            std::cout << "\n " << std::quoted(entry) << " => " << "[" << digits[0] << "," << digits[1] << "]";
        }
    }
    return 0;
}
