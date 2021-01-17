#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS> undirected_graph;
typedef graph_traits<undirected_graph>::vertex_descriptor vertex_desc;
typedef property_map<undirected_graph, vertex_index_t>::type index_map;


void testcase() {
    int n, m; 
    cin >> n >> m;
    
    undirected_graph G(n);

    for (int i = 0; i < m; i++) {
        int from, to;
        cin >> from >> to;
        add_edge(from, to, G);
    }

    index_map index = get(vertex_index, G);

    vector<vertex_desc> arti_ver;
    vector<int> discover(n);
    vector<int> low(n);
    vector<vertex_desc> pred(n);

    articulation_points(G, back_inserter(arti_ver), 
                         discover_time_map(make_iterator_property_map(discover.begin(), index))
                         .lowpoint_map(make_iterator_property_map(low.begin(), index))
                         .predecessor_map(make_iterator_property_map(pred.begin(), index)));
    
    vector<pair<int, int> > bridges;

    for (int i = 0; i < n; i++) {
        int parent = index[pred[i]];
        if (parent == i) {
            continue;
        } else {
            if (low[i] > discover[parent]) {
                if (i < parent) {
                    bridges.push_back(make_pair(i, parent));
                } else {
                    bridges.push_back(make_pair(parent, i));
                }
            }
        }
    }

    // for (int i = 0; i < n; i++) {
    //     cout << "vertex: " << i << ", parent: " << index[pred[i]] << ", dfs: " << discover[i] << ", low: " << low[i] << endl; 
    // }

    sort(bridges.begin(), bridges.end());
    cout << bridges.size() << endl;
    for (auto b : bridges) {
        cout << b.first << " " << b.second << endl;
    }
}

int main() {
    int t; cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
}