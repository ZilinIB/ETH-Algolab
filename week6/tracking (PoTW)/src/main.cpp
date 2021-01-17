#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS,
                       no_property,
                       property<edge_weight_t, int>
                       > weighted_graph;
typedef graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef graph_traits<weighted_graph>::adjacency_iterator adj_vertex_it;
typedef graph_traits<weighted_graph>::edge_iterator edge_it;
typedef property_map<weighted_graph, edge_weight_t>::type edge_weight_map;

void testcase() {
    int n, m, k, x, y;
    cin >> n >> m >> k >> x >> y;
    weighted_graph G(n);
    weighted_graph G_compact(n * (k + 1));
    map<edge_desc, int> is_river; 
    edge_weight_map weights = get(edge_weight, G);
    for (int i = 0; i < m; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        edge_desc e;
        e = add_edge(a, b, c, G).first;
        if (d == 0) {
            is_river[e] = 0;
        } else {
            is_river[e] = 1;
        }
    }

    edge_it eb, ee;
    for (int i = 0; i <= k; i++) {
        for (tie(eb, ee) = edges(G); eb != ee; eb++) {
            int from = source(*eb, G) + i * n;
            int to = target(*eb, G) + i * n;
            int weight = weights[*eb];
            add_edge(from, to , weight, G_compact);
            if (i > 0 && is_river[*eb]) {
                add_edge(from, to -n, weight, G_compact);
                add_edge(from - n, to, weight, G_compact);
            }
        }
    }

    vector<int> dist_map((k + 1) * n);
    dijkstra_shortest_paths(G_compact, x,
        distance_map(make_iterator_property_map(
        dist_map.begin(), get(vertex_index, G_compact))));

    cout << dist_map[k * n + y] << endl;

}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}

