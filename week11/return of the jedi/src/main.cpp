#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>


using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator              edge_it;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator          out_edge_it;


void testcase() {
    int n, i;
    cin >> n >> i;
    i--;
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    for (int j = 0; j < n - 1; j++) {
        for (int k = 0; k < n - j - 1; k++) {
            int cost; cin >> cost;
            boost::add_edge(j, j+k+1, cost, G);
        }
    }
    
    vector<int> pred(n);
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(pred.begin(), boost::get(boost::vertex_index, G)), boost::root_vertex(i));

    weighted_graph MST(n);
    weight_map weights_MST = boost::get(boost::edge_weight, MST);
    // cout << "MST: " << endl;
    for (int j = 0; j < n; j++) {
        
        if (j != pred[j]) {
            // cout << j << " " << pred[j] << endl;
            edge_desc e; bool exist;
            boost::tie(e, exist) = boost::add_edge(j, pred[j], MST);
            weights_MST[e] = weights[boost::edge(j, pred[j], G).first];
        }
    }
    // cout << endl;
    

    vector<vector<int> > bottleneck(n, vector<int>(n, 0));
    for (int j = 0; j < n; j++) {
        vector<bool> vis(n, false);
        queue<int> q;
        q.push(j);
        vis[j] = true;
        // bottleneck[j][j] = 0;
        while(!q.empty()) {
            int curr = q.front();
            q.pop();
 
            out_edge_it ebeg, eend;
            for (boost::tie(ebeg, eend) = boost::out_edges(curr, MST); ebeg != eend; ebeg++) {
                int next = boost::target(*ebeg, MST);
                if (vis[next]) {
                    continue;
                }
                bottleneck[j][next] = max(weights_MST[*ebeg], bottleneck[j][curr]);
                q.push(next);
                vis[next] = true;
            }
        }
    }

    // for (int j = 0; j < n; j++) {
    //     for (int k = 0; k < n; k++) {
    //         cout << bottleneck[j][k] << " ";
    //     }
    //     cout << endl;
    // }

    edge_it ebeg, eend;
    int cost = 0;
    for (boost::tie(ebeg, eend) = boost::edges(MST); ebeg != eend; ebeg++) {
        cost += weights_MST[*ebeg];
    }

    int min_inc = INT_MAX;
    for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ebeg++) {
        int start = boost::source(*ebeg, G);
        int end = boost::target(*ebeg, G);
        if (pred[start] != end && pred[end] != start) {
            min_inc = min(min_inc, weights[*ebeg] - bottleneck[start][end]);
        }
    }
    cout << min_inc + cost << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}
