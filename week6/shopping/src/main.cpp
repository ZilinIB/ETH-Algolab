#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
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
    int n, m, s;
    cin >> n >> m >> s;
    map<int, int> stores;
    for (int i = 0; i < s; i++) {
        int tmp; cin >> tmp;
        stores[tmp]++;
    }

    vector<pair<int, int> > streets;
    for (int i = 0; i < m; i++) {
        int from, to;
        cin >> from >> to;
        streets.push_back(make_pair(from, to));
    }

    graph G(n);
    edge_adder adder(G);

    for (auto s : streets) {
        adder.add_edge(s.first, s.second, 1);
        adder.add_edge(s.second, s.first, 1);
    }

    const vertex_desc v_sink = boost::add_vertex(G);
    for (auto s : stores) {
        adder.add_edge(s.first, v_sink, s.second);
        // cout << "new edge: " << s.first << " " << v_sink << " " << s.second << endl;
    }

    long flow = boost::push_relabel_max_flow(G, 0, v_sink);
    if (flow == s) {
        cout << "yes" << endl;
    } else {
        cout << "no"  << endl;
    }

    // edge_it eb, ee;
    // auto c_map = boost::get(boost::edge_capacity, G);
    // for (boost::tie(eb, ee) = boost::edges(G); eb != ee; eb++) {
    //     int from = boost::source(*eb, G);
    //     int to = boost::target(*eb, G);
    //     int cap = c_map[*eb]; 
    //     cout << "(" << from << ", " << to << ", " << cap << ")" << endl;
    // }
    
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}