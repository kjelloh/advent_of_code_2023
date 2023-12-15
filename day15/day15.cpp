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
// #include <experimental/generator> // supported by visual studio 2022 17.8.2 with project setting/compiler switch /std:c++latest
#include <format>
#include <optional>
#include <regex>


char const* example = R"(rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7)";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int64_t; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22  x 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
struct Solution {
  Answers part1{};
  Answers part2{};
};

using Model = std::vector<std::string>;

Model parse(auto& in) {
    Model result{};
    std::string line{};
    while (std::getline(in,line,',')) {
        result.push_back(line);
    }
    return result;
}

void print(Model const& model) {
    std::cout << NL << "Model:";
    for (auto const& line : model) {
        std::cout << NT << line;
    }
}

class Hasher {
  public:
    Hasher(std::string const& entry) : entry{entry} {}
    std::pair<int,Result> operator++() {
        // Determine the ASCII code for the current character of the string.
        // Increase the current value by the ASCII code you just determined.
        // Set the current value to itself multiplied by 17.
        // Set the current value to the remainder of dividing itself by 256.
        result += entry[index];
        result *= 17;
        result %= 256;
        ++index;
        return {index,result};
    }
    bool next() {
        return (index < entry.size());
    }
    Result operator()() {
        return result;
    }
  private:
    std::string entry;
    int index{0};
    Result result{0};
};

Result hash(std::string const& entry) {
    Result result{};
    // std::cout << NL << "entry: " << entry;
    Hasher hasher{entry};
    while (hasher.next()) {
        auto [index,temp] = ++hasher;
        // std::cout << NT << "index: " << index << " temp: " << temp;
    }
    return hasher();
}

namespace part1 {
  Result solve_for(Model& model) {
      Result result{};
      for (auto const& entry : model) {
          std::cout << NT << entry;
          result += hash(entry);
      }
      return result; // 501680
  }
}

namespace part2 {
  std::tuple<std::string, char, std::string> splitEntry(const std::string& entry) {
    auto pos = std::find_if(entry.begin(), entry.end(), [](char c) {
      return c == '-' || c == '=';
    });

    std::string label;
    char operation;
    std::string value;

    if (pos != entry.end()) {
      label = std::string(entry.begin(), pos);
      operation = *pos;
      value = std::string(pos + 1, entry.end()); // empty value for operation '-'
    }

    return std::make_tuple(label, operation, value);
  }
  Result solve_for(Model& model) {
      Result result{};
      /*
      --- Part Two ---

      You convince the reindeer to bring you the page; the page confirms that your HASH algorithm is working.

      The book goes on to describe a series of 256 boxes numbered 0 through 255. The boxes are arranged in a line starting from the point where light enters the facility.
      */
      struct Lens {
          std::string label{};
          int focal_length{0};
      };
      using Lenses = std::vector<Lens>;
      struct Box {
          Lenses lenses{};
      };
      using Boxes = std::vector<Box>;
      Boxes boxes(256);
     /*
      The boxes have holes that allow light to pass from one box to the next all the way down the line.

            +-----+  +-----+         +-----+
      Light | Box |  | Box |   ...   | Box |
      ----------------------------------------->
            |  0  |  |  1  |   ...   | 255 |
            +-----+  +-----+         +-----+
      Inside each box, there are several lens slots that will keep a lens correctly positioned to focus light passing through the box. 
      The side of each box has a panel that opens to allow you to insert or remove lenses as necessary.

      Along the wall running parallel to the boxes is a large library containing lenses organized by focal length ranging from 1 through 9. The reindeer also brings you a small handheld label printer.

      The book goes on to explain how to perform each step in the initialization sequence, 
      a process it calls the Holiday ASCII String Helper Manual Arrangement Procedure, or HASHMAP for short.

      Each step begins with a sequence of letters that indicate the label of the lens on which the step operates. 
      The result of running the HASH algorithm on the label indicates the correct box for that step.
      */

     /*

      The label will be immediately followed by a character that indicates the operation to perform: either an equals sign (=) or a dash (-).
      */
     /*

      If the operation character is a dash (-), go to the relevant box and remove the lens with the given label if it is present in the box. Then, move any remaining lenses as far forward in the box as they can go without changing their order, filling any space made by removing the indicated lens. (If no lens in that box has the given label, nothing happens.)

      If the operation character is an equals sign (=), it will be followed by a number indicating the focal length of the lens that needs to go into the relevant box; be sure to use the label maker to mark the lens with the label given in the beginning of the step so you can find it later. There are two possible situations:
      */

     /*

      If there is already a lens in the box with the same label, replace the old lens with the new lens: remove the old lens and put the new lens in its place, 
      not moving any other lenses in the box.
      If there is not already a lens in the box with the same label, 
      add the lens to the box immediately behind any lenses already in the box. 
      Don't move any of the other lenses when you do this. If there aren't any lenses in the box, the new lens goes all the way to the front of the box.
      */

     /*    
      Here is the contents of every box after each step in the example initialization sequence above:

      After "rn=1":
      Box 0: [rn 1]

      After "cm-":
      Box 0: [rn 1]

      After "qp=3":
      Box 0: [rn 1]
      Box 1: [qp 3]

      After "cm=2":
      Box 0: [rn 1] [cm 2]
      Box 1: [qp 3]

      After "qp-":
      Box 0: [rn 1] [cm 2]

      After "pc=4":
      Box 0: [rn 1] [cm 2]
      Box 3: [pc 4]

      After "ot=9":
      Box 0: [rn 1] [cm 2]
      Box 3: [pc 4] [ot 9]

      After "ab=5":
      Box 0: [rn 1] [cm 2]
      Box 3: [pc 4] [ot 9] [ab 5]

      After "pc-":
      Box 0: [rn 1] [cm 2]
      Box 3: [ot 9] [ab 5]

      After "pc=6":
      Box 0: [rn 1] [cm 2]
      Box 3: [ot 9] [ab 5] [pc 6]

      After "ot=7":
      Box 0: [rn 1] [cm 2]
      Box 3: [ot 7] [ab 5] [pc 6]
      */
      for (auto const& entry : model) {
        std::string label{};
        char operation{};
        std::string value{};
        std::tie(label, operation, value) = splitEntry(entry);
        auto box_index = hash(label);
        switch (operation) {
          case '-': {
            auto& box = boxes[box_index];
            auto it = std::find_if(box.lenses.begin(), box.lenses.end(), [&](Lens const& lens) {
              return lens.label == label;
            });
            if (it != box.lenses.end()) {
              box.lenses.erase(it); // erase existing lense
            }
            break;
          }
          case '=': {
            auto& box = boxes[box_index];
            auto it = std::find_if(box.lenses.begin(), box.lenses.end(), [&](Lens const& lens) {
              return lens.label == label;
            });
            if (it != box.lenses.end()) {
              it->focal_length = std::stoi(value); // change focal length of existing lense
            }
            else {
              box.lenses.push_back({label,std::stoi(value)});
            }
            break;
          }
        }
        std::cout << NL << "After " << std::quoted(entry) << ":";
        for (auto iter = boxes.begin(); iter != boxes.end(); ++iter) {
          if (!iter->lenses.empty()) {
            std::cout << NT << "Box " << std::distance(boxes.begin(), iter) << ":";
            for (auto const& lens : iter->lenses) {
              std::cout << " [" << lens.label << " " << lens.focal_length << "]";
            }
          }
        }
        /*

        ...

        After "ot=7":
          Box 0: [rn 1] [cm 2]
          Box 3: [ot 7] [ab 5] [pc 6]
      */

      } 

     /*
      All 256 boxes are always present; only the boxes that contain any lenses are shown here. Within each box, lenses are listed from front to back; 
      each lens is shown as its label and focal length in square brackets.

      To confirm that all of the lenses are installed correctly, add up the focusing power of all of the lenses. 
      The focusing power of a single lens is the result of multiplying together:

      One plus the box number of the lens in question.
      The slot number of the lens within the box: 1 for the first lens, 2 for the second lens, and so on.
      The focal length of the lens.
      */
    for (auto boxIter = boxes.begin(); boxIter != boxes.end(); ++boxIter) {
      for (auto lensIter = boxIter->lenses.begin(); lensIter != boxIter->lenses.end(); ++lensIter) {
        auto box_number = std::distance(boxes.begin(), boxIter) + 1;
        auto slot_number = std::distance(boxIter->lenses.begin(), lensIter) + 1;
        result += box_number * slot_number * lensIter->focal_length;
      }
    }
    /*
      At the end of the above example, the focusing power of each lens is as follows:

      rn: 1 (box 0) * 1 (first slot) * 1 (focal length) = 1
      cm: 1 (box 0) * 2 (second slot) * 2 (focal length) = 4
      ot: 4 (box 3) * 1 (first slot) * 7 (focal length) = 28
      ab: 4 (box 3) * 2 (second slot) * 5 (focal length) = 40
      pc: 4 (box 3) * 3 (third slot) * 6 (focal length) = 72
      So, the above example ends up with a total focusing power of 145.

      With the help of an over-enthusiastic reindeer in a hard hat, follow the initialization sequence. What is the focusing power of the resulting lens configuration?
      */      
      return result;
  }
}

int main(int argc, char *argv[])
{
  Solution solution{};
  std::cout << NL << "argc : " << argc;
  Answer part1_answer{ "Failed to obtain any input",0 };
  Answer part2_answer{ "Failed to obtain any input",0 };
  if (argc == 1) {
    std::cout << NL << "no data file provided ==> WIll use hard coded example input";
    std::istringstream in{ example };
    auto model = parse(in);
    part1_answer = { "Example",part1::solve_for(model) };
    part2_answer = { "Example",part2::solve_for(model) };
    solution.part1.push_back(part1_answer);
    solution.part2.push_back(part2_answer);
  }
  else for (int i = 0; i < argc; ++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
    if (i > 0) {
      std::ifstream in{ argv[i] };
      if (in) {
        auto model = parse(in);
        part1_answer = { argv[i],part1::solve_for(model) };
        part2_answer = { argv[i],part2::solve_for(model) };
      }
      else {
        part1_answer = { std::string{"Failed to open file "} + argv[i],-1 };
        part2_answer = { std::string{"Failed to open file "} + argv[i],-1 };
      }
      solution.part1.push_back(part1_answer);
      solution.part2.push_back(part2_answer);
    }
  }
  std::cout << NL << NL << "------------ REPORT----------------";
  std::cout << NL << "<Part 1>";
  for (auto const& answer : solution.part1) {
    if (answer.second != 0) std::cout << NT << "answer[" << answer.first << "] " << answer.second;
    else std::cout << NT << "answer[" << answer.first << "] " << " NO OPERATION ";
  }
  std::cout << NL << "<Part 2>";
  for (auto const& answer : solution.part2) {
    if (answer.second != 0) std::cout << NT << "answer[" << answer.first << "] " << answer.second;
    else std::cout << NT << "answer[" << answer.first << "] " << " NO OPERATION ";
  }
  std::cout << "\n\n";
  return 0;
}