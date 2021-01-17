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
    int m, n, k, l;
    cin >> n >> m >> k >> l;
    map<int, int> police_station;
    map<int, int> photos;
    vector<pair<int, int> > streets;
    graph G(2 * n);
    edge_adder adder = edge_adder(G);
    for (int i = 0; i < k; i++) {
        int tmp; cin >> tmp;
        police_station[tmp]++;
    }
    for (int i = 0; i < l; i++) {
        int tmp; cin >> tmp;
        photos[tmp]++;
    }

    for (int i = 0; i < m; i++) {
        int t1, t2; cin >> t1 >> t2;
        adder.add_edge(t1, t2, 500);
        adder.add_edge(t1 + n, t2 + n, 1);
    }

    const vertex_desc v_s = boost::add_vertex(G);
    const vertex_desc v_t = boost::add_vertex(G);
    for (auto p : police_station) {
        if (p.second > 0) {
            adder.add_edge(v_s, p.first, p.second);
            adder.add_edge(p.first + n, v_t, p.second);
        }
    }

    for (auto p : photos) {
        if (p.second > 0) {
            adder.add_edge(p.first, p.first + n, p.second);
        }
    }

    long flow = boost::push_relabel_max_flow(G, v_s, v_t);
    
    
    cout << flow << endl;
}

int main() {
    // ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}