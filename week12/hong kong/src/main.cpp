#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <tuple>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

// BGL includes
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_exact_constructions_kernel      K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Index, K>    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef Delaunay::Face_iterator Face_iterator;
typedef tuple<Index,Index,K::FT> Edge;
typedef vector<Edge> EdgeV;


// K::FT MAX_SQ_DIST = K::FT(LONG_MAX);
K::FT MAX_SQ_DIST = K::FT(LONG_MAX) * K::FT(LONG_MAX); 

void dfs(vector<EdgeV>& mst, vector<K::FT>& bottle_neck, vector<bool>& vis, int start) {
    for (auto e : mst[start]) {
        int child;
        if (get<0>(e) == start) {
            child = get<1>(e);
        } else {
            child = get<0>(e);
        }
        if (vis[child]) {
            continue;
        }
        vis[child] = true;
        bottle_neck[child] = min(bottle_neck[start], get<2>(e));
        dfs(mst, bottle_neck, vis, child);
    }
}

void testcase() {
    int n, m;
    K::FT r;
    cin >> n >> m >> r;
    vector<K::Point_2> points;
    points.reserve(n);
    for (int i = 0; i < n; i++) {
        long x, y;
        cin >> x >> y;
        points.push_back(K::Point_2(x, y));
    }
    Delaunay t;
    t.insert(points.begin(), points.end());
    int ind = 1;
    vector<K::FT> max_clearance;
    max_clearance.push_back(MAX_SQ_DIST);
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
        f->info() = ind++;
    }

    EdgeV edges;
    edges.reserve(3 * ind);

    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
        int curr = f->info();
        for (int i = 0; i < 3; i++) {
            auto neighbor = f->neighbor(i);
            auto p1 = f->vertex((i + 1) % 3)->point();
            auto p2 = f->vertex((i + 2) % 3)->point();
            int next;
            if (t.is_infinite(neighbor)) {
                next = 0;
            } else {
                next = neighbor->info();
            }
            if (next > curr) {
                continue;
            }
            K::FT val = CGAL::squared_distance(p1, p2);
            
            edges.emplace_back(curr, next, val);
        }

        K::Point_2 p1 = f->vertex(0)->point();
        K::Point_2 p2 = f->vertex(1)->point();
        K::Point_2 p3 = f->vertex(2)->point();

        K::FT curr_clearance = CGAL::squared_radius(p1, p2, p3);
        edges.emplace_back(curr, 0, curr_clearance);
    }

    // cout << "edges: " << endl;
    // for (auto e : edges) {
    //     cout << "(" << get<0>(e) << ", " << get<1>(e) << "): " << get<2>(e) << endl;
    // }

    std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) > std::get<2>(e2);
            });
    
    boost::disjoint_sets_with_storage<> uf(ind);
    Index n_components = ind;
    // ... and process edges in order of increasing length
    vector<EdgeV> mst(ind);

    // vector<vector<int> > G(ind);
    // map<pair<int, int>, K::FT> weights;
    for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
        // determine components of endpoints
        int u = std::get<0>(*e);
        int v = std::get<1>(*e);
        Index c1 = uf.find_set(u);
        Index c2 = uf.find_set(v);
        if (c1 != c2) {
            // this edge connects two different components => part of the emst
            uf.link(c1, c2);
            mst[u].push_back(*e);
            mst[v].push_back(*e);

            // G[u].push_back(v);
            // G[v].push_back(u);
            // weights[make_pair(u, v)] = get<2>(*e);
            // weights[make_pair(v, u)] = get<2>(*e);
            if (--n_components == 1){
                break;
            }
        }
    }
    // cout << "mst" << endl;
    // for (int i = 0; i < ind; i++) {
    //     cout << i << " : ";
    //     for (auto e : mst[i]) {
    //         int u = get<0>(e);
    //         int v = get<1>(e);
    //         if (i == u) {
    //             cout << v << " ";
    //         } else {
    //             cout << u << " ";
    //         }
    //     }
    //     cout << endl;
    // }

    // cout << "mst" << endl;
    // for (int i = 0; i < ind; i++) {
    //     cout << i << " : ";
    //     for (auto v : G[i]) {
    //         cout << v << " ";
    //     }
    //     cout << endl;
    // }

    vector<K::FT> bottle_neck(ind);
    vector<bool> vis(ind, false);
    bottle_neck[0] = MAX_SQ_DIST;
    vis[0] = true;
    // cout << "ind" << ind << endl;
    dfs(mst, bottle_neck, vis, 0);

    // cout << " bottleneck: " << endl;
    // for (auto b : bottle_neck) {
    //     cout << b << " ";
    // }

    for (int i = 0; i < m; i++) {
        long x, y;
        K::FT s;
        cin >> x >> y >> s;
        K::Point_2 p(x, y);
        K::FT r_s_squared = (r + s) * (r + s);

        if (CGAL::squared_distance(p, t.nearest_vertex(p)->point()) < r_s_squared) {
            cout << "n";
            continue;
        }
        auto start_face = t.locate(p);
        int start = t.is_infinite(start_face) ? 0 : start_face->info();
        if (4 * r_s_squared <= bottle_neck[start]) {
            cout << "y";
        } else {
            cout << "n";
        }
    }
    cout << endl;

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}
