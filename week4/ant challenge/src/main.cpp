#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       property<vertex_distance_t, int>,
                       property<edge_weight_t, int>
                       > weighted_graph;
            
typedef graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef property_map<weighted_graph, vertex_index_t>::type index_map;
typedef property_map<weighted_graph, edge_weight_t>::type edge_weight_map;
typedef property_map<weighted_graph, vertex_distance_t>::type dist_map;

void testcase() {
  int n, e, s, a, b;
  cin >> n >> e >> s >> a >> b;
  vector<weighted_graph> graph_spicies(s, weighted_graph(n));
  for (int i = 0; i < e; i++) {
    int t1, t2;
    cin >> t1 >> t2;
    for (int j = 0; j < s; j++) {
      int w; cin >> w;
      add_edge(t1, t2, w, graph_spicies[j]);
    }
  }
  
  vector<vector<vertex_desc> > parent(s, vector<vertex_desc>(n));
  weighted_graph G_combined(n);
  edge_weight_map final_weights = get(edge_weight, G_combined);
  for (int i = 0; i < s; i++) {
    int h; cin >> h;
    weighted_graph &G = graph_spicies[i];
    edge_weight_map weights = get(edge_weight, G);
    // index_map index = get(vertex_index, G);
    // dist_map distance = get(vertex_distance, G);
    // prim_minimum_spanning_tree(G, *(vertices(G).first + h), &(parent[i][0]), distance, weights, 
                              // index, default_dijkstra_visitor());
    prim_minimum_spanning_tree(G, &(parent[i][0]), root_vertex(*(vertices(G).first + h)));
    
    // combine all the graphs in graph_spicies so that every edge has a minimal weight
    for (int j = 0; j < n; j++) {
      int p = parent[i][j];
      if (p != j) {
        int w = weights[edge(p, j, G).first];
        if (!edge(p, j, G_combined).second) {
          add_edge(p, j, w, G_combined);
        } else if (w < final_weights[edge(p, j, G_combined).first]) {
          final_weights[edge(p, j, G_combined).first] = w;
        }
      }
    }
  }
  
  vector<int> final_dist_map(n);
  dijkstra_shortest_paths(G_combined, a, distance_map(make_iterator_property_map(
                          final_dist_map.begin(), get(vertex_index, G_combined))));
                          
  cout << final_dist_map[b] << endl;
}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}