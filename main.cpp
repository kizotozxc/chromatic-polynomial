#include <iostream>
#include <map>
#include <set>
#include <vector>

using Graph = std::map<int, std::set<int>>;

// Check if the graph is empty (no edges)
bool isEmpty(const Graph& graph) {
  for (const auto& [vertex, neighbors] : graph) {
    if (!neighbors.empty()) return false;
  }
  return true;
}

// Remove an edge from the graph
Graph removeEdge(const Graph& graph, int u, int v) {
  Graph newGraph = graph;
  newGraph[u].erase(v);
  newGraph[v].erase(u);
  return newGraph;
}

// Contract an edge (u, v): merge vertices u and v
Graph contractEdge(const Graph& graph, int u, int v) {
  Graph newGraph = graph;

  // Transfer neighbors from vertex v to vertex u
  for (int neighbor : graph.at(v)) {
    if (neighbor != u) {
      newGraph[u].insert(neighbor);
      newGraph[neighbor].insert(u);
    }
  }

  // Remove vertex v
  newGraph.erase(v);
  for (auto& [vertex, neighbors] : newGraph) {
    neighbors.erase(v);
  }

  return newGraph;
}

// Recursive function to compute the chromatic polynomial
std::vector<int> chromaticPolynomial(const Graph& graph, int n) {
  std::vector<int> result(n);

  if (isEmpty(graph)) {
    // If the graph is empty, return x^(number of active vertices)
    int n = graph.size();
    result[n - 1] = 1;
    return result;
  }

  // Find any edge (u, v)
  int u = -1, v = -1;
  for (const auto& [vertex, neighbors] : graph) {
    if (!neighbors.empty()) {
      u = vertex;
      v = *neighbors.begin();
      break;
    }
  }

  // Remove and contract edge (u, v)
  Graph graphWithoutEdge = removeEdge(graph, u, v);
  Graph graphContracted = contractEdge(graph, u, v);

  // Recursive calls
  std::vector<int> vec1 = chromaticPolynomial(graphWithoutEdge, n);
  std::vector<int> vec2 = chromaticPolynomial(graphContracted, n);

  for (int i = 0; i < n; i++) {
    result[i] = vec1[i] - vec2[i];
  }

  return result;
}

// function to create a graph
Graph createGraph(const std::vector<std::pair<int, int>>& edges) {
  Graph graph;
  for (const auto& edge : edges) {
    graph[edge.first].insert(edge.second);
    graph[edge.second].insert(edge.first);
  }
  return graph;
}

int main() {
  int numEdges;
  std::cout << "Enter the number of edges: ";
  std::cin >> numEdges;

  int vert;
  std::cout << "Enter the number of vertexes: ";
  std::cin >> vert;

  std::vector<std::pair<int, int>> edges;
  std::cout << "Enter the edges (u v):" << std::endl;
  for (int i = 0; i < numEdges; ++i) {
    int u, v;
    std::cin >> u >> v;
    edges.emplace_back(u, v);
  }

  // Create the graph
  Graph graph = createGraph(edges);

  // Compute the chromatic polynomial
  std::vector<int> result = chromaticPolynomial(graph, vert);
  for (int i = 0; i < result.size(); i++) {
    std::cout << "coefficient of ";
    std::cout << "x^" << i + 1 << " equal " << result[i] << std::endl;
  }

  return 0;
}
