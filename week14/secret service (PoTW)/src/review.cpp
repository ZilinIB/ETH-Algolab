#include <iostream>
#include <vector>
// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > > graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

bool feasible(int limit, int a, int s, int d, int c, vector<vector<int> >& paths) {
    graph G(a + c * s);
    edge_adder adder(G);
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < s; j++) {
            for (int k = 0; k < c; k++) {
                if (paths[i][j] < INT_MAX && paths[i][j] + (k + 1) * d <= limit) {
                    adder.add_edge(i, a + j + k * s, 1);
                }
            }
        }
    }
    vertex_desc v_source = boost::add_vertex(G);
    vertex_desc v_target = boost::add_vertex(G);

    for (int i = 0; i < a; i++) {
        adder.add_edge(v_source, i, 1);
    }

    for (int i = 0; i < s; i++) {
        for (int k = 0; k < c; k++) {
            adder.add_edge(a + s * k + i, v_target, 1);
        }
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_target);

    return flow == a;
}

void testcase() {
    int n, m, a, s, c, d;
    cin >> n >> m >> a >> s >> c >> d;
    weighted_graph G_w(n);
    for (int i = 0; i < m; i++) {
        char w;
        int x, y, z;
        cin >> w >> x >> y >> z;
        if (w == 'S') {
            boost::add_edge(x, y, z, G_w);
        } else {
            boost::add_edge(x, y, z, G_w);
            boost::add_edge(y, x, z, G_w);
        }
    }

    vector<int> agents;
    for (int i = 0; i < a; i++) {
        int pos; cin >> pos;
        agents.push_back(pos);
    }

    vector<int> shelters;
    for (int i = 0; i < s; i++) {
        int pos; cin >> pos;
        shelters.push_back(pos);
    }

    
    vector<vector<int> > paths(a, vector<int>(s));
    // cout << "print paths" << endl;
    for (int i = 0; i < a; i++) {
        vector<int> dist_map(n);
        boost::dijkstra_shortest_paths(G_w, agents[i],
            boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G_w))));
        for (int j = 0; j < s; j++) {
            paths[i][j] = dist_map[shelters[j]];
            // cout << paths[i][j] << " ";
        }
        // cout << endl;
    }

    int l = d, r = INT_MAX / 2;
    while (l < r) {
        int mid = (l + r) / 2;
        if (feasible(mid, a, s, d, c, paths)) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    cout << l << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}