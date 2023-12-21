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


auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = long long; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22 x 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
using Solution = std::map<int, Answers>; // Puzzle part -> Answers

using Integer = int64_t; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22 x 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
using Solution = std::map<int, Answers>; // Puzzle part -> Answers

/*
--- Day 5: If You Give A Seed A Fertilizer ---

You take the boat and find the gardener right where you were told he would be: managing a giant "garden" that looks more to you like a farm.

"A water source? Island Island is the water source!" You point out that Snow Island isn't receiving any water.

"Oh, we had to stop the water because we ran out of sand to filter it with! Can't make snow with dirty water. Don't worry, I'm sure we'll get more sand soon; we only turned off the water a few days... weeks... oh no." His face sinks into a look of horrified realization.

"I've been so busy making sure everyone here has food that I completely forgot to check why we stopped getting more sand! There's a ferry leaving soon that is headed over in that direction - it's much faster than your boat. Could you please go check it out?"

You barely have time to agree to this request when he brings up another. "While you wait for the ferry, maybe you can help us with our food production problem. The latest Island Island Almanac just arrived and we're having trouble making sense of it."

The almanac (your puzzle input) lists all of the seeds that need to be planted. It also lists what type of soil to use with each kind of seed, what type of fertilizer to use with each kind of soil, what type of water to use with each kind of fertilizer, and so on. Every type of seed, soil, fertilizer and so on is identified with a number, but numbers are reused by each category - that is, soil 123 and fertilizer 123 aren't necessarily related to each other.

For example:

seeds: 79 14 55 13

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
56 93 4
The almanac starts by listing which seeds need to be planted: seeds 79, 14, 55, and 13.

The rest of the almanac contains a list of maps which describe how to convert numbers from a source category into numbers in a destination category. That is, the section that starts with seed-to-soil map: describes how to convert a seed number (the source) to a soil number (the destination). This lets the gardener and his team know which soil to use with which seeds, which water to use with which fertilizer, and so on.

Rather than list every source number and its corresponding destination number one by one, the maps describe entire ranges of numbers that can be converted. Each line within a map contains three numbers: the destination range start, the source range start, and the range length.

Consider again the example seed-to-soil map:

50 98 2
52 50 48
The first line has a destination range start of 50, a source range start of 98, and a range length of 2. This line means that the source range starts at 98 and contains two values: 98 and 99. The destination range is the same length, but it starts at 50, so its two values are 50 and 51. With this information, you know that seed number 98 corresponds to soil number 50 and that seed number 99 corresponds to soil number 51.

The second line means that the source range starts at 50 and contains 48 values: 50, 51, ..., 96, 97. This corresponds to a destination range starting at 52 and also containing 48 values: 52, 53, ..., 98, 99. So, seed number 53 corresponds to soil number 55.

Any source numbers that aren't mapped correspond to the same destination number. So, seed number 10 corresponds to soil number 10.

So, the entire list of seed numbers and their corresponding soil numbers looks like this:

seed  soil
0     0
1     1
...   ...
48    48
49    49
50    52
51    53
...   ...
96    98
97    99
98    50
99    51
With this map, you can look up the soil number required for each initial seed number:

Seed number 79 corresponds to soil number 81.
Seed number 14 corresponds to soil number 14.
Seed number 55 corresponds to soil number 57.
Seed number 13 corresponds to soil number 13.
The gardener and his team want to get started as soon as possible, so they'd like to know the closest location that needs a seed. Using these maps, find the lowest location number that corresponds to any of the initial seeds. To do this, you'll need to convert each seed number through other categories until you can find its corresponding location number. In this example, the corresponding types are:

Seed 79, soil 81, fertilizer 81, water 81, light 74, temperature 78, humidity 78, location 82.
Seed 14, soil 14, fertilizer 53, water 49, light 42, temperature 42, humidity 43, location 43.
Seed 55, soil 57, fertilizer 57, water 53, light 46, temperature 82, humidity 82, location 86.
Seed 13, soil 13, fertilizer 52, water 41, light 34, temperature 34, humidity 35, location 35.
So, the lowest location number in this example is 35.

What is the lowest location number that corresponds to any of the initial seed numbers?

*/

/*

--- Part Two ---

Everyone will starve if you only plant such a small number of seeds. Re-reading the almanac, it looks like the seeds: line actually describes ranges of seed numbers.

The values on the initial seeds: line come in pairs. Within each pair, the first value is the start of the range and the second value is the length of the range. So, in the first line of the example above:

seeds: 79 14 55 13
This line describes two ranges of seed numbers to be planted in the garden. The first range starts with seed number 79 and contains 14 values: 79, 80, ..., 91, 92. The second range starts with seed number 55 and contains 13 values: 55, 56, ..., 66, 67.

Now, rather than considering four seed numbers, you need to consider a total of 27 seed numbers.

In the above example, the lowest location number can be obtained from seed number 82, which corresponds to soil 84, fertilizer 84, water 84, light 77, temperature 45, humidity 46, and location 46. So, the lowest location number is 46.

Consider all of the initial seed numbers listed in the ranges on the first line of the almanac. What is the lowest location number that corresponds to any of the initial seed numbers?

Answer:  

Although it hasn't changed, you can still get your puzzle input.

You can also [Share] this puzzle.
*/

enum Category {
  undefined
  ,seed
  ,soil
  ,fertilizer
  ,water
  ,light
  ,temperature
  ,humidity
  ,location
  ,unknown
  
};
struct Value {
  Category category{undefined};
  Integer value{};
  bool operator<(Value const& other) const {return (category==other.category)?value < other.value:category<other.category;}
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
  Map(Category target_category,MapEntries entries) 
    :  target_category{target_category}
      ,entries{entries} {}
  Map() = default;
  Values operator[](Integer source) const {
    Values result{};
    bool has_mapping{false};
    for (auto const& entry : entries) {
      Integer offset = (source-entry.source_begin);
      if (offset>=0 and offset<entry.length) {
        // defined mapping
        has_mapping = true;
        result.insert({target_category,entry.target_begin + offset});
        // std::cout << NT << "entry.target_begin:" << entry.target_begin << "+" << offset << "=" << entry.target_begin + offset;
      }
    }
    if (!has_mapping) result.insert({target_category,source});
    return result;
  }
private:
  Category target_category;
  MapEntries entries{};
};
using Maps = std::map<Category,Map>;
struct Model {
  std::vector<Integer> seeds;
  Maps maps;
};

Model parse(auto& in) {
    Model result{};
    std::string line{};
    while (std::getline(in,line)) {
      std::cout << NL << "parsing:" << std::quoted(line);
      std::pair<Category,Category> mapping{undefined,undefined};
      if (line.starts_with("seeds:")) {
        auto s = line.substr(std::strlen("seeds:")+1);
        std::cout << " as seeds " << std::quoted(s) << "-->";
        std::istringstream ss{s};
        std::string n;
        while (ss >> n) {
          std::cout << " value:" << n;
          result.seeds.push_back(std::stoll(n));
        }
      }
      else if (line.starts_with("seed-to-soil map:")) mapping = {seed,soil};
      else if (line.starts_with("soil-to-fertilizer map:")) mapping = {soil,fertilizer};
      else if (line.starts_with("fertilizer-to-water map:")) mapping = {fertilizer,water};
      else if (line.starts_with("water-to-light map:")) mapping = {water,light};
      else if (line.starts_with("light-to-temperature map:")) mapping = {light,temperature};
      else if (line.starts_with("temperature-to-humidity map:")) mapping = {temperature,humidity};
      else if (line.starts_with("humidity-to-location map:")) mapping = {humidity,location};

      if (mapping.first != undefined and mapping.second != undefined) {
        MapEntries entries{};
        while (std::getline(in,line)) {
          std::cout << NT << "entry:" << line;
          if (line.empty()) break;
          MapEntry entry{};
          if (std::istringstream{line} >> entry.target_begin >> entry.source_begin >> entry.length) {
            std::cout << " push";
            entries.push_back(entry);
          }
        }
        result.maps[mapping.first] = Map{mapping.second,entries};
      }
    }
    return result;
}

Values source_to_targets(Maps const& maps,Value const& source,Category end_category) {
  Values result{};
  std::queue<Value> unmapped{};unmapped.push(source);
  while (unmapped.size()>0) {
    auto current = unmapped.front(); unmapped.pop();
    // std::cout << NT << current.category << ":" << current.value << " --> ";
    if (maps.contains(current.category)) {
      auto map = maps.at(current.category);
      auto mapped = map[current.value];
      for (auto const& next : mapped) {
        if (next.category == end_category) {
          result.insert(next);
          // std::cout << " END " << next.category << ":" << next.value;
        }
        else {
          // std::cout << " NEXT " << next.category << ":" << next.value;
          unmapped.push(next);
        }
      }
    }
    else {
      std::cout << NL << "ERROR: No mapping defined for source category " << current.category;
    }
  }
  return result;
}

namespace part1 {

  Result solve_for(auto& in) {
      Result result{std::numeric_limits<Result>::max()};
      auto model = parse(in);
      /*
      What is the lowest location number that corresponds to any of the initial seed numbers?

      To answer we have an "almanac" with "seeds" and then a set of "maps".

      seed-to-soil map:
      soil-to-fertilizer map:
      fertilizer-to-water map:
      water-to-light map:
      light-to-temperature map:
      temperature-to-humidity map:
      humidity-to-location map:

      Ok, so to transform a "seed" to a "location" we need to trace the seed through the maps to its "location"
      seed -> seed-to-soil -> soil to fertilizer -> fertilizer-to-water -> water-to-light -> light-to-temperature -> temperature-to-humidity -> humidity-to-location -> location

      Each mapping is a bit "messed up" and not so straight forward.
      1) The map consists of a list of triplets of numbers.
      2) The numbers define <start of target range> <start of source range> <range length>
      3) Additionally, a source NOT in the source range maps to a target with the same value.

      Aha, because each map defines multiple ranges to map, the number of possible location for each seed is more than one.
      So each map "branches" an source to multiple targets.
      AND, we must suspect that mappings can overlap (a map can define alternative mappings for each source value).

      If we study seed-to-soil map:
      50 98 2
      52 50 48       

      The first entry maps the range [50..51] to [98..99] (range length 2)
      The second entry maps range [52..(52+48-1)] to [50..(50+48-1)] (range length 48)
      Ok, so for this map the source ranges does NOT overlap.
      I wonder if any maps of the puzzle input does?

      Hm...how do we design an algorithm for this?

      It seems we need a way to mark a "value" with its "type" (seed value, soil value, fertilizer value etc...)?

      for each seed in seeds {
        candidates = seed_to_locations(seed)
      }

      seed_to_locations : seed -> locations
      {
        source = seed
        do {
          targets = source_to_targets(source)
        } while (targets.type != location) {
        return targets
      }

      source_to_targets : source -> target
      {
        source_map = maps[source.type]
        targets = source_map[source]
        answer = min(targets)
      }
      */
      
      for (auto const& value : model.seeds) {
        // std::cout << NL << "seed:" << value << std::flush;
        // auto candidates = source_to_targets(model.maps,seed,fertilizer);
        auto candidates = source_to_targets(model.maps,{seed,value},location);

        result = std::accumulate(candidates.begin(),candidates.end(),result,[](auto acc,auto const& candidate){
          acc = std::min(candidate.value,acc);
          return acc;});
      }
      return result;// 57075758
  }
}

namespace part2 {
  // based on https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day05p2.py
  // Thanks!
  namespace hyperneutrino {

    int main(auto& file) {
      std::string line{};
      std::getline(file,line);
      std::cout << NL << "parsing:" << std::quoted(line) << std::flush;
      std::pair<Category,Category> mapping{undefined,undefined};
      std::vector<Integer> input_values;
      auto s = line.substr(std::strlen("seeds:")+1);
      std::cout << " as seeds " << std::quoted(s) << "-->";
      std::istringstream ss{s};
      std::string n;
      while (ss >> n) {
        std::cout << " value:" << n << std::flush;
        Integer value = std::stoll(n);
        if (value<0) {
          std::cout << NL << "ERROR: value < 0" << std::flush;
          exit(1);
        }
        input_values.push_back(value);
      }

      std::vector<std::pair<Integer, Integer>> seeds;
      for (Integer i = 0; i < input_values.size(); i += 2) {
        seeds.push_back({input_values[i], input_values[i] + input_values[i + 1]});
      }

      while (file) {
        std::string block;
        while (std::getline(file, line) and !line.empty()) block += line + '\n';
        std::cout << NL << "block:" << std::quoted(block) << std::flush;

        std::vector<std::vector<Integer>> ranges;
        std::istringstream iss(block);
        std::string line;
        while (std::getline(iss, line)) {
          std::cout << NL << NT << "line:" << std::quoted(line) << std::flush;
          if (line.find(':') != std::string::npos) continue; // skip header
          std::istringstream iss2(line);
          std::vector<Integer> temp;
          Integer value;
          while (iss2 >> value) {
            std::cout << NT << NT << "value:" << value << std::flush;
            temp.push_back(value);
          }
          std::cout << NT << "push temp size:" << temp.size() << std::flush;
          ranges.push_back(temp);
          if (temp.size() != 3) {
            std::cout << NL << "ERROR: temp size:" << temp.size() << std::flush;
            exit(1);
          }
        }

        std::cout << NL << "while seeds : size:" << seeds.size() << std::flush;

        std::vector<std::pair<Integer, Integer>> new_seeds;
        while (!seeds.empty()) {
          auto [s, e] = seeds.back(); // seed range start..end
          std::cout << NT << "seed: " << s << " .. " << e << std::flush;
          seeds.pop_back();
          bool broken = false;
          for (auto& range : ranges) {
            std::cout << NT << "range size:" << range.size() << std::flush;
            Integer a = range[0], b = range[1], c = range[2];
            Integer os = std::max(s, b);
            Integer oe = std::min(e, b + c);
            if (os < oe) {
              std::cout << NT << NT << "new seeds " << os - b + a << ".." << oe - b + a << std::flush;
              if (os - b + a < 0) {
                std::cout << NL << "ERROR: os - b + a < 0" << std::flush;
                exit(1);
              }
              if (oe - b + a < 0) {
                std::cout << NL << "ERROR: oe - b + a < 0" << std::flush;
                exit(1);
              }
              new_seeds.push_back({os - b + a, oe - b + a});
              if (os > s) {
                seeds.push_back({s, os});
              }
              if (e > oe) {
                seeds.push_back({oe, e});
              }
              broken=true;
              break;
            }
          }
          if (!broken) {
            std::cout << NT << NT << "new seeds " << s << " .. " << e << std::flush;
            if (s<0 or e<0) {
              std::cout << NL << "ERROR: s<0 or e<0" << std::flush;
              exit(1);  
            }
            new_seeds.push_back({s, e});
          }
        }
        seeds = new_seeds;
      }

      std::cout << NL << "seeds size:" << seeds.size() << std::flush;
      for (auto [s, e] : seeds) {
        std::cout << NT << "seed: " << s << " .. " << e << std::flush;
        if (s<0 or e<0) {
          std::cout << NL << "ERROR: s<0 or e<0" << std::flush;
          exit(1);  
        }
      }

      Result result = std::min_element(seeds.begin(), seeds.end())->first; 

      std::cout << NL << "hyperneutrino says: " << result << std::endl;

      return result; // 31161857
    }
  } // namespace hyperneutrino
  Result solve_for(auto& in) {
      Result result{std::numeric_limits<Result>::max()};
      if (true) return hyperneutrino::main(in); // 31161857
      else return 0;
  }
}

int main(int argc, char *argv[])
{
  Solution solution{};
  std::cout << NL << "argc : " << argc;
  for (int i = 0; i < argc; ++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
  }
  // day05 part file
  int part{1};
  std::string file{"example.txt"};
  if (argc > 1 ) {
    part = std::stoi(argv[1]);
    if (argc > 2) {
      file = argv[2];
    }
  }
  std::cout << NL << "Part : " << part << " file : " << file;
  std::ifstream in{ file };

  switch (part) {
  case 1: {
    auto answer = part1::solve_for(in);
    solution[part].push_back({ file,answer });
  } break;
  case 2: {
    auto answer = part2::solve_for(in);
    solution[part].push_back({ file,answer });
  } break;
  default:
    std::cout << NL << "No part " << part << " only part 1 and 2";
  }

  std::cout << NL << NL << "------------ REPORT----------------";
  for (auto const& [part, answers] : solution) {
    std::cout << NL << "Part " << part << " answers";
    for (auto const& [heading, answer] : answers) {
      if (answer != 0) std::cout << NT << "answer[" << heading << "] " << answer;
      else std::cout << NT << "answer[" << heading << "] " << " NO OPERATION ";
    }
  }
  std::cout << NL << NL;
  return 0;
}