#include <cctype>
#include <iostream>
#include <iomanip> // E.g., std::quoted
#include <string>
#include <string_view>
#include <sstream> // E.g., std::istringstream, std::ostringstream
#include <vector>
#include <set>
#include <unordered_set>
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
#include <functional>


char const* example = R"(broadcaster -> a, b, c
%a -> b
%b -> c
%c -> inv
&inv -> a)";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int64_t; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22 x 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
struct Solution {
  Answers part1{};
  Answers part2{};
};

struct Wiring {
  std::string name{};
  std::string type{};
  std::vector<std::string> destinations{};
};
using Model = std::vector<Wiring>;

Model parse(auto& in) {
  Model result{};
  std::string line{};
  while (std::getline(in,line)) {
    std::string_view name{};
    std::string_view type{};
    std::vector<std::string_view> destinations{};
    std::cout << NL << "parse: " << std::quoted(line);
    auto sv = std::string_view{ line };
    std::string delim{" -> "};
    if (auto pos = sv.find(delim); pos != std::string_view::npos) {
      auto left = sv.substr(0, pos);
      switch (left[0]) {
      case '%': type = left.substr(0, 1); name = left.substr(1); break;
      case '&': type = left.substr(0, 1); name = left.substr(1); break;
      default: name = left; break;
      }
      auto right = sv.substr(pos + std::size(delim));
      std::cout << NT << "left : " << std::quoted(left);
      std::cout << NT << "right : " << std::quoted(right);
      std::string delim2{", "};
      while (true) {
        auto pos2 = right.find(delim2);
        auto destination = right.substr(0, pos2);
        destinations.push_back(destination);
        if (pos2 == std::string_view::npos) break;
        right.remove_prefix(pos2 + std::size(delim2));
      }
      result.push_back({ std::string{name},std::string{type},std::vector<std::string>{destinations.begin(),destinations.end()} });
    }
    else std::cout << NL << "failed to find '->' in " << sv;
  }
  return result;
}

// broadcaster -> a, b, c
// %a -> b
// %b -> c
// %c -> inv
// &inv -> a
void print_model(Model const& model) {
  std::cout << NL << "model:";
  for (auto const& [name,type,destinations] : model) {
    std::cout << NT << type << name << " -> ";
    int count{};
    for (auto const& destination : destinations) {
      if (count++ > 0) std::cout << ", ";
      std::cout << destination; 
    }
  }
}

namespace part1 {
  bool const HIGH{ true };
  bool const LOW{ false };
  using Signal = std::optional<bool>;
  using Cache = std::map<std::string,Signal>;

  template <typename T>
  struct hash {};

  template <>
  struct hash<Cache> {
    size_t operator()(const Cache& cache) const {
      size_t seed = 0;
      for (const auto& entry : cache) {
        seed ^= std::hash<std::string>{}(entry.first) ^ (std::hash<Signal>{}(*entry.second) << 1);
      }
      return seed;
    }
  };

  // Flip-flop modules (prefix %) 
  //   are either on or off; they are initially off. If a flip-flop module receives a high pulse, it is ignored and nothing happens. 
  //   However, if a flip-flop module receives a low pulse, it flips between on and off. 
  //   If it was off, it turns on and sends a high pulse. If it was on, it turns off and sends a low pulse.
  auto flip_flop = [](Cache const& cache,bool state,Signal const& pulse)->Signal {
    if (pulse != std::nullopt) {
      return (state and *pulse == LOW)? state ^ state : state; // xor flip, or nothing
    }
    else return std::nullopt;
  };

  // Conjunction modules (prefix &) 
  //   remember the type of the most recent pulse received from each of their connected input modules; 
  //   they initially default to remembering a low pulse for each input. 
  //   When a pulse is received, the conjunction module first updates its memory for that input. 
  //   Then, if it remembers high pulses for all inputs, it sends a low pulse; otherwise, it sends a high pulse.
  auto conjunction = [](Cache const& cache,bool state,Signal const& pulse)->Signal {
    if (std::all_of(cache.begin(), cache.end(), [](auto const& entry) {
      auto [name,signal] = entry;
      return signal and *signal == HIGH; 
      })) {
      return LOW;
    }
    else return HIGH;
  };

  auto const broadcast = [](Cache const& cache,bool state,Signal const& pulse)->Signal {
    return pulse;
  };

  namespace detail {
    template <typename F>
    struct Module {
      Cache cache{}; // initially all false / low
      bool state{ LOW };
      F op{};
      Signal act_on(std::string name,Signal pulse) {
        cache[name] = pulse;
        return op(cache,state,pulse);
      }
      size_t hash_value() const {
        size_t type_hash = std::hash<std::string_view>{}(typeid(F).name());
        size_t cache_hash = hash<Cache>{}(cache);
        size_t state_hash = std::hash<bool>{}(state);
        return type_hash ^ (cache_hash + state_hash + 0x9e3779b9 + (type_hash << 6) + (type_hash >> 2));
      }
    };
  } // namespace detail
  template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
  template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;   
  using Module = std::variant<detail::Module<decltype(broadcast)>,detail::Module<decltype(flip_flop)>,detail::Module<decltype(conjunction)>>;
  using Modules = std::map<std::string,Module>;

  template<>
  struct hash<Module> {
    size_t operator()(const Module& module) const {
      return std::visit([](const auto& arg) {
        using T = std::decay_t<decltype(arg)>;
        size_t type_hash = std::hash<std::string_view>{}(typeid(T).name());
        size_t value_hash = arg.hash_value(); // Assuming each Module type has a hash_value() method
        return type_hash ^ (value_hash + 0x9e3779b9 + (type_hash << 6) + (type_hash >> 2));
      }, module);
    }
  };

  template<>
  struct hash<Modules> {
    size_t operator()(const Modules& modules) const {
      size_t seed = 0;
      for (const auto& entry : modules) {
        seed ^= std::hash<std::string>{}(entry.first) ^ (hash<Module>{}(entry.second) << 1);
      }
      return seed;
    }
  };

  struct AlwaysDistinct {
      bool operator()(const Modules& lhs, const Modules& rhs) const {
          return false;
      }
  };  

  // Analyze and dig into the problem to find a good solution
  void explore(Model const& model) {
    /*
    With your help, the Elves manage to find the right parts and fix all of the machines. 
    Now, they just need to send the command to boot up the machines and get the sand flowing again.

    The machines are far apart and wired together with long cables. 
    The cables don't connect to the machines directly, 
    but rather to communication modules attached to the machines that perform various initialization tasks 
    and also act as communication relays.

    Modules communicate using pulses. Each pulse is either a high pulse or a low pulse. 
    When a module sends a pulse, it sends that type of pulse to each module in its list of destination modules.
    */
   /*

    There are several different types of modules:

    Flip-flop modules (prefix %) 
      are either on or off; they are initially off. If a flip-flop module receives a high pulse, it is ignored and nothing happens. 
      However, if a flip-flop module receives a low pulse, it flips between on and off. 
      If it was off, it turns on and sends a high pulse. If it was on, it turns off and sends a low pulse.
      */

     /*
    Conjunction modules (prefix &) 
      remember the type of the most recent pulse received from each of their connected input modules; 
      they initially default to remembering a low pulse for each input. 
      When a pulse is received, the conjunction module first updates its memory for that input. 
      Then, if it remembers high pulses for all inputs, it sends a low pulse; otherwise, it sends a high pulse.
      */

    /*

    There is a single broadcast module (named broadcaster). 
      When it receives a pulse, it sends the same pulse to all of its destination modules.
    */
    /*

    Here at Desert Machine Headquarters, there is a module with a single button on it called, aptly, the button module. 
    When you push the button, a single low pulse is sent directly to the broadcaster module.

    After pushing the button, you must wait until all pulses have been delivered and fully handled before pushing it again. 
    Never push the button if modules are still processing pulses.

    Pulses are always processed in the order they are sent. 
    So, if a pulse is sent to modules a, b, and c, and then module 'a' processes its pulse and sends more pulses, 
    the pulses sent to modules b and c would have to be handled first.

    The module configuration (your puzzle input) lists each module. 
    The name of the module is preceded by a symbol identifying its type, if any. 
    The name is then followed by an arrow and a list of its destination modules. For example:

    broadcaster -> a, b, c
    %a -> b
    %b -> c
    %c -> inv
    &inv -> a

    */
      print_model(model);
    /*

    In this module configuration, the broadcaster has three destination modules named a, b, and c. 
    Each of these modules is a flip-flop module (as indicated by the % prefix). 
    a outputs to b which outputs to c which outputs to another module named inv. 
    inv is a conjunction module (as indicated by the & prefix) which, 
    because it has only one input, acts like an inverter (it sends the opposite of the pulse type it receives); it outputs to a.

    By pushing the button once, the following pulses are sent:

    button -low-> broadcaster
    broadcaster -low-> a
    broadcaster -low-> b
    broadcaster -low-> c
    a -high-> b
    b -high-> c
    c -high-> inv
    inv -low-> a
    a -low-> b
    b -low-> c
    c -low-> inv
    inv -high-> a

    */
    /*

    After this sequence, the flip-flop modules all end up off, so pushing the button again repeats the same sequence.

    Here's a more interesting example:

    broadcaster -> a
    %a -> inv, con
    &inv -> b
    %b -> con
    &con -> output

    This module configuration includes the broadcaster, two flip-flops (named a and b), a single-input conjunction module (inv), a multi-input conjunction module (con), and an untyped module named output (for testing purposes). The multi-input conjunction module con watches the two flip-flop modules and, if they're both on, sends a low pulse to the output module.

    Here's what happens if you push the button once:

    button -low-> broadcaster
    broadcaster -low-> a
    a -high-> inv
    a -high-> con
    inv -low-> b
    con -high-> output
    b -high-> con
    con -low-> output

    Both flip-flops turn on and a low pulse is sent to output! 
    However, now that both flip-flops are on and con remembers a high pulse from each of its two inputs, 

    pushing the button a second time does something different:

    button -low-> broadcaster
    broadcaster -low-> a
    a -low-> inv
    a -low-> con
    inv -high-> b
    con -high-> output

    Flip-flop a turns off! Now, con remembers a low pulse from module a, and so it sends only a high pulse to output.

    Push the button a third time:

    button -low-> broadcaster
    broadcaster -low-> a
    a -high-> inv
    a -high-> con
    inv -low-> b
    con -low-> output
    b -low-> con
    con -high-> output

    This time, flip-flop a turns on, then flip-flop b turns off. 
    However, before b can turn off, the pulse sent to con is handled first, 
    so it briefly remembers all high pulses for its inputs and sends a low pulse to output. 
    After that, flip-flop b turns off, which causes con to update its state and send a high pulse to output.

    Finally, with a on and b off, push the button a fourth time:

    button -low-> broadcaster
    broadcaster -low-> a
    a -low-> inv
    a -low-> con
    inv -high-> b
    con -high-> output

    This completes the cycle: a turns off, causing con to remember only low pulses and restoring all modules to their original states.

    To get the cables warmed up, the Elves have pushed the button 1000 times. 
    How many pulses got sent as a result (including the pulses sent by the button itself)?

    In the first example, the same thing happens every time the button is pushed: 8 low pulses and 4 high pulses are sent. 
    So, after pushing the button 1000 times, 8000 low pulses and 4000 high pulses are sent. Multiplying these together gives 32000000.

    In the second example, after pushing the button 1000 times, 4250 low pulses and 2750 high pulses are sent. 
    Multiplying these together gives 11687500.

    Consult your module configuration; 
    determine the number of low pulses and high pulses that would be sent after pushing the button 1000 times, 
    waiting for all pulses to be fully handled after each push of the button. 

    What do you get if you multiply the total number of low pulses sent by the total number of high pulses sent?
    */
    struct World {
      Model model{};
      Modules modules{};
      Result m_low_count{ 0 };
      Result m_high_count{ 0 };
      std::unordered_set<Modules, hash<Modules>,AlwaysDistinct> seen{};
      void act_on(std::string const& from, Signal pulse, std::string const& to) {
        using Message = std::tuple<std::string,Signal,std::string>;
        std::queue<Message> message_queue{};
        message_queue.push({ from,pulse,to });
        while (!seen.contains(modules) and !message_queue.empty()) {
          auto [from,pulse,to] = message_queue.front();
          message_queue.pop();
          std::cout << NL << "act_on: " << std::quoted(from) << " " << (pulse ? (*pulse ? "high" : "low") : "null") << " " << std::quoted(to);
          m_low_count += (pulse and *pulse == LOW);
          m_high_count += (pulse and *pulse == HIGH);
          std::cout << NL << "act_on: " << std::quoted(from) << " " << (pulse? "high" : "low") << " " << std::quoted(to);
          auto it = std::find_if(model.begin(), model.end(), [&to](auto const& wiring) { return wiring.name == to; });
          if (it != model.end()) {
            auto [name,type,destinations] = *it;
            if (modules.find(name) == modules.end()) {
              switch (type[0]) {
              case '%': modules.insert({name, detail::Module<decltype(flip_flop)>{}}); break;
              case '&': modules.insert({name, detail::Module<decltype(conjunction)>{}}); break;
              default: modules.insert({name, detail::Module<decltype(broadcast)>{}}); break;
              }
            }
            
            Signal out = std::visit(overloaded{
              [from, pulse](detail::Module<decltype(flip_flop)>& module) {
                return module.act_on(from, pulse);               
              },
              [from, pulse](detail::Module<decltype(conjunction)>& module) {
                // handle conjunction module using 'from' and 'pulse'
                return module.act_on(from, pulse);               
              },
              [from, pulse](detail::Module<decltype(broadcast)>& module) {
                // handle broadcast module using 'from' and 'pulse'
                return module.act_on(from, pulse);               
              }
            }, modules.at(name));
            for (auto const& destination : destinations) {
              message_queue.push({ name,out,destination });
            }
          }
          else std::cout << NL << "failed to find: " << std::quoted(to);
        }

      }
      Result low_count() {
        return m_low_count;
      }
      Result high_count() {
        return m_high_count;
      }
    };
    auto world = World{model};
    for (int i = 0; i < 1000; ++i) {
      world.act_on("button", LOW,"broadcaster");
    };
    auto low_count = world.low_count();
    auto high_count = world.high_count();
    Result result = low_count * high_count;
    std::cout << NL << NL << "==> explore says result : " << result; 

  }
  Result solve_for(Model& model) {
    Result result{};
    std::cout << NL << NL << "part1";
    explore(model); // Until a solution is found :)
    return result;
  }
}

namespace part2 {
  Result solve_for(Model& model) {
    Result result{};
    std::cout << NL << NL << "part2";
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