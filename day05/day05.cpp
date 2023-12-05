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
auto const NT = "\n\t";

using Integer = int; // int: 843 253 387 long int: 32 762 853 787 275 long long int: 32 762 853 787 275
using Result = Integer;
using Answers = std::vector<std::pair<std::string,Result>>;

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
  int value{};
  bool operator<(Value const& other) const {return (category==other.category)?value < other.value:category<other.category;}
};
using Values = std::set<Value>;
struct MapEntry {
  int target_begin{};
  int source_begin{};
  int length{};
};
using MapEntries = std::vector<MapEntry>;
class Map {
public:
  Map(Category target_category,MapEntries entries) 
    :  target_category{target_category}
      ,entries{entries} {}
  Map() = default;
  Values operator[](int source) const {
    Values result{};
    for (auto const& entry : entries) {
      int offset = (source-entry.source_begin);
      if (offset>=0 and offset<entry.length) {
        // defined mapping
        result.insert({target_category,entry.target_begin + offset});
        std::cout << NT << "entry.target_begin:" << entry.target_begin << "+" << offset << "=" << entry.target_begin + offset;
      }
      else {
        result.insert({target_category,source});
      }
    }
    return result;
  }
private:
  Category target_category;
  MapEntries entries{};
};
using Maps = std::map<Category,Map>;
struct Model {
  Values seeds;
  Maps maps;
};

Model parse(auto& in) {
    Model result{};
    if (false) {
      result.seeds = {{seed,79},{seed,14},{seed,55},{seed,13}};
      result.maps.insert({seed,Map{soil,{{50,98,2},{52,50,48}}}});
      result.maps.insert({soil,Map{fertilizer,{{0,15,37},{37,52,2},{39,0,15}}}});
    }
    // parse actual input
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
          result.seeds.insert({seed,std::stoi(n)});
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

namespace part1 {
    Values source_to_targets(Maps const& maps,Value const& source,Category end_category) {
      Values result{};
      /*
        sources.push(source)
        targets{}
        do {
          source = sources.pop()
          source_map = maps[source.type]
          targets = source_map[source]
          if (targets.type != target_type) {
            for each source in targets {
              mapped = source_to_targets[source]
              sources.push(mapped);
            }
          }
        } while sources.size() > 0
        return targets
      */
      std::queue<Value> unmapped{};unmapped.push(source);
      while (unmapped.size()>0) {
        auto current = unmapped.front(); unmapped.pop();
        std::cout << NT << current.category << ":" << current.value << " --> ";
        if (maps.contains(current.category)) {
          auto map = maps.at(current.category);
          auto mapped = map[current.value];
          for (auto const& next : mapped) {
            if (next.category == end_category) {
              result.insert(next);
              std::cout << " END " << next.category << ":" << next.value;
            }
            else {
              std::cout << " NEXT " << next.category << ":" << next.value;
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

      hm...it seems we should just use recursion?

      candidates = source_to_targets(seed)

      source_to_targets: source -> target_type -> targets
      {
        sources.push(source)
        targets{}
        do {
          source = sources.pop()
          source_map = maps[source.type]
          targets = source_map[source]
          if (targets.type != target_type) {
            for each source in targets {
              mapped = source_to_targets[source]
              sources.push(mapped);
            }
          }
        } while sources.size() > 0
        return targets
      }

      Lets try this approach :)
      */
      
      for (auto const& seed : model.seeds) {
        std::cout << NL << "seed:" << seed.value << std::flush;
        // auto candidates = source_to_targets(model.maps,seed,fertilizer);
        auto candidates = source_to_targets(model.maps,seed,location);

        result = std::accumulate(candidates.begin(),candidates.end(),result,[](auto acc,auto const& candidate){
          acc = std::min(candidate.value,acc);
          return acc;});
      }
      return result;
  }
}

namespace part2 {
  Result solve_for(auto& in) {
      Result result{};
      auto data_model = parse(in);
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