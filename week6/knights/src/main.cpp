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
    int m, n, k, c;
    cin >> m >> n >> k >> c;

    if (k == 0) {
        cout << 0 << endl;
        return;
    }

    graph G;
    edge_adder adder(G);

    vector<vector<vertex_desc> > in_vertices(m, vector<vertex_desc>(n));
    vector<vector<vertex_desc> > out_vertices(m, vector<vertex_desc>(n));
    for(int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            vertex_desc v_in = boost::add_vertex(G);
            vertex_desc v_out = boost::add_vertex(G);
            in_vertices[i][j] = v_in;
            out_vertices[i][j] = v_out;
        }
    }

    // add all knights
    vector<vertex_desc> knights;
    for (int i = 0; i < k; i++) {
        int x, y;
        cin >> x >> y;
        knights.push_back(in_vertices[x][y]);
    }
    
    // all normal edges and edges for vertex constrains
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (i < m - 1) {
                adder.add_edge(out_vertices[i][j], in_vertices[i + 1][j], 1);
                adder.add_edge(out_vertices[i + 1][j], in_vertices[i][j], 1);
            }
            if (j < n - 1) {
                adder.add_edge(out_vertices[i][j], in_vertices[i][j + 1], 1);
                adder.add_edge(out_vertices[i][j + 1], in_vertices[i][j], 1);
            }
            adder.add_edge(in_vertices[i][j], out_vertices[i][j], c);
        }
    }

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for (int i = 0; i < m; i++) {
        vertex_desc v1 = out_vertices[i][0];
        vertex_desc v2 = out_vertices[i][n - 1];
        adder.add_edge(v1, v_sink, 1);
        adder.add_edge(v2, v_sink, 1);
    }

    for (int i = 0; i < n; i++) {
        vertex_desc v1 = out_vertices[0][i];
        vertex_desc v2 = out_vertices[m - 1][i];
        adder.add_edge(v1, v_sink, 1);
        adder.add_edge(v2, v_sink, 1);
    }

    for (auto knight : knights) {
        adder.add_edge(v_source, knight, 1);
    }


    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    cout << flow << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}