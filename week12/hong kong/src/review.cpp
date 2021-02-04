#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <iostream>

using namespace std;

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_2<K>                    Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index, K>    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef K::Point_2  Point;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

K::FT MAX_SQ_DIST = K::FT(LONG_MAX) * K::FT(LONG_MAX);

void testcase() {
    int n, m;
    long r;
    cin >> n >> m >> r;
    vector<Point> trees;
    for (int i = 0; i < n; i++) {
        long x, y; cin >> x >> y;
        trees.push_back(Point(x, y));
    }

    Delaunay t;
    t.insert(trees.begin(), trees.end());

    int count = 1;
    for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
        f->info() = count++;
    }

    EdgeV edges;
    for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
        int curr_index = f->info();
        for (int i = 0; i < 3; i++) {
            auto f_neighbor = f->neighbor(i); 
            int neighbor_index;
            if (t.is_infinite(f_neighbor)) {
                neighbor_index = 0;
            } else {
                neighbor_index = f_neighbor->info();
            }
            // if (neighbor_index > curr_index) {
            //     continue;
            // }
            Point p1 = f->vertex((i+1)%3)->point();
            Point p2 = f->vertex((i+2)%3)->point();
            K::FT edge_len_sq = CGAL::squared_distance(p1, p2);                
            edges.emplace_back(curr_index, neighbor_index, edge_len_sq);
            
        }
        Point p1 = f->vertex(0)->point();
        Point p2 = f->vertex(1)->point();
        Point p3 = f->vertex(2)->point();

        K::FT curr_clearance = CGAL::squared_radius(p1, p2, p3);
        edges.emplace_back(f->info(), 0, curr_clearance);
    }

    // cout << "edges: " << endl;
    // for (auto e : edges) {
    //     cout << "(" << get<0>(e) << ", " << get<1>(e) << "): " << get<2>(e) << endl;
    // }

    sort(edges.rbegin(), edges.rend(),
	    [](const Edge& e1, const Edge& e2) -> bool {
	      return std::get<2>(e1) < std::get<2>(e2);
            });

    vector<vector<int> > G(count);
    map<pair<int, int>, K::FT> weights;
    boost::disjoint_sets_with_storage<> uf(count);
    Index n_components = count;
    // ... and process edges in order of increasing length
    for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
        // determine components of endpoints
        int u = std::get<0>(*e);
        int v = std::get<1>(*e);
        Index c1 = uf.find_set(u);
        Index c2 = uf.find_set(v);
        if (c1 != c2) {
            // this edge connects two different components => part of the emst
            uf.link(c1, c2);
            G[u].push_back(v);
            G[v].push_back(u);
            weights[make_pair(u, v)] = get<2>(*e);
            weights[make_pair(v, u)] = get<2>(*e);
            if (--n_components == 1) break;
        }
    }

    // cout << "mst" << endl;
    // for (int i = 0; i < count; i++) {
    //     cout << i << " : ";
    //     for (auto v : G[i]) {
    //         cout << v << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "weights: " << endl;
    // for (auto p : weights) {
    //     cout << p.first.first << ", " << p.first.second << ": " << p.second << endl;
    // }
    

    vector<K::FT> bottleneck(count);
    bottleneck[0] = MAX_SQ_DIST;

    queue<int> q;
    vector<bool> vis(count);
    q.push(0);
    vis[0] = true;
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        for (int child : G[curr]) {
            if (vis[child]) {
                continue;
            }
            q.push(child);
            vis[child] = true;
            bottleneck[child] = min(bottleneck[curr], weights[make_pair(curr, child)]);
        }
    }

    // cout << " bottleneck: " << endl;
    // for (auto b : bottleneck) {
    //     cout << b << " ";
    // }

    for (int i = 0; i < m; i++) {
        long x, y, s;
        cin >> x >> y >> s;
        K::FT r_s_sq = K::FT(r + s) * K::FT(r + s);
        Point curr(x, y);
        Point nn = t.nearest_vertex(curr)->point();
        if (CGAL::squared_distance(curr, nn) < r_s_sq) {
            cout << "n";
        } else {
            auto f = t.locate(curr);
            int f_ind;
            if (t.is_infinite(f)) {
                f_ind = 0;
            } else {
                f_ind = f->info();
            }
            if (4 * r_s_sq <= bottleneck[f_ind]) {
                cout << "y";
            } else {
                cout << "n";
            }
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