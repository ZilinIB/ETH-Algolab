// STL includes
#include <iostream>
#include <vector>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, long> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator              out_edge_it;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index, K>    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2 Point;

using namespace std;

void testcase(int n) {
    vector<Point> pts;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        Point p(x, y);
        pts.push_back(p);
    }

    Delaunay t;
    t.insert(pts.begin(), pts.end());

    int count = 1;
    for (auto face_it = t.finite_faces_begin(); face_it != t.finite_faces_end(); face_it++) {
        face_it->info() = count++;
    }

    weighted_graph G(count);
    weight_map weights = boost::get(boost::edge_weight, G);

    for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {
        int curr_index = f->info();
        for (int i = 0; i < 3; i++) {
            auto neighbor = f->neighbor(i);
            auto p1 = f->vertex((i + 1) % 3)->point();
            auto p2 = f->vertex((i + 2) % 3)->point();
            int neighbor_index = (t.is_infinite(neighbor)) ? 0 : neighbor->info();
            // int neighbor_index = neighbor->info();
            auto e = boost::edge(curr_index, neighbor_index, G);
            if (!e.second) {
                boost::add_edge(curr_index, neighbor_index, LONG_MAX - long(CGAL::squared_distance(p1, p2)), G);
            }
        }
    }
    vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    weighted_graph MST(count);
    weight_map weights_mst = boost::get(boost::edge_weight, MST);
    for (edge_desc e : mst) {
        boost::add_edge(boost::source(e, G), boost::target(e, G), weights[e], MST);
    }

    // bfs to find the bottleneck
    vector<long> bottleneck(count, 0);
    vector<bool> vis(count, false);
    queue<int> q;
    q.push(0);
    vis[0] = true;
    bottleneck[0] = 0;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        out_edge_it ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(curr, MST); ebeg != eend; ebeg++) {
            int target = boost::target(*ebeg, MST);
            if (vis[target]) {
                continue;
            }
            vis[target] = true;
            q.push(target);
            bottleneck[target] = max(bottleneck[curr], weights_mst[*ebeg]);
        }
    }

    for (int i = 0; i < count; i++) {
        bottleneck[i] = LONG_MAX - bottleneck[i];
    }

    int m; cin >> m;
    for (int i = 0; i < m; i++) {
        int x, y;
        long d;
        cin >> x >> y >> d;
        Point curr(x, y);
        Point nn = t.nearest_vertex(curr)->point();
        auto curr_face = t.locate(curr);
        int curr_ind;
        if (t.is_infinite(curr_face)) {
            curr_ind = 0;
        } else {
            curr_ind = t.locate(curr)->info();
        }
        long dist = long(CGAL::squared_distance(curr, nn));
        if (4 * d > bottleneck[curr_ind] || dist < d) {
            cout << "n";
        } else {
            cout << "y";
        }
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while(n) {
        testcase(n);
        cin >> n;
    }
}