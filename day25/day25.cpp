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
#include <chrono>
#include <random>

char const* example = R"(jqt: rhn xhk nvd
rsh: frs pzl lsr
xhk: hfx
cmg: qnr nvd lhk bvb
rhn: xhk bvb hfx
bvb: xhk hfx
pzl: lsr hfx nvd
qnr: nvd
ntq: jqt hfx bvb xhk
nvd: lhk
lsr: lhk
rzs: qnr cmg lsr rsh
frs: qnr lhk lsr)";

auto const NL = "\n";
auto const T = "\t";
auto const NT = "\n\t";

using Integer = int64_t; // 16 bit int: 3.27 x 10^4, 32 bit int: 2.14 x 10^9, 64 bit int: 9.22 x 10^18
using Result = Integer;
using Answer = std::pair<std::string, Result>;
using Answers = std::vector<Answer>;
using Solution = std::map<int, Answers>; // Puzzle part -> Answers

/*

--- Day 25: Snowverload ---

Still somehow without snow, 
you go to the last place you haven't checked: 
the center of Snow Island, directly below the waterfall.

Here, someone has clearly been trying to fix the problem. 
Scattered everywhere are hundreds of weather machines, almanacs, communication modules, 
hoof prints, machine parts, mirrors, lenses, and so on.

Somehow, everything has been wired together into a massive snow-producing apparatus, 
but nothing seems to be running. 
You check a tiny screen on one of the communication modules: 

Error 2023. 

It doesn't say what Error 2023 means, 
but it does have the phone number for a support line printed on it.

"Hi, you've reached Weather Machines And So On, Inc. How can I help you?" 

You explain the situation.

"Error 2023, you say? Why, that's a power overload error, of course! 
It means you have too many components plugged in. 
Try unplugging some components and--" 
You explain that there are hundreds of components here and you're in a bit of a hurry.

"Well, let's see how bad it is; 
do you see a big red reset button somewhere? 
It should be on its own module. 
If you push it, it probably won't fix anything, 
but it'll report how overloaded things are." 
After a minute or two, you find the reset button; 
it's so big that it takes two hands just to get enough leverage to push it. 
Its screen then displays:

SYSTEM OVERLOAD!

Connected components would require
power equal to at least 100 stars!
"Wait, how many components did you say are plugged in? 
With that much equipment, you could produce snow for an entire--" 
You disconnect the call.

You have nowhere near that many stars 
- you need to find a way to disconnect at least half of the equipment here, 
but it's already Christmas! 

You only have time to disconnect three wires.

Fortunately, someone left a wiring diagram (your puzzle input) 
that shows how the components are connected. For example:

jqt: rhn xhk nvd
rsh: frs pzl lsr
xhk: hfx
cmg: qnr nvd lhk bvb
rhn: xhk bvb hfx
bvb: xhk hfx
pzl: lsr hfx nvd
qnr: nvd
ntq: jqt hfx bvb xhk
nvd: lhk
lsr: lhk
rzs: qnr cmg lsr rsh
frs: qnr lhk lsr

Each line shows the name of a component, 
a colon, and then a list of other components to which that component is connected. 
Connections aren't directional; 

  abc: xyz and 
  xyz: abc 
  
both represent the same configuration. 

Each connection between two components is represented only once, 
so some components might only ever appear on the left or right side of a colon.

In this example, 
if you disconnect the wire between hfx/pzl, 
the wire between bvb/cmg, and the wire between nvd/jqt, 

you will divide the components into two separate, disconnected groups:

9 components: cmg, frs, lhk, lsr, nvd, pzl, qnr, rsh, and rzs.
6 components: bvb, hfx, jqt, ntq, rhn, and xhk.

Multiplying the sizes of these groups together produces 54.

Find the three wires you need to disconnect 
in order to divide the components into two separate groups. 

What do you get if you multiply the sizes of these two groups together?

*/

using Model = std::map<std::string,std::vector<std::string>>;

template <char C> class SplitOn {
public:
  using Candidate = std::pair<std::string_view, std::string_view>;
  using Candidates = std::vector<Candidate>;
  Candidates parse(std::string_view input) {
    if (input.empty()) return Candidates{}; // return empty parse
    Candidates result;
    // Find the next char C
    size_t pos = input.find(C);
    // If there's no char C, this is the tail
    if (pos == std::string_view::npos) result.push_back({input, {}});
    else {
      auto head = input.substr(0, pos);
      auto tail = input.substr(pos + 1);
      result.push_back({head, tail});
    }
    return result;
  }
};

Model parse(auto& in) {
  Model result{};
  std::string line{};
  while (std::getline(in,line)) {
    SplitOn<':'> split{};
    auto [head,tail] = split.parse(line)[0];
    tail.remove_prefix(1); // remove leading space
    std::cout << NL << "head : " << std::quoted(head) << " tail : " << std::quoted(tail);
    for (auto split = SplitOn<' '>{}.parse(tail); !split.empty(); split = SplitOn<' '>{}.parse(split[0].second)) {
      std::cout << NT << "split : " << std::quoted(split[0].first) << " " << std::quoted(split[0].second);
      result[std::string{head}].push_back(std::string{split[0].first});
    }
  }
  return result;
}

void print_model(Model const& model) {
  std::cout << NL << "Model";
  for (auto const& [head,tails] : model) {
    std::cout << NT << std::quoted(head) << " <--> ";
    for (auto const& tail : tails) {
      std::cout << " , " << std::quoted(tail);
    }
  }
}

namespace part1 {

  class BidirectionalTree {
  public:
    using Node = std::string;
    using AdjacencyList = std::map<Node, std::vector<Node>>;

    BidirectionalTree() = default;

    BidirectionalTree(Model const& model) {
      for (auto const& [head, tails] : model) {
        for (auto const& tail : tails) {
          addEdge(head, tail);
        }
      }
    }

    void addEdge(const Node& from, const Node& to) {
      adjacencyList[from].push_back(to);
      adjacencyList[to].push_back(from);
    }

    void printTree() const {
      std::cout << NL << "Bidirectional Tree"; 
      for (const auto& [node, neighbors] : adjacencyList) {
        std::cout << NT << std::quoted(node) << " --> ";
        for (const auto& neighbor : neighbors) {
          std::cout << " " << std::quoted(neighbor);
        }
      }
      std::cout << NT << "vertex_count : " << vertex_count() << " edge_count : " << edge_count();
    }

    Integer vertex_count() const {
      return adjacencyList.size();
    }

    Integer edge_count() const {
      int result = 0;
      for (auto const& [node, neighbors] : adjacencyList) {
        result += neighbors.size();
      }
      return result / 2; // bidirectional means the edges are counted twice
    }

    AdjacencyList const& getAdjacencyList() const {
      return adjacencyList;
    }

  private:
    AdjacencyList adjacencyList;
  };

  class ConnectedComponents {
  public:
    using Node = std::string;
    using AdjacencyList = std::map<Node, std::vector<Node>>;

    ConnectedComponents(BidirectionalTree const& tree) {
      for (auto const& [node, _] : tree.getAdjacencyList()) {
        nodeToIndex[node] = nodeToIndex.size();
      }
      marked.resize(tree.vertex_count());
      id.resize(tree.vertex_count());
      count = 0;
      for (auto const& [node, _] : tree.getAdjacencyList()) {
        if (!marked[getIndex(node)]) {
          dfs(tree, node);
          count++;
        }
      }
    }

    int getCount() const {
      return count;
    }

    int getId(const Node& node) const {
      return id[getIndex(node)];
    }

  private:
    std::map<Node,int> nodeToIndex{};
    std::vector<bool> marked;
    std::vector<int> id;
    int count{0};

    void dfs(BidirectionalTree const& tree, const Node& node) {
      marked[getIndex(node)] = true;
      id[getIndex(node)] = count;
      for (const auto& neighbor : tree.getAdjacencyList().at(node)) {
        if (!marked[getIndex(neighbor)]) {
          dfs(tree, neighbor);
        }
      }
    }
    int getIndex(const Node& node) const {
      // use index as the position in the adjacency list of the string node
      return nodeToIndex.at(node);
    }
  };

  // In this class, addEdge adds an edge with capacity 1 between two nodes, 
  // bfs performs a breadth-first search from the source to the sink, 
  // fordFulkerson finds the maximum flow from the source to the sink, 
  // and minCut finds the minimum cut after finding the maximum flow.
  class MinCutGraph {
  public:
    MinCutGraph(Model const &model) {
      for (auto const &[node, adjs] : model) {
        for (auto const &adj : adjs) {
          addEdge(node, adj);
        }
      }
    }
    void addEdge(const std::string &u, const std::string &v) {
      adj[u][v] = 1;
      adj[v][u] = 1; // add reverse edge for bidirectional graph
    }

    int vertex_count() const {
      return adj.size();
    }

    int edge_count() const {
      int result = 0;
      for (auto const &[node, neighbors] : adj) {
        result += neighbors.size();
      }
      return result / 2; // bidirectional means the edges are counted twice
    }

    void print_graph() const {
      std::cout << NL << "MinCutGraph";
      for (const auto &[u, neighbors] : adj) {
        std::cout << NT << std::quoted(u) << " -->";
        for (const auto &[v, capacity] : neighbors) {
          std::cout << " " << std::quoted(v);
        }
      }
      std::cout << NT << "vertex_count : " << vertex_count() << " edge_count : " << edge_count();
    }

    std::string getVertex(int index) {
      if (index < 0 || index >= adj.size()) {
        throw std::out_of_range("index out of range");
      }
      return std::next(adj.begin(), index)->first;
    }

    bool bfs(const std::string &source, const std::string &sink,
             std::map<std::string, std::string> &parent) {
      std::map<std::string, bool> visited;
      std::queue<std::string> q;
      q.push(source);
      visited[source] = true;
      parent[source] = "-1";

      while (!q.empty()) {
        std::string u = q.front();
        q.pop();

        for (const auto &[v, capacity] : adj[u]) {
          if (!visited[v] && capacity > 0) {
            q.push(v);
            parent[v] = u;
            visited[v] = true;
          }
        }
      }

      return visited[sink];
    }

    int fordFulkerson(const std::string &source, const std::string &sink) {
      std::string u, v;
      std::map<std::string, std::string> parent;
      int max_flow = 0;

      while (bfs(source, sink, parent)) {
        int path_flow = 1; // as all edges have capacity 1

        for (v = sink; v != source; v = parent[v]) {
          u = parent[v];
          adj[u][v] -= path_flow;
          adj[v][u] += path_flow;
        }

        max_flow += path_flow;
      }

      return max_flow;
    }

    std::pair<std::set<std::string>, std::set<std::string>>
    minCut(const std::string &source, const std::string &sink) {
      fordFulkerson(source, sink);

      std::map<std::string, bool> is_visited;
      std::queue<std::string> q;
      q.push(source);
      is_visited[source] = true;

      while (!q.empty()) {
        std::string u = q.front();
        q.pop();

        for (const auto &[v, capacity] : adj[u]) {
          if (!is_visited[v] && capacity > 0) {
            q.push(v);
            is_visited[v] = true;
          }
        }
      }

      std::set<std::string> reachable, non_reachable;
      for (const auto &[v, _] : is_visited) {
        if (is_visited[v])
          reachable.insert(v);
        else
          non_reachable.insert(v);
      }

      return {reachable, non_reachable};
    }

  private:
    std::map<std::string, std::map<std::string, int>> adj;
  };

  std::pair<BidirectionalTree, BidirectionalTree> split(BidirectionalTree const& tree) {
    auto edge_count = tree.edge_count();
    Integer search_space = edge_count * (edge_count-1) * (edge_count-2); // brute force search space
    std::cout << NL << "split() on tree with edge_count : " << edge_count << " brute force search space : " << search_space;
    // For puzzle input split() on tree with edge_count : 3223 brute force search space : 33 448 493 826

    // Assume there is only one way to cut the graph into two components?
    // Then, if we select two nodes at random, we may quite soon find two that belong to each of the components after the cut?
    // So, using the Ford-Fulkerson, that finds the max flow between a source and a sink, we may detect when we found such a source and sink?

    for (int i = 0; i < 10; ++i) {
      std::set<std::tuple<std::string,std::string>> seen{};
      auto graph = MinCutGraph{tree.getAdjacencyList()};

      // Randomly select two yet unseen nodes as source and sink.
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> dis(0, tree.vertex_count() - 1);

      std::string source = graph.getVertex(dis(gen));
      std::string sink = graph.getVertex(dis(gen));
      while (true) {
        while (source == sink) {
          sink = graph.getVertex(dis(gen));
        }
        if (!seen.contains({source,sink})) break;
        std::string source = graph.getVertex(dis(gen));
      }
      auto max_flow = graph.fordFulkerson(source, sink);
      if (max_flow==3) {
        auto result = graph.minCut(source, sink);
        std::cout << NL << std::quoted(source) << " -- " << max_flow << " -> " << std::quoted(sink);
        std::cout << NT << result.first.size()  << " :";
        for (auto const& node : result.first) {
          std::cout << " " << std::quoted(node);
        }
        std::cout << NT << result.second.size() << " :";
        for (auto const& node : result.second) {
          std::cout << " " << std::quoted(node);
        }
      }
      else {
        std::cout << NL << std::quoted(source) << " -- " << max_flow << " -> " << std::quoted(sink);
      }
    }

    return {{}, {}};
  }

  Result solve_for(Model& model,auto args) {
    Result result{};
    std::cout << NL << NL << "part1";
    print_model(model);
    auto tree = BidirectionalTree{model};
    tree.printTree(); // For example: vertex_count : 15 edge_count : 33 , for puzzle: vertex_count : 1443 edge_count : 3223
    auto [first,second] = split(tree);
    result = first.vertex_count() * second.vertex_count();
    return result;
  }
}

namespace part2 {
  Result solve_for(Model& model,auto args) {
    Result result{};
    std::cout << NL << NL << "part2";
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
  // day00 x y
  std::tuple<int,std::string> args{1,"example.txt"};
  auto& [part,file] = args;
  if (argc > 1 ) {
    part = std::stoi(argv[1]);
    if (argc > 2) {
      file = argv[2];
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