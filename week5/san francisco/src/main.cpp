#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS,
                       vecS,
                       directedS,
                       no_property,
                       property<edge_weight_t, long>
                       > weighted_graph;
typedef graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef graph_traits<weighted_graph>::vertex_iterator vertex_it;
typedef graph_traits<weighted_graph>::out_edge_iterator out_edge_it;
typedef graph_traits<weighted_graph>::edge_iterator edge_it;
typedef property_map<weighted_graph, edge_weight_t>::type Edge_weight_map;

long max_points(int from, int num_steps, weighted_graph& conn, vector<vector<long> >& res) {
    if (res[from][num_steps] != -1) {
        return res[from][num_steps];
    }

    Edge_weight_map weights = get(edge_weight, conn);
    out_edge_it oeb, oee;
    long max_p = 0;
    for (tie(oeb, oee) = out_edges(from, conn); oeb != oee; oeb++) {
        long p = max_points(target(*oeb, conn), num_steps - 1, conn, res);
        if (weights[*oeb] + p > max_p) {
            max_p = p + weights[*oeb];
        }
    }
    res[from][num_steps] = max_p;
    // cout << "from: " << from << ", num_steps: " << num_steps << ", value: " << max_p << endl;
    return max_p;
}

void testcase() {
    int n, m, k;
    long x;
    cin >> n >> m >> x >> k;
    weighted_graph conn(n);
    Edge_weight_map weights = get(edge_weight, conn);
    for (int i = 0; i < m; i++){
        int u, v;
        long p;
        cin >> u >> v >> p;
        edge_desc e;
        e = add_edge(u, v, conn).first; weights[e] = p;
    }
    

    vertex_it vb, ve;
    vector<vertex_desc> end_points;
    for (tie(vb, ve) = vertices(conn); vb != ve; vb++) {
        if (out_degree(*vb, conn) == 0) {
            end_points.push_back(*vb);
        }
    }


    out_edge_it oeb, oee;
    edge_it eb, ee;
    tie(eb, ee) = edges(conn);
    // cout << "*******" << endl;
    // cout << typeid(*eb).name() << endl;
    // cout << "*******" << endl;
    vector<pair<int, long> > edges_to_add;
    for (int i = 0; i < n; i++) {
        for (tie(oeb, oee) = out_edges(i, conn); oeb != oee; oeb++) {
            if (find(end_points.begin(), end_points.end(), target(*oeb, conn)) != end_points.end()) {
                // e.first = 0;
                // conn[i].push_back(make_pair(0, e.second));
                // add_edge(source(*oeb, conn), 0, 0, conn);
                // add_edge(source(*oeb, conn), 0, weights[*oeb], conn);
                edges_to_add.push_back(make_pair(source(*oeb, conn), weights[*oeb]));
                // cout << typeid(*oeb).name() << endl;
                // cout << weights[*oeb] << endl;
            }

        }
    }

    // cout << "start" << endl;

    for (auto p : edges_to_add) {
        add_edge(p.first, 0, p.second, conn);
    }

    vector<vector<long> > res(n, vector<long>(k + 1, -1));
    for (int i = 0; i < n; i++) {
        res[i][0] = 0;
    }

    for (int i = 1; i <= k; i++) {
        // cout << "max:" << max_points(0, i, conn, res) << endl;
        if (max_points(0, i, conn, res) >= x) {
            cout << i << endl;
            return;
        }
    }

    cout << "Impossible" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}