#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

using namespace std;

int calc_f(EdgeV& edges, int end, int n, int k) {
    boost::disjoint_sets_with_storage<> uf(n);
    Index n_components = n;
    // ... and process edges in order of increasing length
    for (int i = 0; i <= end; i++) {
        Edge e = edges[i];
        // determine components of endpoints
        Index c1 = uf.find_set(get<0>(e));
        Index c2 = uf.find_set(get<1>(e));
        if (c1 != c2) {
            // this edge connects two different components => part of the emst
            uf.link(c1, c2);
        }
    }

    map<Index, int> m;
    for (int i = 0; i < n; i++) {
        Index c = uf.find_set(i);
        m[c]++;
    }

    int f = 0;
    vector<int> counts(k, 0);
    for (auto it = m.begin(); it != m.end(); it++) {
        if (it->second >= k) {
            f++;
        } else {
            counts[it->second]++;
        }
    }

    if (k == 2) {
        f += counts[1] / 2;
    } else if (k == 3) {
        if (counts[2] >= counts[1]) {
            f += counts[1];
            f += (counts[2] - counts[1]) / 2;
        } else {
            f += counts[2];
            f += (counts[1] - counts[2]) / 3;
        }
    } else if (k == 4) {
        f += counts[2] / 2;
        int twos_left = counts[2] % 2;

        if (counts[3] >= counts[1]) {
            f += counts[1];
            f += (counts[3] - counts[1] + twos_left) / 2;
        } else {
            f += counts[3];
            f += (counts[1] - counts[3] + twos_left * 2) / 4;
        }
    }


    return f;
}

void testcase() {
    int n, k, f_;
    long s_;
    cin >> n >> k >> f_ >> s_;

    vector<IPoint> points;
    points.reserve(n);
    for (Index i = 0; i < n; ++i) {
        int x, y;
        cin >> x >> y;
        points.emplace_back(K::Point_2(x, y), i);
    }
    Delaunay t;
    t.insert(points.begin(), points.end());

    EdgeV edges;
    edges.reserve(3*n); // there can be no more in a planar graph
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Index i1 = e->first->vertex((e->second+1)%3)->info();
        Index i2 = e->first->vertex((e->second+2)%3)->info();
        // ensure smaller index comes first
        if (i1 > i2) swap(i1, i2);
        edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    sort(edges.begin(), edges.end(),
            [](const Edge& e1, const Edge& e2) -> bool {
                return get<2>(e1) < get<2>(e2);
            });
    
    int l = -1;
    int r = edges.size() - 1;
    while(l != r) {
        int mid = (l + r + 1) / 2;
        if (get<2>(edges[mid]) >= s_) {
            r = mid - 1;
        } else {
            l = mid;
        }
    }
    int f = calc_f(edges, l, n, k);

    l = -1;
    r = edges.size() - 1;
    while(l != r) {
        int mid = (l + r + 1) / 2;
        if (calc_f(edges, mid, n, k) >= f_) {
            l = mid;
        } else {
            r = mid - 1;
        }
    }
    long s;
    s = get<2>(edges[l + 1]);
    cout << s << " " << f << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}
