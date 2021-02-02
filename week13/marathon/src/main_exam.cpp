// graph for mincost maxflow contains negative weight edge, can't be used in 
// Calculating shortest paths.
#include <iostream>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > > graph;


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  edge_desc add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    return e;
  }
};

vector<int> dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map;
}

void testcase() {
    int n, m, s, f;
    cin >> n >> m >> s >> f;

    graph G(n);
    weighted_graph G_aux(n);
    edge_adder adder(G);
    weight_map weights = boost::get(boost::edge_weight, G_aux); // new!
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);

    vector<vector<edge_desc> > edges(n, vector<edge_desc>(n));

    for (int i = 0; i < m; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        if (a == b) {
            continue;
        }

        if (boost::edge(a, b, G_aux).second) {
            auto e1 = boost::edge(a, b, G_aux).first;
            auto e2 = boost::edge(b, a, G_aux).first;

            if (weights[e1] > d) {
                weights[e1] = d;
                weights[e2] = d;
                c_map[edges[a][b]] = c;
                c_map[edges[b][a]] = c;
            } else if (weights[e1] == d) {
                c_map[edges[a][b]] += c;
                c_map[edges[b][a]] += c;
            }
        } else {
            
            auto e1 = adder.add_edge(a, b, c);
            auto e2 = adder.add_edge(b, a, c);
            edges[a][b] = e1;
            edges[b][a] = e2;

            boost::add_edge(a, b, d, G_aux);
            boost::add_edge(b, a, d, G_aux);
        }
    }

    vector<int> dist_from_s = dijkstra_dist(G_aux, s);
    vector<int> dist_from_f = dijkstra_dist(G_aux, f);
    
    int min_dist = dist_from_s[f];

    auto edge_its = boost::edges(G);
    auto ebeg = edge_its.first;
    auto eend = edge_its.second;
    for (; ebeg != eend; ebeg++) {
        int start = boost::source(*ebeg, G);
        int end = boost::target(*ebeg, G);
        if (weights[boost::edge(start, end, G_aux).first] + dist_from_s[start] + dist_from_f[end] > min_dist) {
            c_map[*ebeg] = 0;
        }
    }
    cout << boost::push_relabel_max_flow(G, s, f) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}