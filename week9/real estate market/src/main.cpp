#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, int> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, int capacity, int cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

void testcase() {
    int N, M, S;
    cin >> N >> M >> S;
    graph G;
    vector<vertex_desc> buyers, sites, states;
    for (int i = 0; i < N; i++) {
        auto v = boost::add_vertex(G);
        buyers.push_back(v);
    }
    for (int i = 0; i < M; i++) {
        auto v = boost::add_vertex(G);
        sites.push_back(v);
    }
    for (int i = 0; i < S; i++) {
        auto v = boost::add_vertex(G);
        states.push_back(v);
    }

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    for (int i = 0; i < S; i++) {
        int cap; cin >> cap;
        adder.add_edge(v_source, states[i], cap, 0);
    }
    for (int i = 0; i < M; i++) {
        int state; cin >> state;
        adder.add_edge(states[state - 1], sites[i], 1, 0);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int bid; cin >> bid;
            adder.add_edge(sites[j], buyers[i], 1, 100 - bid);
        }
        adder.add_edge(buyers[i], v_sink, 1, 0);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);

    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        // std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
        //     << " with capacity " << c_map[*e] << " and residual capacity " << rc_map[*e] << "\n";
        s_flow += c_map[*e] - rc_map[*e];     
    }
    
    int cost = boost::find_flow_cost(G);
    int profit = 100 * s_flow - cost;
    cout << s_flow << " " << profit << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}