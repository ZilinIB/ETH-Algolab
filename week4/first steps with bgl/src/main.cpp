#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       no_property,
                       property<edge_weight_t, int>
                       > weighted_graph;

typedef graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef property_map<weighted_graph, vertex_index_t>::type Index_map;
typedef property_map<weighted_graph, edge_weight_t>::type Edge_weight_map;

void testcase() {
  int n, m;
  cin >> n >> m;
  weighted_graph G(n);
  for (int i = 0; i < m; i++) {
    int from, to, weight;
    cin >> from >> to >> weight;
    add_edge(from, to, weight, G);
  }
  
  Index_map index = get(vertex_index, G);
  Edge_weight_map weights = get(edge_weight, G);
  vector<edge_desc> mst; // vector to store MST edges
  kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  int sum_w = 0;
  
  weighted_graph mst_graph(n);
  for (edge_desc e : mst) {
    sum_w += weights[e];
    add_edge(source(e, G), target(e, G), mst_graph);
  }
  
  
  vertex_desc s = *(vertices(G).first);
  vector<int> dist_map(n);
  dijkstra_shortest_paths(G, s, 
    distance_map(make_iterator_property_map(dist_map.begin(), index)));
  
  int max_dist = 0;
  for (int d : dist_map) {
    if (d > max_dist) {
      max_dist = d;
    }
  }
  
  
  cout << sum_w << " " << max_dist << endl;
}

int main() {
  int t; cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}