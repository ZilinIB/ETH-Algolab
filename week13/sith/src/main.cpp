// Be careful with disjoint sets. Only use it with range that starts from 0.
// some error occurs only when number gets large, pay attention to data type.
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel    K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef std::pair<K::Point_2,Index> IPoint;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

bool possible(vector<K::Point_2>& domain, int n, int k, long r) {
    Delaunay t;
    int count = 0;
    t.insert(domain.begin(), domain.end());
    for(auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) v->info() = count++;

    boost::disjoint_sets_with_storage<> uf(n - k);
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Index i1 = e->first->vertex((e->second+1)%3)->info();
        Index i2 = e->first->vertex((e->second+2)%3)->info();
        if (t.segment(e).squared_length() <= r * r) {
            uf.link(uf.find_set(i1), uf.find_set(i2));
        }
    }

    map<int, int> counts;
    for (int i = 0; i < n - k; i++) {
        counts[uf.find_set(i)]++;
    }
    int max_num = 0;
    for (auto p : counts) {
        if (p.second > max_num) {
            max_num = p.second;
        }
    }
    return max_num >= k;
    
}

void testcase() {
    long n, radius;
    cin >> n >> radius;
    vector<K::Point_2> planets(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> planets[i];
    }

    int l = 1, r = n / 2;
    while (l < r) {
        int mid = (l + r + 1) / 2;
        vector<K::Point_2> domain(planets.begin() + mid, planets.end());
        if (possible(domain, n, mid, radius)) {
            l = mid;
        } else {
            r = mid - 1;
        }
    }
    cout << l << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}