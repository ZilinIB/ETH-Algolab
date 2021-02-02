// STL includes
#include <iostream>
#include <vector>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

// CGAL includes
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, long> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator              out_edge_it;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index, K>    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2 Point;

using namespace std;

void testcase() {
    int n; cin >> n;
    vector<Point> pts;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        Point p(x, y);
        pts.push_back(p);
    }

    Delaunay t;
    t.insert(pts.begin(), pts.end());

    auto f = t.incident_faces(t.infinite_vertex());
    do {
        f->info() = 0;
    } while (++f != t.incident_faces(t.infinite_vertex()));

    int count = 1;
    for (auto face_it = t.finite_faces_begin(); face_it != t.finite_faces_end(); face_it++) {
        face_it->info() = count++;
    }

    weighted_graph G(count);
    weight_map weights = boost::get(boost::edge_weight, G);
    for (auto edge_it = t.finite_edges_begin(); edge_it != t.finite_edges_end(); edge_it++) {
        Point p1 = edge_it->first->vertex((edge_it->second+1)%3)->point();
        Point p2 = edge_it->first->vertex((edge_it->second+2)%3)->point();
        auto dist = CGAL::squared_distance(p1, p2);
        long length = LONG_MAX - long(CGAL::to_double(CGAL::squared_distance(p1, p2)));

        int source, target;
        source = edge_it->first->info();
        target = edge_it->first->neighbor(edge_it->second)->info();
        boost::add_edge(source, target, length, G);
    }
    vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    weighted_graph MST(count);
    for (edge_desc e : mst) {
        boost::add_edge(boost::source(e, G), boost::target(e, G), weights[e], MST);
    }

    // bfs to find the bottleneck
    vector<long> bottleneck(count);
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
            bottleneck[target] = max(bottleneck[curr], weights[*ebeg]);
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
        int nn_ind = t.locate(curr)->info();
        long dist = long(CGAL::to_double(CGAL::squared_distance(curr, nn)));
        if (dist > bottleneck[nn_ind] || dist < d) {
            cout << "n";
        } else {
            cout << "y";
        }
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}