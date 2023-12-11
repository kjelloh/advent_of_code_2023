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
#include <coroutine>
#include <format>
#include <optional>
#include <regex>

char const* example = R"(.....
.F-7.
.|.|.
.L-J.
.....)";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int64_t; // 16 bit int: � 3.27 � 10^4, 32 bit int: � 2.14 � 10^9, 64 bit int: � 9.22 � 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
struct Solution {
  Answers part1{};
  Answers part2{};
};

using Model = std::vector<std::string>;

Model parse(std::istream& in) {
  Model result{};
  std::string line{};
  while (std::getline(in, line)) {
    result.push_back(line);
  }
  return result;
}

void printModel(Model const& model) {
  std::cout << NL << "Model:";
  for (auto const& line : model) {
    std::cout << NL << line;
  }
}

class Delta {
public:
  int delta_x; /**< The change in x-coordinate representing east/west. */
  int delta_y; /**< The change in y-coordinate representing south/north. */

  /**
   * @brief Constructs a Delta object with the given change in x and y coordinates.
   * @param delta_x The change in x-coordinate representing east/west.
   * @param delta_y The change in y-coordinate representing south/north.
   */
  Delta(int delta_x, int delta_y) : delta_x(delta_x), delta_y(delta_y) {}

  bool operator==(const Delta& other) const {
    return delta_x == other.delta_x && delta_y == other.delta_y;
  }

  bool operator<(const Delta& other) const {
    return delta_x < other.delta_x || (delta_x == other.delta_x && delta_y < other.delta_y);
  }
};

class Position {
public:
  int x; /**< The x-coordinate representing east. */
  int y; /**< The y-coordinate representing south. */

  /**
   * @brief Constructs a Position object with the given x and y coordinates.
   * @param x The x-coordinate representing east.
   * @param y The y-coordinate representing south.
   */
  Position(int x, int y) : x(x), y(y) {}

  /**
   * @brief Equality comparison operator for Position objects.
   * @param other The Position object to compare with.
   * @return True if the Position objects are equal, false otherwise.
   */
  bool operator==(const Position& other) const {
    return x == other.x && y == other.y;
  }

  /**
   * @brief Addition operator to add a Delta to a Position.
   * @param delta The Delta to add.
   * @return The resulting Position after adding the Delta.
   */
  Position operator+(const Delta& delta) const {
    return Position(x + delta.delta_x, y + delta.delta_y);
  }

  
};

using Positions = std::vector<Position>;

const Delta EAST(1, 0);
const Delta WEST(-1, 0);
const Delta SOUTH(0, 1);
const Delta NORTH(0, -1);

struct Walker {
  Walker(Position const& position, Delta const& direction, bool on_route = true) 
    : position{ position }, direction{ direction }, on_route{ on_route } {}
  Result step_count{ 0 };
  Result operator++() {
    ++step_count;
    position = position + direction;
    return step_count;
  }
  Position position;
  Delta direction;
  bool on_route{true};
  operator bool() const { return on_route; }
};

/*
  The pipes are arranged in a two-dimensional grid of tiles:

  | is a vertical pipe connecting north and south.
  - is a horizontal pipe connecting east and west.
  L is a 90-degree bend connecting north and east.
  J is a 90-degree bend connecting north and west.
  7 is a 90-degree bend connecting south and west.
  F is a 90-degree bend connecting south and east.
  . is ground; there is no pipe in this tile.
  S is the starting position of the animal; there is a pipe on this tile, but your sketch doesn't show what shape the pipe has.
*/
const std::string SYMBOLS{"|-LJ7FS."};

Walker init_walker(Position const& position, char symbol) {
  switch (symbol) {
  case '|':
    return Walker{ position, NORTH };
    break;
  case '-':
    return Walker{ position, EAST };
    break;
  case 'L':
    return Walker{ position, EAST };
    break;
  case 'J':
    return Walker{ position, WEST };
    break;
  case '7':
    return Walker{ position, SOUTH };
    break;
  case 'F':     
    return Walker{ position, EAST };
    break;
  default:
    std::cout << NL << "Unknown symbol: " << symbol;
    return Walker{ position, EAST ,false};
    break;
  }
}

Walker aligned(Walker const& walker, char symbol) {
  Walker result{ walker };
  switch (symbol) {
  case '|':
    if (walker.direction == NORTH || walker.direction == SOUTH) {
      result.on_route = true;
    }
    break;
  case '-':
    if (walker.direction == EAST || walker.direction == WEST) {
      result.on_route = true;
    }
    break;
  case 'L':
    if (walker.direction == SOUTH) {
      result.direction = EAST;
      result.on_route = true;
    }
    else if (walker.direction == WEST) {
      result.direction = NORTH;
      result.on_route = true;
    }
    break;
  case 'J':
    if (walker.direction == SOUTH) {
      result.direction = WEST;
      result.on_route = true;
    }
    else if (walker.direction == EAST) {
      result.direction = NORTH;
      result.on_route = true;
    }
    break;
  case '7':
    if (walker.direction == NORTH) {
      result.direction = WEST;
      result.on_route = true;
    }
    else if (walker.direction == EAST) {
      result.direction = SOUTH;
      result.on_route = true;
    }
    break;
  case 'F':
    if (walker.direction == NORTH) {
      result.direction = EAST;
      result.on_route = true;
    }
    else if (walker.direction == WEST) {
      result.direction = SOUTH;
      result.on_route = true;
    }
    break;
  case '.':
    break;
  default:
    std::cout << NL << "Unknown symbol: " << symbol;
    break;
  }
  if (!result.on_route) std::cout << NL << "Walk invalid at pipe " << symbol << " at position " << walker.position.x << "," << walker.position.y << " with direction " << walker.direction.delta_x << "," << walker.direction.delta_y << " and symbol " << symbol;
  return result;
}

bool on_map(Model const& model, Position const& position) {
  return position.y >= 0 && position.y < model.size() && position.x >= 0 && position.x < model[position.y].size();
}

std::vector<char> to_pipe_candidates(Position const& position, Model const& model) {
  std::cout << NL << "to_pipe_candidates for " << position.x << "," << position.y;
  std::string const NORTH_CONNECTABLE = { '|','7','F'};
  std::string const SOUTH_CONNECTABLE = { '|','L','J'};
  std::string const WEST_CONNECTABLE = { '-','L','F'};
  std::string const EAST_CONNECTABLE = { '-','7','J'};
  std::vector<char> result{};
  std::map<Delta, bool> connectable{};
  for (auto const& delta : { NORTH, EAST, SOUTH, WEST }) {
    Position candidate = position + delta;
    if (on_map(model, candidate)) {
      char symbol = model[candidate.y][candidate.x];
      connectable[delta] = 
            (delta == NORTH and std::any_of(NORTH_CONNECTABLE.begin(), NORTH_CONNECTABLE.end(), [symbol](char c) {return c == symbol; }))
        or (delta == SOUTH and std::any_of(SOUTH_CONNECTABLE.begin(), SOUTH_CONNECTABLE.end(), [symbol](char c) {return c == symbol; }))
        or (delta == WEST and std::any_of(WEST_CONNECTABLE.begin(), WEST_CONNECTABLE.end(), [symbol](char c) {return c == symbol; }))
        or (delta == EAST and std::any_of(EAST_CONNECTABLE.begin(), EAST_CONNECTABLE.end(), [symbol](char c) {return c == symbol; }));
    }
  }
  for (int i=0;i<connectable.size();++i) {
    for (int j=i;j<connectable.size();++j) {
      auto [delta1,flag1] = *std::next(connectable.begin(), i);
      auto [delta2, flag2] = *std::next(connectable.begin(), j);
      if (flag1 and flag2) {
              if (delta1 == NORTH and delta2 == EAST) result.push_back('L');
        else if (delta1 == NORTH and delta2 == WEST) result.push_back('J');
        else if (delta1 == NORTH and delta2== SOUTH) result.push_back('|');
        else if (delta1 == SOUTH and delta2 == EAST) result.push_back('F');
        else if (delta1 == SOUTH and delta2 == WEST) result.push_back('7');
        else if (delta1 == SOUTH and delta2 == NORTH) result.push_back('|');
        else if (delta1 == EAST and delta2 == WEST) result.push_back('-');          
        else if (delta1 == EAST and delta2 == NORTH) result.push_back('L');
        else if (delta1 == EAST and delta2 == SOUTH) result.push_back('F');
        else if (delta1 == WEST and delta2 == NORTH) result.push_back('J');
        else if (delta1 == WEST and delta2 == SOUTH) result.push_back('7');
        else if (delta1 == WEST and delta2 == EAST) result.push_back('-');
      }
    }
  }
  for (auto candidate : result) std::cout << NT << "Candidate: " << candidate;
  return result;
}

namespace part1 {

  Result solve_for(Model& model) {
    Result result{};
    std::cout << NL << "Solver for part 1";

    for (int y = 0; y < model.size() and result==0; ++y) {
      for (int x = 0; x < model[y].size() and result==0; ++x) {
        if (model[y][x] == 'S') {          
          Position start_position{ x,y };
          // Fix the pipe with each candidate
          auto candidates = to_pipe_candidates(start_position, model);
          for (auto const& candidate : candidates) {
            std::cout << NL << "Candidate for S is " << candidate << std::flush;
            Walker walker = init_walker(start_position, candidate);
            do {
              std::cout << NL << "walker: " << walker.position.x << "," << walker.position.y << " with direction " << walker.direction.delta_x << "," << walker.direction.delta_y << std::flush;
              ++walker;
              walker.on_route = on_map(model, walker.position);
              char symbol = model[walker.position.y][walker.position.x];
              walker = aligned(walker, symbol);
            } while (walker.on_route && walker.position != start_position);
            if (walker.on_route) {
              std::cout << NL << "Found loop at " << walker.position.x << "," << walker.position.y;
              result = std::max(result,walker.step_count);
              break;
            }
            else {
              std::cout << NL << "No loop found";
            }
          }
        }
      }
    }
    if (result % 2 == 0 ) {
      std::cout << NL << "Step count is even: " << result;
      result = result / 2;
    }
    else {
      std::cout << NL << "Result is odd: " << result;
      result = (result -1) / 2;
    }
    return result; // 13514 / 2 = 6757 ok 
  }
}

namespace part2 {
  Result solve_for(Model& model) {
      Result result{};
      /*
      Just for "fun" - could it be that we can find the enclosed area in the following way:
      1) mark the loop with some unique symbol
      2) scan the map and mark all the positions before the enclosure with some "pre-enclosure" symbol and the another symbol for the positions after the enclosure
      3) Do the same by scanning each column.

      Now "simply" count the number of post-enclosure symbols in each row and sum (integrate) them up?
      */
    std::cout << NL << "Solver for part 2";

    std::vector<Position> path{};
    for (int y = 0; y < model.size() and result==0; ++y) {
      for (int x = 0; x < model[y].size() and result==0; ++x) {
        if (model[y][x] == 'S') {          
          Position start_position{ x,y };
          // Fix the pipe with each candidate
          auto candidates = to_pipe_candidates(start_position, model);
          for (auto const& candidate : candidates) {
            path.clear(); // Works as long as we have only one loop
            std::cout << NL << "Candidate for S is " << candidate << std::flush;
            // mend the map so that we can analyze it later for enclosed tiles
            model[start_position.y][start_position.x] = candidate;
            Walker walker = init_walker(start_position, candidate);
            path.push_back(walker.position);
            do {
              std::cout << NL << "walker: " << walker.position.x << "," << walker.position.y << " with direction " << walker.direction.delta_x << "," << walker.direction.delta_y << std::flush;
              ++walker;
              walker.on_route = on_map(model, walker.position);
              char symbol = model[walker.position.y][walker.position.x];
              walker = aligned(walker, symbol);
              path.push_back(walker.position);
            } while (walker.on_route && walker.position != start_position);
            if (walker.on_route) {
              std::cout << NL << "Found loop at " << walker.position.x << "," << walker.position.y;
              break;
            }
            else {
              std::cout << NL << "No loop found";
            }
          }
        }
      }
    }

    // Create a map with only the found loop path on it.
    Model enclosure_model(model.size(),std::string(model[0].size(),'.'));
    for (auto const& position : path) {
      enclosure_model[position.y][position.x] = model[position.y][position.x];
    }
    printModel(enclosure_model);
    // We can now detect the tiles that are inside the loop in at least two ways.
    // 1) Walk the path, say clockwise, and mark each ground tile to the right of us as "inside"
    //    Then we can go over the map in a second pass and flood fill all found inside tiles :)
    //    (I suppose we could do the flood filling as we go along as well?)
    // 2) Use a ray cast algorithm to detect the inside tiles. We can scan the map rows and keep count of crossings of the path.
    //    Each crossing flips the inside/outside state. The state of a tile will then be fully determined based on the ra cast passing the tile

    // Lets say we ray cast each row. On our way from left to right we start at state "outside",
    // Each time we cross a '|' we flip the state.
    // Problem is we may find we trace along sections of the path itself (like "...L-----7.."). Is this to count as a "crossing" or not?
    // Well, it is a crossing if the "corners" represent a crossing (in this example L can be seen as going "up" and '7' as going "down")
    // This means it is a crossing. While "...L----J..." is not a crossing (both corners goes "up" so our ray never goes "inside".)
    for (int row = 0; row < enclosure_model.size(); ++row) {
      std::cout << NL << "row[" << row << "]";
      std::cout << NT << std::quoted(model[row]);
      // transform the row into crossing count symbols '+','-','2','0' (increment, decrement, +2)
      std::string crossing_ops{};
      std::transform(enclosure_model[row].begin(), enclosure_model[row].end(), std::back_inserter(crossing_ops), [](char c) {
        std::map<char,char> ops{
          {'|','2'}, // confirmed crossing (same as ++)
          {'L','+'}, // going down
          {'J','-'}, // going up , L---J cancels out with +-, F--J confirms with --
          {'7','+'}, // going down, L---7 conforms with ++, L--7 cancels out with -+
          {'F','-'}, // going up, F--J confirms with --, F--7 cancels out with -+
        };
        char result = ops.contains(c) ? ops[c] : '0';
        std::cout << NT << c << " => " << result;
        return result;
        });
      std::cout << NT << std::quoted(model[row]);
      std::cout << NT <<  std::quoted(crossing_ops);
      // transform the ops into confirmed crossings (|), possible crossings (?) and no crossings ( )
      int crossings{ 0 };
      std::string col_states(enclosure_model[row].size(), '0'); // a mod 3 thingy (0 = no cross,1 maybe cross, 2 cross confirmed)
      for (int col = 0;col<crossing_ops.size();++col) {
        auto crossing_op = crossing_ops[col];
        if (crossing_op == '+') ++crossings;
        else if (crossing_op == '-') --crossings;
        else if (crossing_op == '2') crossings = 2;
        std::cout << NT << " op " << crossing_op << " crossings: " << crossings;
        switch (crossings) {
        case 0:
          col_states[col] = ' ';
          break;
        case 1:
        case -1:
          col_states[col] = '?';
          break;
        case 2:
        case -2:
          col_states[col] = '|';
          break;
        } 
        crossings = crossings % 2; // Keep only maybe and no cross state
      }
      std::cout << NT << std::quoted(col_states);
      // transform the col_states into inside/outside states (I/O)
      std::string insides{col_states};
      bool inside{ false };
      for (int col = 0; col < col_states.size(); ++col) {
        inside ^= (col_states[col] == '|'); // flip on confirmed crossing
        insides[col] = inside and enclosure_model[row][col] == '.' ? 'I' : 'O';
      }
      std::cout << NT << std::quoted(insides);
      int count{};
      for (auto const& c : insides) if (c == 'I') ++count;
      result += count;
      std::cout << NT <<  std::quoted(insides) << " contains " << count << " inside tiles. result is now " << result;
    }

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
    printModel(model);
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
        printModel(model);
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