#include <iostream>
#include <vector>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator          out_edge_it;
typedef boost::graph_traits<weighted_graph>::edge_iterator              edge_it;

using namespace std;

void testcase() {
    int n, i; cin >> n >> i;
    i--;
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    vector<vector<int> > edge_weights(n, vector<int>(n));
    for (int j = 1; j <= n-1; j++) {
        for (int k = 1; k <= n - j; k++) {
            int cost; cin >> cost;
            auto e = boost::add_edge(j - 1, j + k - 1, cost, G).first;
            edge_weights[j - 1][j + k - 1] = cost;
            edge_weights[j + k - 1][j - 1] = cost;
        }
    }

    vector<vertex_desc> pred(n);
    boost::prim_minimum_spanning_tree(G, 
                                      boost::make_iterator_property_map(
                                          pred.begin(), boost::get(boost::vertex_index, G)),
                                      boost::root_vertex(i));
    
    weighted_graph MST(n);
    weight_map weights_MST = boost::get(boost::edge_weight, MST);
    for (int j = 0; j < n; j++) {
        if (pred[j] != j) {
            boost::add_edge(j, pred[j], edge_weights[j][pred[j]], MST);
        }
    }

    vector<vector<int> > bottleneck(n, vector<int>(n, 0));
    for (int j = 0; j < n; j++) {
        vector<bool> vis(n, false);
        queue<int> q;
        q.push(j);
        vis[j] = true;
        while(!q.empty()) {
            int curr = q.front();
            q.pop();      
            out_edge_it ebeg, eend;
            for (boost::tie(ebeg, eend) = boost::out_edges(curr, MST); ebeg != eend; ebeg++) {
                int next = boost::target(*ebeg, MST);
                if (vis[next]) {
                    continue;
                }
                vis[next] = true;
                q.push(next);
                bottleneck[j][next] = max(bottleneck[j][curr], weights_MST[*ebeg]);
            }
        }
    }

    int min_cost = 0;
    edge_it ebeg, eend;
    // cout << "MST: " << endl;
    for (boost::tie(ebeg, eend) = boost::edges(MST); ebeg != eend; ebeg++) {
        min_cost += weights_MST[*ebeg];
        // cout << "(" << boost::source(*ebeg, MST) << ", " << boost::target(*ebeg, MST) << ") ";
    }

    // cout << "min cost: " << min_cost << endl;

    // cout << "bottleneck: " << endl;
    // for (int j = 0; j < n; j++) {
    //     for (int k = 0; k < n; k++) {
    //         cout << bottleneck[j][k] << " ";
    //     }
    //     cout << endl;
    // }

    int sub_min_cost = INT_MAX;
    for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ebeg++) {
        int source = boost::source(*ebeg, G);
        int target = boost::target(*ebeg, G);
        if (!boost::edge(source, target, MST).second) {
            sub_min_cost = min(sub_min_cost, min_cost - bottleneck[source][target] + weights[*ebeg]);
        }
    }
    cout << sub_min_cost << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}