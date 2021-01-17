// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>
#include <utility>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >	graph;
// Interior Property Maps
typedef	boost::graph_traits<graph>::vertex_descriptor			vertex_desc;
typedef	boost::graph_traits<graph>::edge_descriptor			edge_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
    int n; cin >> n;
    vector<vector<int> > board(n, vector<int>(n));
    vector<vector<vertex_desc> > vertices(n, vector<vertex_desc>(n));
    // map<vertex_desc, pair<int, int> > locations;
    graph G;
    edge_adder adder(G);
	auto rc_map = boost::get(boost::edge_residual_capacity, G);
    vertex_desc v_source = boost::add_vertex(G);
    vertex_desc v_target = boost::add_vertex(G);
    int aux = (n - 1) % 2;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int tmp; cin >> tmp;
            board[i][j] = tmp;
            if (tmp) {
                vertex_desc v = boost::add_vertex(G);
                vertices[i][j] = v;
                // locations[v] = make_pair(i, j);
                if ((i + j) % 2 == aux) {
                    adder.add_edge(v_source, v, 1);
                } else {
                    adder.add_edge(v, v_target, 1);
                }
            }

        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j]) {
                if ((i + j) % 2 == aux) {
                    vertex_desc v = vertices[i][j];
                    if (i >= 1) {
                        if (j >= 2 && board[i - 1][j - 2]) {
                            adder.add_edge(v, vertices[i - 1][j - 2], 1);
                        }
                        if (j <= n - 3 && board[i - 1][j + 2]) {
                            adder.add_edge(v, vertices[i - 1][j + 2], 1);
                        }
                    }
                    
                    if (i >= 2) {
                        if (j >= 1 && board[i - 2][j - 1]) {
                            adder.add_edge(v, vertices[i - 2][j - 1], 1);
                        }
                        if (j <= n - 2 && board[i - 2][j + 1]) {
                            adder.add_edge(v, vertices[i - 2][j + 1], 1);
                        }
                    }

                    if (i <= n - 2) {
                        if (j >= 2 && board[i + 1][j - 2]) {
                            adder.add_edge(v, vertices[i + 1][j - 2], 1);
                        }
                        if (j <= n - 3 && board[i + 1][j + 2]) {
                            adder.add_edge(v, vertices[i + 1][j + 2], 1);
                        }
                    }

                    if (i <= n - 3) {
                        if (j >= 1 && board[i + 2][j - 1]) {
                            adder.add_edge(v, vertices[i + 2][j - 1], 1);
                        }
                        if (j <= n - 2 && board[i + 2][j + 1]) {
                            adder.add_edge(v, vertices[i + 2][j + 1], 1);
                        }
                    }
                }
            }
        }
    }
    

    // Find a min cut via maxflow
	int flow = boost::push_relabel_max_flow(G, v_source, v_target);
	// std::cout << "maximum flow = minimum cut = " << flow << "\n";

	// BFS to find vertex set S
	map<vertex_desc, int> vis;
	queue<int> Q; // BFS queue (from std:: not boost::)
	vis[v_source] = true; // Mark the source as visited
	Q.push(v_source);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		out_edge_it ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const vertex_desc v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity
			if (rc_map[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}

    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((i + j) % 2 == aux) {
                if (board[i][j] && vis[vertices[i][j]]) {
                    count++;
                }
            } else {
                if (board[i][j] && !vis[vertices[i][j]]) {
                    count++;
                }
            }
        }
    }

    cout << count << endl;

}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}