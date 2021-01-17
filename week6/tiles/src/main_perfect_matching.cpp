#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/max_cardinality_matching.hpp>


using namespace std;


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

bool is_even(int a) {
    return a % 2 == 0;
}

void testcase() {
    int w, h;
    cin >> w >> h;
    vector<vector<bool> > garden(h, vector<bool>(w, false));
    vector<vector<vertex_desc> > vertices(h, vector<vertex_desc>(w));
    graph G;
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
    
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (!garden[i][j]) {
                continue;
            }

            if (i < h - 1 && garden[i + 1][j]) {
                vertex_desc from = vertices[i][j];
                vertex_desc to = vertices[i + 1][j];
                boost::add_edge(from, to, G);
            }

            if (j < w - 1 && garden[i][j + 1]) {
                vertex_desc from = vertices[i][j];
                vertex_desc to = vertices[i][j + 1];
                boost::add_edge(from, to, G);
            }
        }
    }

    vector<vertex_desc> mate_map(V);  // exterior property map
    boost::edmonds_maximum_cardinality_matching(G,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
    long flow = boost::matching_size(G,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
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