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

namespace part1 {
  /*
  So, todays puzzle is a circular graph problem. We are to find this loop in the graph.
  And then the answer we are looking for is where the half way point is in the loop.

  Our input seems to define edges? So the nodes themselves are implicit.

  First we need to represent the inout data as a graph.

  Then we can split this graph into its connected components.

  one of these connected components is a loop so we need to find that one.

  Now the problem is "just" to find the length of this loop and half it to get our answer.

  */

  class Graph {
  private:
    int V; // number of vertices
    std::map<int,std::set<int>> adj{}; // adjacency list

  public:
    class Edge {
    public:
      int src; /**< The source vertex of the edge. */
      int dest; /**< The destination vertex of the edge. */

      /**
       * @brief Constructs an Edge object with the given source and destination vertices.
       * @param src The source vertex of the edge.
       * @param dest The destination vertex of the edge.
       */
      Edge(int src, int dest) : src(src), dest(dest) {}
    };

    using Edges = std::vector<Edge>;

    void addEdge(int v, int w) {
      adj[v].insert(w);
      adj[w].insert(v);
    }

    void addEdge(const Edge& edge) {
      std::cout << NL << "addEdge" << std::flush;
      std::cout << " " << edge.src << " -> " << edge.dest << std::flush;
      addEdge(edge.src, edge.dest);
    }

    std::set<int> const& getAdjacentVertices(int v) const {
      return adj.at(v);
    }

    int getVertexCount() const {
      return adj.size();
    }
  };

  class CC {
  private:
    std::vector<bool> marked; // marked[v] = has vertex v been marked?
    std::vector<int> id; // id[v] = id of connected component containing v
    std::vector<int> size; // size[id] = number of vertices in component id
    int count; // number of connected components

  public:
    CC(const Graph& graph) {
      marked.resize(graph.getVertexCount(), false);
      id.resize(graph.getVertexCount());
      size.resize(graph.getVertexCount());
      count = 0;

      for (int v = 0; v < graph.getVertexCount(); ++v) {
        if (!marked[v]) {
          dfs(graph, v);
          ++count;
        }
      }
    }

    bool connected(int v, int w) const {
      return id[v] == id[w];
    }

    int getCount() const {
      return count;
    }

    int getId(int v) const {
      return id[v];
    }

    int getSize(int id) const {
      return size[id];
    }

  private:
    void dfs(const Graph& graph, int v) {
      marked[v] = true;
      id[v] = count;
      ++size[count];

      for (int w : graph.getAdjacentVertices(v)) {
        if (!marked[w]) {
          dfs(graph, w);
        }
      }
    }
  };

  class Cycle {
  private:
    std::vector<bool> marked{}; // marked[v] = has vertex v been visited?
    bool has_cycle{false}; // does the graph have a cycle?

  public:
    Cycle(const Graph& graph) {
      marked.resize(graph.getVertexCount(), false);

      for (int v = 0; v < graph.getVertexCount(); ++v) {
        if (!marked[v]) {
          dfs(graph, v, v);
        }
      }
    }

    bool hasCycle() const {
      return has_cycle;
    }

  private:
    void dfs(const Graph& graph, int v, int u) {
      marked[v] = true;
      for (int w : graph.getAdjacentVertices(v)) {
        if (!marked[w]) {
          dfs(graph, w, v);
        } else if (w != u) {
          has_cycle = true;
        }
      }
    }
  };

class Paths {
private:
  std::vector<bool> marked; // marked[v] = is there a path from source to v?
  std::vector<int> edgeTo; // edgeTo[v] = last edge on path from source to v
  int source; // source vertex

public:
  Paths(const Graph& graph, int source) : marked(graph.getVertexCount(), false), edgeTo(graph.getVertexCount()), source(source) {
    dfs(graph, source);
  }

  bool hasPathTo(int v) const {
    return marked[v];
  }

  std::vector<int> pathTo(int v) const {
    std::cout << NL << "pathTo" << std::flush;
    if (!hasPathTo(v)) {
      return {}; // No path exists
    }

    std::vector<int> path;
    for (int x = v; x != source; x = edgeTo[x]) {
      path.push_back(x);
    }
    path.push_back(source);
    std::reverse(path.begin(), path.end());
    return path;
  }

private:
  void dfs(const Graph& graph, int v) {
    marked[v] = true;
    for (int w : graph.getAdjacentVertices(v)) {
      if (!marked[w]) {
        edgeTo[w] = v;
        dfs(graph, w);
      }
    }
  }
};

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

  using MapEdge = std::pair<Position, Delta>;
  using MapEdges = std::vector<MapEdge>;

  const Delta EAST(1, 0);
  const Delta WEST(-1, 0);
  const Delta SOUTH(0, 1);
  const Delta NORTH(0, -1);

  MapEdges to_map_edges(Position pos, char symbol) {
    std::cout << NL << "to_map_edges" << std::flush;
    MapEdges result{};
    // Define an edge as going in a direction from the position of the symbol.
    // So 'L' defines one edge to north of pos and one edge east of pos.
    switch (symbol) {
      case '|':
        result.push_back({ pos, NORTH });
        result.push_back({ pos, SOUTH });
        break;
      case '-':
        result.push_back({ pos, EAST });
        result.push_back({ pos, WEST });
        break;
      case 'L':
        result.push_back({ pos, NORTH });
        result.push_back({ pos, EAST });
        break;
      case 'J':
        result.push_back({ pos, WEST});
        result.push_back({ pos, NORTH });
        break;
      case '7':
        result.push_back({ pos, WEST});
        result.push_back({ pos, SOUTH });
        break;
      case 'F':
        result.push_back({ pos, SOUTH });
        result.push_back({ pos, EAST});
        break;
      case '.':
        break;
      default:
        break;
    }
    return result;
  }

  int to_vertex(Position const& pos,Positions& positions) {
    std::cout << NL << "to_vertex"  << std::flush;
    auto iter = std::find(positions.begin(), positions.end(), pos);
    if (iter == positions.end()) {
      std::cout << NL << "new position" << std::flush;
      positions.push_back(pos);
      return positions.size() - 1; // return index of new position
    }
    std::cout << NL << "existing position" << std::flush;
    return std::distance(positions.begin(), iter);
  }

  int to_vertex(Position const& pos,Positions const& positions) {
    std::cout << NL << "to_vertex"  << std::flush;
    auto iter = std::find(positions.begin(), positions.end(), pos);
    if (iter == positions.end()) {
      throw std::runtime_error("vertex not found");
    }
    std::cout << NL << "existing position" << std::flush;
    return std::distance(positions.begin(), iter);
  }

  Position to_position(int vertex, Positions const& positions) {
    if (vertex < positions.size()) return positions[vertex];
    throw std::runtime_error("vertex out of range");
  }

  std::pair<Graph,Positions> to_graph_and_positions(Model const& model) {
    std::cout << NL << "to_graph";
    Graph graph{};
    Positions positions{};
    for (int y = 0; y < model.size(); y++) {
      for (int x = 0; x < model[y].size(); x++) {
        Position pos(x, y); // Create position from row and col
        std::cout << NL << "pos (" << pos.x << "," << pos.y << ")" << std::flush;
        auto symbol = model[y][x];
        auto map_edges = to_map_edges(pos,symbol);
        for (auto const& [pos,delta] : map_edges) {
          if (pos.x > 0 and pos.y > 0) {
            std::cout << NL << "map_edge (" << pos.x << "," << pos.y << ") (" << delta.delta_x << "," << delta.delta_y << ")" << std::flush;
            auto src = pos;
            auto dest = Position(src.x + delta.delta_x, src.y + delta.delta_y);
            auto edge = Graph::Edge(to_vertex(src,positions), to_vertex(dest,positions));
            graph.addEdge(edge); 
          }          
        }
      }
    }
    return {graph,positions};
  }

  void print_graph(Graph const& graph,Positions const& positions) {
    std::cout << NL << "Graph:";
    for (int v = 0; v < graph.getVertexCount(); v++) {
      auto pos = to_position(v,positions);
      std::cout << NL << "Vertex " << v << " position : (col:" << pos.x << ",row:" << pos.y << ")";
      auto adjacent_vertices = graph.getAdjacentVertices(v);
      for (auto const& adjacent_vertex : adjacent_vertices) {
        auto adjacent_pos = to_position(adjacent_vertex,positions);
        std::cout << NT << "Adjacent vertex " << adjacent_vertex << " (" << adjacent_pos.x << "," << adjacent_pos.y << ")";
      }
    }
  }

  int dfs(Graph const& graph, int current_vertex, int start_vertex, std::vector<bool>& visited) {
    visited[current_vertex] = true;
    int max_steps = 0;
    auto adjacent_vertices = graph.getAdjacentVertices(current_vertex);
    for (auto const& adjacent_vertex : adjacent_vertices) {
      if (!visited[adjacent_vertex]) {
        int steps = dfs(graph, adjacent_vertex, start_vertex, visited);
        max_steps = std::max(max_steps, steps + 1);
      } else if (adjacent_vertex == start_vertex) {
        max_steps = std::max(max_steps, 1);
      }
    }
    visited[current_vertex] = false;
    return max_steps;
  }  

  Result solve_for(Model& model) {
    Result result{};
    std::cout << NL << "Solver for part 1";
    auto start = Position(0, 0);
    int width = model[0].size();
    int height = model.size();
    // Find the start vertex
    // NOTE: For my input the start vertex is (0,0)!
    // But in the examples it is a bit more complicated...
    // Anyhow...
    bool found_start = false;
    for (int distance = 0; distance < width + height;++distance and !found_start) {
      for (int x = 0; x <= std::min(width,distance) and !found_start; ++x) {
        int y = std::min(height,std::abs(distance - x));
        if (model[y][x] == 'S') {
          model[y][x] = 'F';
          // repair example map
        }
        if (model[y][x] == 'F') {
          std::cout << NL << "Found start vertex at (" << x << "," << y << ")" << std::flush;
          start = Position(x, y);
          found_start = true;
        }
      }
    }
    // transform the model into a graph
    auto const& [graph, positions] = to_graph_and_positions(model);
    print_graph(graph, positions);
    std::vector<bool> visited(graph.getVertexCount(), false);
    std::cout << NL << "graph has cycle" << std::flush;
    try {
      int max_steps = dfs(graph, 0, 0, visited); // max steps to loop
      std::cout << NL << "max steps : " << max_steps << std::flush;
      result = max_steps / 2;
    } catch (std::runtime_error const& e) {
      std::cout << NL << "runtime error : " << e.what() << std::flush;
    }
    return result;
  }
}

namespace part2 {
  Result solve_for(Model& model) {
      Result result{};
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