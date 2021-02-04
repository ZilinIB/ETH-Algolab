#include <iostream>
#include <vector>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2                                             Point;
typedef pair<Point, Index>                                     IPoint;

bool feasible(int k, vector<IPoint>& planets, int r) {

    long sq_r = long(r) * long(r);
    int n = planets.size();
    Delaunay t;
    t.insert(planets.begin(), planets.end());
    boost::disjoint_sets_with_storage<> uf(n);

    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Index i1 = e->first->vertex((e->second+1)%3)->info();
        Index i2 = e->first->vertex((e->second+2)%3)->info();
        // ensure smaller index comes first
        if (t.segment(e).squared_length() <= sq_r) {
            uf.link(uf.find_set(i1), uf.find_set(i2));
        }
    }

    // cout << "k: " << k << endl;
    map<int, int> counts;
    for (int i = 0; i < n; i++) {
        // cout << i << ": " << uf.find_set(i) << endl;
        counts[uf.find_set(i)]++;
    }
    int max_count = 0;
    for (auto p : counts) {
        if (p.second > max_count) {
            max_count = p.second;
        }
    }
    return max_count >= k;
}

void testcase() {
    int n, r; cin >> n >> r;
    vector<IPoint> planets;
    planets.reserve(n);
    for (int i = 0; i < n; i++) {
        int x, y; cin >> x >> y;
        planets.emplace_back(Point(x, y), n - 1 - i);
    }

    int lo = 1, hi = n / 2;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        vector<IPoint> pPlanets(planets.begin() + mid, planets.end());
        if (feasible(mid, pPlanets, r)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    cout << lo << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}