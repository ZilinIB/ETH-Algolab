#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS,
                       vecS,
                       undirectedS> undirected_graph;
typedef graph_traits<undirected_graph>::vertex_descriptor vertex_desc;

bool similar(vector<string> &c1, vector<string> &c2, int f) {
    int count = 0;
    int n = c1.size();
    int a = 0, b = 0;
    while(a < n && b < n) {
        int res = c1[a].compare(c2[b]);
        if (res == 0) {
            count++;
            a++;
            b++;
        } else if (res < 0) {
            a++;
        } else {
            b++;
        }
    }
    return count > f;
}

void testcase() {
    int n, c, f; 
    cin >> n >> c >> f;
    vector<vector<string> > cha(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < c; j++) {
            string curr_char; cin >> curr_char;
            cha[i].push_back(curr_char);
        }
        sort(cha[i].begin(), cha[i].end());
    }

    undirected_graph G(n);
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (similar(cha[i], cha[j], f)) {
                add_edge(i, j, G);
            }
        }
    }

    vector<vertex_desc> mate_map(n);
    const vertex_desc NULL_VERTEX = graph_traits<undirected_graph>::null_vertex();

    edmonds_maximum_cardinality_matching(G, make_iterator_property_map(mate_map.begin(),
                                         get(vertex_index, G)));
    int m_size = matching_size(G, make_iterator_property_map(mate_map.begin(), 
                               get(vertex_index, G)));
    
    cout << ((m_size == n / 2) ? "not optimal" : "optimal") << endl;

}

int main() {
    int t; cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
}