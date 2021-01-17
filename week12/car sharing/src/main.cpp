#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <set>
#include <map>

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
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator


// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
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

struct Booking {
    int start, target, depart, arrive, profit;
    Booking(int s, int t, int d, int a, int p) : start(s), target(t), depart(d), arrive(a), profit(p) {}
};

int T = 100000;
int P = 100;

void testcase() {
    int N, S; cin >> N >> S;
    vector<int> initial_cars;
    for (int i = 0; i < S; i++) {
        int tmp; cin >> tmp;
        initial_cars.push_back(tmp);
    }

    vector<set<int>> time(S);

    vector<Booking> bookings;
    for (int i = 0; i < N; i++) {
        int s, t, d, a, p;
        cin >> s >> t >> d >> a >> p;
        s--;
        t--;
        bookings.push_back(Booking(s, t, d, a, p));
        time[s].insert(d);
        time[t].insert(a);
    }

    for (int i = 0; i < S; i++) {
        time[i].insert(0);
        time[i].insert(T);
    }

    int count = 0;
    vector<map<int, int> > index_map(S);
    for (int i = 0; i < S; i++) {
        for (int t : time[i]) {
            index_map[i][t] = count++;
        }
    }

    graph G(count);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    vertex_desc v_source = boost::add_vertex(G);
    vertex_desc v_target = boost::add_vertex(G);
    
    // add initial edges
    for (int i = 0; i < S; i++) {
        adder.add_edge(v_source, index_map[i][*time[i].begin()], initial_cars[i], 0);
    }

    // add end edges (till the last booking ends)
    for (int i = 0; i < S; i++) {
        adder.add_edge(index_map[i][*time[i].rbegin()], v_target, 1000, 0);
    }

    // add parking edges
    for (int i = 0; i < S; i++) {
        int prev_index = index_map[i][0];
        int prev_time = 0;
        for (set<int>::iterator it = ++time[i].begin(); it != time[i].end(); it++) {
            int curr_time = *it;
            int curr_index = index_map[i][curr_time];
            int duration = curr_time - prev_time;
            adder.add_edge(prev_index, curr_index, 1000, duration * P);
            prev_index = curr_index;
            prev_time = curr_time;
        }
    }

    // add booking edges
    for (Booking b : bookings) {
        int from = index_map[b.start][b.depart];
        int to = index_map[b.target][b.arrive];
        int duration = b.arrive - b.depart;
        adder.add_edge(from, to, 1, duration * P - b.profit);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }

    cout << s_flow * P * T - cost << endl;


}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}
