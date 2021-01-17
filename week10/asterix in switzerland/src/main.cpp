#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

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

void testcase() {
    int n, m; cin >> n >> m;
    graph G(n);
    vertex_desc v_source = boost::add_vertex(G);
    vertex_desc v_target = boost::add_vertex(G);
    edge_adder adder(G);

    int pos_counter = 0;
    int pos_total = 0;
    for (int i = 0; i < n; i++) {
        int b; cin >> b;
        // cout << " b " << b << endl;
        if (b > 0) {
            pos_counter++;
            pos_total += b;
            adder.add_edge(v_source, i, b);
        } else {
            adder.add_edge(i, v_target, -b);
        }
    }

    for (int i = 0; i < m; i++) {
        int u, v, debt;
        cin >> u >> v >> debt;
        adder.add_edge(u, v, debt);
    }

    if (pos_counter == n) {
        cout << "yes" << endl;
        return;
    } else if (pos_counter == 0) {
        cout << "no" << endl;
        return;
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_target);
    
    if (flow < pos_total) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}
