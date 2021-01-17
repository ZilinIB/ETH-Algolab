// Remarks: keep in mind that the unreachable points are represented by INT_MAX in dist map, avoid overflow

#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > > graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

typedef std::tuple<int, int, int> Edge;
typedef std::vector<Edge> EdgeV;

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
    int n, m, a, s, c, d;
    cin >> n >> m >> a >> s >> c >> d;
    weighted_graph G(n);
    for (int i = 0; i < m; i++) {
        char w;
        int x, y, z;
        cin >> w >> x >> y >> z;
        boost::add_edge(x, y, z, G);
        if (w == 'L') {
            boost::add_edge(y, x, z, G);
        }
    }

    vector<int> agents, shelters;
    for (int i = 0; i < a; i++) {
        int pos; cin >> pos;
        agents.push_back(pos);
    }

    for (int i = 0; i < s; i++) {
        int pos; cin >> pos;
        shelters.push_back(pos);
    }

    int max_dist = 0;
    vector<vector<int> > edges(a, vector<int>(s));
    for (int i = 0; i < a; i++) {
        vector<int> dist_map(n);
        boost::dijkstra_shortest_paths(G, agents[i],
            boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));
        for (int j = 0; j < s; j++) {
            int dist = dist_map[shelters[j]];
            edges[i][j] = dist;
            if (dist > max_dist) {
                max_dist = dist;
            }
        }
    }
    
    int l = 0, r = INT_MAX;

    while(l < r) {
        int mid = (l + r) / 2;

        // create new graph
        graph G_new(a + s * c);
        edge_adder adder(G_new);
        
        vertex_desc v_source = boost::add_vertex(G_new);
        vertex_desc v_target = boost::add_vertex(G_new);

        for (int i = 0; i < a; i++) {
            adder.add_edge(v_source, i, 1);
        }
        for (int i = 0; i < s; i++) {
            for (int j = 0; j < c; j++) {
                adder.add_edge(a + i + j * s, v_target, 1);
            }   
        }

        for (int i = 0; i < a; i++) {
            for (int j = 0; j < s; j++) {
                if (edges[i][j] == INT_MAX) {
                    continue;
                }
                for (int k = 0; k < c; k++) {
                    if (edges[i][j] + d * (k + 1) <= mid) {
                        adder.add_edge(i, a + j + k * s, 1);
                    }
                }
            }
        }

        long flow = boost::push_relabel_max_flow(G_new, v_source, v_target);
        if (flow == a) {
            r = mid;
        } else { 
            l = mid + 1;
        }
    }
    assert(l == r);
    cout << l << endl; 
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}