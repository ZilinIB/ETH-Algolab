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

bool is_even(int a) {
    return a % 2 == 0;
}

void testcase() {
    int w, h;
    cin >> w >> h;
    vector<vector<bool> > garden(h, vector<bool>(w, false));
    vector<vector<vertex_desc> > vertices(h, vector<vertex_desc>(w));
    graph G;
    edge_adder adder = edge_adder(G);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            char tmp;
            cin >> tmp;
            if (tmp == '.') {
                vertex_desc v = boost::add_vertex(G);
                vertices[i][j] = v;
                garden[i][j] = true;
            }
        }
    }

    int V = boost::num_vertices(G);
    if (!is_even(V)) {
        cout << "no" << endl;
        return;
    }

    vertex_desc source = boost::add_vertex(G);
    vertex_desc target = boost::add_vertex(G);
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (!garden[i][j]) {
                continue;
            }
            int distance = i + j;
            if (is_even(distance)) {
                adder.add_edge(source, vertices[i][j], 1);
            } else {
                adder.add_edge(vertices[i][j], target, 1);
            }

            if (i < h - 1 && garden[i + 1][j]) {
                vertex_desc from = vertices[i][j];
                vertex_desc to = vertices[i + 1][j];
                if (!is_even(distance)){
                    swap(from, to);
                }
                adder.add_edge(from, to, 1);
            }

            if (j < w - 1 && garden[i][j + 1]) {
                vertex_desc from = vertices[i][j];
                vertex_desc to = vertices[i][j + 1];
                if (!is_even(distance)){
                    swap(from, to);
                }
                adder.add_edge(from, to , 1);
            }
        }
    }

    long flow = boost::push_relabel_max_flow(G, source, target);
    if (flow == V / 2) {
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