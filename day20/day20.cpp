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
using Solution = std::map<int, Answers>; // Puzzle part -> Answers

/*
--- Day 20: Pulse Propagation ---

With your help, the Elves manage to find the right parts and fix all of the machines. 
Now, they just need to send the command to boot up the machines and get the sand flowing again.

The machines are far apart and wired together with long cables. 
The cables don't connect to the machines directly, 
but rather to communication modules attached to the machines 
that perform various initialization tasks and also act as communication relays.

Modules communicate using pulses. 
Each pulse is either a high pulse or a low pulse. 
When a module sends a pulse, it sends that type of pulse 
to each module in its list of destination modules.

There are several different types of modules:

Flip-flop modules (prefix %) 
  are either on or off; 
  they are initially off. 
  If a flip-flop module receives a high pulse, 
  it is ignored and nothing happens. 
  However, if a flip-flop module receives a low pulse, 
  it flips between on and off. 
  If it was off, it turns on and sends a high pulse. 
  If it was on, it turns off and sends a low pulse.

Conjunction modules (prefix &) 
  remember the type of the most recent pulse 
  received from each of their connected input modules; 
  they initially default to remembering a low pulse for each input. 
  When a pulse is received, 
  the conjunction module first updates its memory for that input. 
  Then, if it remembers high pulses for all inputs, 
  it sends a low pulse; 
  otherwise, it sends a high pulse.

There is a single broadcast module (named broadcaster). 
  When it receives a pulse, 
  it sends the same pulse to all of its destination modules.

Here at Desert Machine Headquarters, 
there is a module with a single button on it called, aptly, 
the button module. 
When you push the button, 
a single low pulse is sent directly to the broadcaster module.

After pushing the button, 
you must wait until all pulses have been delivered and fully handled 
before pushing it again. 

Never push the button if modules are still processing pulses.

Pulses are always processed in the order they are sent. 

So, if a pulse is sent to modules a, b, and c, 
and then module a processes its pulse and sends more pulses, 
the pulses sent to modules b and c would have to be handled first.

The module configuration (your puzzle input) lists each module. 

The name of the module is preceded by a symbol identifying its type, if any. 
The name is then followed by an arrow and a list of its destination modules. For example:

broadcaster -> a, b, c
%a -> b
%b -> c
%c -> inv
&inv -> a

In this module configuration, 
the broadcaster has three destination modules named a, b, and c. 

Each of these modules is a flip-flop module (as indicated by the % prefix). 
a outputs to b which outputs to c which outputs to another module named inv. 

inv is a conjunction module (as indicated by the & prefix) which, because it has only one input, 
acts like an inverter (it sends the opposite of the pulse type it receives); 
it outputs to a.

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

After this sequence, 
the flip-flop modules all end up off, 
so pushing the button again repeats the same sequence.

Here's a more interesting example:

broadcaster -> a
%a -> inv, con
&inv -> b
%b -> con
&con -> output

This module configuration includes 
  the broadcaster, 
  two flip-flops (named a and b), 
  a single-input conjunction module (inv), 
  a multi-input conjunction module (con), 
  and an untyped module named output (for testing purposes). 
  
The multi-input conjunction module con watches the two flip-flop modules and, if they're both on, 
sends a low pulse to the output module.

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

However, now that both flip-flops are on and 
con remembers a high pulse from each of its two inputs, 
pushing the button a second time does something different:

button -low-> broadcaster
broadcaster -low-> a
a -low-> inv
a -low-> con
inv -high-> b
con -high-> output

Flip-flop a turns off! 
Now, con remembers a low pulse from module a, 
and so it sends only a high pulse to output.

Push the button a third time:

button -low-> broadcaster
broadcaster -low-> a
a -high-> inv
a -high-> con
inv -low-> b
con -low-> output
b -low-> con
con -high-> output

This time, flip-flop a turns on, 
then flip-flop b turns off. 
However, before b can turn off, the pulse sent to con is handled first, 
so it briefly remembers all high pulses for its inputs and sends a low pulse to output. 

After that, flip-flop b turns off, 
which causes con to update its state and send a high pulse to output.

Finally, with a on and b off, push the button a fourth time:

button -low-> broadcaster
broadcaster -low-> a
a -low-> inv
a -low-> con
inv -high-> b
con -high-> output

This completes the cycle: 
  a turns off, 
  causing con to remember only low pulses 
  and restoring all modules to their original states.

To get the cables warmed up, 
the Elves have pushed the button 1000 times. 

How many pulses got sent as a result (including the pulses sent by the button itself)?

In the first example, 
the same thing happens every time the button is pushed: 
8 low pulses and 4 high pulses are sent. 
So, after pushing the button 1000 times, 8000 low pulses and 4000 high pulses are sent. 
Multiplying these together gives 32000000.

In the second example, after pushing the button 1000 times, 
4250 low pulses and 2750 high pulses are sent. 
Multiplying these together gives 11687500.

Consult your module configuration; 
  determine the number of low pulses and high pulses that would be sent after pushing the button 1000 times, 
  waiting for all pulses to be fully handled after each push of the button. 
  
What do you get if you multiply the total number of low pulses sent by the total number of high pulses sent?

*/

/*
--- Part Two ---

The final machine responsible for moving the sand down to Island Island 
as a module attached named rx. 

The machine turns on when a single low pulse is sent to rx.

Reset all modules to their default states. 

Waiting for all pulses to be fully handled after each button press, 
  what is the fewest number of button presses required 
  to deliver a single low pulse to the module named rx?

*/


// Some overkill string manipulation helpers
// C++ gets wordy but at least it was fun exploring what is required to do this in C++
template <typename T, typename = void>
struct is_pair : std::false_type {};

template <typename T>
struct is_pair<T, std::void_t<decltype(std::declval<T>().first), decltype(std::declval<T>().second)>> : std::true_type {};

template <typename T>
std::string join(const T& container, std::string_view delimiter = " ") {
    std::ostringstream os;
    bool first = true;
    for (const auto& item : container) {
        if (!first) {
            os << delimiter;
        }
        if constexpr (is_pair<typename T::value_type>::value) {
            os << item.first << ": " << std::quoted(item.second);
        } else {
            os << std::quoted(item);
        }
        first = false;
    }
    return os.str();
}

class Module {
public:
    Module(std::string_view name, char type, std::vector<std::string_view> outputs)
        : name(name), type(type), outputs(outputs.begin(),outputs.end()) {
        if (this->type == '%') {
            memory["self"] = "off";
        }
    }

    operator std::string() const {
      // Requires g++-13 to link (clang building fails to link due to missing conversion functions...)      
      return std::format("{}{{type={},outputs={},memory={}}}", name, type, join(outputs,", "), join(memory,", "));
    }

    std::string name;
    char type;
    std::vector<std::string> outputs;
    std::map<std::string, std::string> memory;
};

struct Model {
  std::map<std::string,Module> modules{};
  std::vector<std::string> broadcast_targets{};
};

std::string_view strip(std::string_view line) {
  // Remove leading spaces
  while (!line.empty() && std::isspace(line.front())) {
      line.remove_prefix(1);
  }

  // Remove trailing spaces
  while (!line.empty() && std::isspace(line.back())) {
      line.remove_suffix(1);
  }
  return line;
}

// Python-like split
std::vector<std::string_view> split(std::string_view str, std::string_view delimiter = " ") {
    std::vector<std::string_view> result;
    size_t pos = 0;
    while (pos < str.size()) {
        size_t next_pos = str.find(delimiter, pos);
        if (next_pos == std::string_view::npos) {
            next_pos = str.size();
        }
        if (next_pos != pos || delimiter != " ") {
            result.push_back(str.substr(pos, next_pos - pos));
        }
        pos = next_pos + delimiter.size();
    }
    return result;
}

Model parse(auto& in) {
  Model result{};
  std::string line{};
  // parse
  while (std::getline(in,line)) {
    std::string_view line_view(line);
    line_view = strip(line_view); 
    auto left_right = split(line_view," -> ");
    auto left = left_right[0];
    auto right = left_right[1];
    auto outputs = split(right,", ");
    if (left == "broadcaster") {
      result.broadcast_targets = {outputs.begin(),outputs.end()};
    } else {
        auto type = left[0];
        std::string name{left.substr(1)};
      // modules[name] = Module(name, type, outputs)
      result.modules.emplace(name,Module(name, type, outputs));
    }
  }
  // wire all modules together
  for (auto& [name,module] : result.modules) {
    for (auto& output : module.outputs) {
      if (result.modules.contains(output) && result.modules.at(output).type == '&') {
        result.modules.at(output).memory[name] = "lo";
      }
    }
  }
  return result;
}

std::ostream& operator<<(std::ostream& os, Module const& module) {
  os << static_cast<std::string>(module);
  return os;
}

void print_model(Model const& model) {
  std::cout << NL << "Model";
  std::cout << NT << "broadcast_targets : " << join(model.broadcast_targets,", ");
  for (auto const& [name,module] : model.modules) {
    std::cout << NT <<  module;
  }
}

namespace part1 {
  // based on https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day20p1.py
  // Thanks!
  namespace hyperneutrino {
    
    Result count(Model& model,int pushes) {
      Result result{};
      auto& [modules,broadcast_targets] = model;
      std::cout << NL << "hyperneutrino::count";
      // lo = hi = 0
      Result lo{},hi{};

      // for _ in range(1000):
      for (auto _ = 0;_<pushes;++_) {
        // lo += 1
        ++lo; // the "push"
        // q = deque([("broadcaster", x, "lo") for x in broadcast_targets])
        std::deque<std::tuple<std::string,std::string,std::string>> q{};
        for (auto const& x : broadcast_targets) {
          q.push_back({ "broadcaster",x,"lo" });
        }
        // while q:
        while (!q.empty()) {
          auto [origin,target,pulse] = q.front();
          q.pop_front();
          // std::cout << NL << std::quoted(origin) << " -" << pulse << "-> " << std::quoted(target);
          if (pulse == "lo") {
            ++lo;
          } else {
            ++hi;
          }
          // if target not in modules:
          if (!modules.contains(target)) {
            // std::cout <<  " not in modules";
            continue;
          }
          auto& module = modules.at(target);
          if (module.type == '%') {
            if (pulse == "lo") {
              module.memory["self"] = (module.memory.at("self") == "off" ? "on" : "off");
              auto outgoing = module.memory.at("self") == "on" ? "hi" : "lo";
              // std::cout << NT << module << " -" << outgoing << "-> " << join(module.outputs,", ");
              
              for (auto const& x : module.outputs) {
                q.push_back({ module.name,x,outgoing });
              }
            }
          } else {
            module.memory[origin] = pulse;
            auto outgoing = std::all_of(module.memory.begin(),module.memory.end(),[](auto const& x){ return x.second == "hi"; }) ? "lo" : "hi";
            // std::cout << NT << module << " -" << outgoing << "-> " << join(module.outputs,", ");
            for (auto const& x : module.outputs) {
              q.push_back({ module.name,x,outgoing });
            }
          }
        }
      }
      std::cout << NL << "hyperneutrino::count lo : " << lo << " hi : " << hi << " lo * hi : " << lo * hi;
      result = lo * hi;
      // print(lo * hi)
      return result; // 763500168
    } // count
  } // namespace hyperneutrino
  Result solve_for(Model& model,auto args) {
    Result result{};
    auto const& [part,file,pushes] = args;
    std::cout << NL << NL << "part1::solve_for(" << file << "," << pushes << ")";
    print_model(model);
    result = hyperneutrino::count(model,pushes);
    return result; // 763500168
  }
} // namespace part1

namespace part2 {
  using Tree = std::map<std::string,std::vector<std::string>>;
  using Edge = std::tuple<std::string,std::string,std::string>;
  Tree bfs(const std::map<std::string, Module>& modules, const std::string& root) {
    Tree result{};
    std::queue<std::string> to_process;  // Queue of modules to process
    std::set<std::string> seen;  // Set of seen module names

    // Start with the root module
    to_process.push(root);
    seen.insert(root);

    while (!to_process.empty()) {
      std::string current = to_process.front();
      to_process.pop();

      // find all (name.module) with an output to current
      for (auto const& [name,module] : modules) {
        if (std::find(module.outputs.begin(), module.outputs.end(), current) != module.outputs.end()) {
          result[current].push_back(name); // current has an input from module "name"
          if (!seen.contains(name)) {
              to_process.push(name); // backtrack from name
              seen.insert(name);
          }            
        }
      }
    }
    return result;
  }
  void print_tree(Tree const& tree) {
    std::cout << NL << "Tree";
    for (auto const& [name,outputs] : tree) {
      std::cout << NT << std::quoted(name) << " -> " << join(outputs,", ");
    }
  }
  // From python solution https://github.com/hyper-neutrino/advent-of-code/blob/main/2023/day20p2.py
  // Thanks!
  namespace hyperneutrino {
    Result count_to_low_to_rx(Model& model) {
      auto& [modules,broadcast_targets] = model;
      /*
      (feed,) = [name for name, module in modules.items() if "rx" in module.outputs]
      */      
      std::string feed;
      for (const auto& [name, module] : modules) {
          if (std::find(module.outputs.begin(), module.outputs.end(), "rx") != module.outputs.end()) {
            std::cout << NL << "module " << name << " meta: " << module << " has an output to 'rx'";
            feed = name;
            break;
          }
          else {
            std::cout << NL << "module " << module << " has no output to 'rx'";
          }
      }
      std::cout << NL << "'rx' is feed from " << feed;

      std::map<std::string, Integer> cycle_lengths{};
      // make seen a map from all module names that in turn is connected to feed to a sufficient large integer
      std::map<std::string, Integer> seen;
      for (const auto& [name, module] : modules) {
          if (std::find(module.outputs.begin(), module.outputs.end(), feed) != module.outputs.end()) {
            std::cout << NT << "detect cycles between low from " << name << " to " << feed;
            seen[name] = 0;
          }
      }
      Integer presses{};
      while (true) {
        ++presses;
        // put the signals emitted by broadcaster on push into a queue
        std::deque<std::tuple<std::string,std::string,std::string>> q{};
        for (auto const& x : broadcast_targets) {
          q.push_back({ "broadcaster",x,"lo" });
        }

        while (!q.empty()) {
          auto [origin,target,pulse] = q.front();
          q.pop_front();
          if (!modules.contains(target)) continue; // skip unexisting modules
          auto& module = modules.at(target);
          if (target == feed && pulse == "hi") {
            ++seen[origin];
            std::cout << NT << "seen[" << origin << "] = " << seen[origin];
            if (!cycle_lengths.contains(origin)) {
              cycle_lengths[origin] = presses;
            } else {
              if (presses != seen[origin] * cycle_lengths[origin]) {
                throw std::runtime_error("Assertion failed: presses != seen[origin] * cycle_lengths[origin]");
              }
            }
            if (std::all_of(seen.begin(), seen.end(), [](const auto& pair) { return pair.second > 0; })) {
                // All values in the seen map are truthy
                Result x{1};
                for (const auto& [name, cycle_length] : cycle_lengths) {
                  std::cout << NT << "cycle_length[" << name << "] = " << cycle_length << " and " << x;
                  x = std::lcm(x, cycle_length);
                  std::cout << " lcm: " << x;
                }
                return x;
            }        
          }
          // engage the modules signalling
          if (module.type == '%') {
            if (pulse == "lo") {
              module.memory["self"] = (module.memory.at("self") == "off" ? "on" : "off");
              auto outgoing = module.memory.at("self") == "on" ? "hi" : "lo";
              // std::cout << NT << module << " -" << outgoing << "-> " << join(module.outputs,", ");
              
              for (auto const& x : module.outputs) {
                q.push_back({ module.name,x,outgoing });
              }
            }
          } else {
            module.memory[origin] = pulse;
            auto outgoing = std::all_of(module.memory.begin(),module.memory.end(),[](auto const& x){ return x.second == "hi"; }) ? "lo" : "hi";
            // std::cout << NT << module << " -" << outgoing << "-> " << join(module.outputs,", ");
            for (auto const& x : module.outputs) {
              q.push_back({ module.name,x,outgoing });
            }
          }
        } // while q
      } // while
      return -1; // failed
    }
  }
  Result solve_for(Model& model,auto args) {
    Result result{};
    auto const& [part,file,pushes] = args;
    std::cout << NL << NL << "part2::solve_for(" << file << "," << pushes << ")";
    // These functions don't work yet...
    // auto to_rx_tree = bfs(model.modules,"rx");
    // print_tree(to_rx_tree);
    hyperneutrino::count_to_low_to_rx(model);
    return result;
  }
}

int main(int argc, char *argv[])
{
  Solution solution{};
  std::cout << NL << "argc : " << argc;
  for (int i = 0; i < argc; ++i) {
    std::cout << NL << "argv[" << i << "] : " << std::quoted(argv[i]);
  }
  // day20 part file ste
  std::tuple<int,std::string,int> args{1,"example.txt",6};
  auto& [part,file,pushes] = args;
  if (argc > 1 ) {
    part = std::stoi(argv[1]);
    if (argc > 2) {
      file = argv[2];
    }
    if (argc > 3) {
      pushes = std::stoi(argv[3]);
    }
  }
  std::cout << NL << "Part : " << part << " file : " << file;
  std::ifstream in{ file };
  auto model = parse(in);

  switch (part) {
  case 1: {
    auto answer = part1::solve_for(model,args);
    solution[part].push_back({ file,answer });
  } break;
  case 2: {
    auto answer = part2::solve_for(model,args);
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