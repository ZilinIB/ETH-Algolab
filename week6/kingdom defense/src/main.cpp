#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>


using namespace std;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > > graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

typedef boost::graph_traits<graph>::edge_iterator edge_it;


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
    int l, p;
    cin >> l >> p;
    vector<int> demands(l);
    graph G(l);
    edge_adder adder = edge_adder(G);
    for (int i = 0; i < l; i++) {
        int g, d;
        cin >> g >> d;
        demands[i] = d - g;
    }

    for (int i = 0; i < p; i++) {
        int f, t, c_min, c_max;
        cin >> f >> t >> c_min >> c_max;
        adder.add_edge(f, t, c_max - c_min);
        demands[f] += c_min;
        demands[t] -= c_min;
    }

    vertex_desc v_s = boost::add_vertex(G);
    vertex_desc v_t = boost::add_vertex(G);


    int demands_total = 0;
    for (int i = 0; i < l; i++) {
        if (demands[i] < 0) {
            adder.add_edge(v_s, i, -demands[i]);
        } else if (demands[i] > 0) {
            demands_total += demands[i];
            adder.add_edge(i, v_t, demands[i]);
        }
    }

    int flow = boost::push_relabel_max_flow(G, v_s, v_t);
    if (flow == demands_total) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }

}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}