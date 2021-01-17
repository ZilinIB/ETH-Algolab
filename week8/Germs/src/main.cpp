#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <algorithm>
#include <utility>

using namespace std;

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
// typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef pair<K::Point_2,Index> IPoint;

K::FT min_squared_dist_to_bound(int x, int y, int l, int b, int r, int t) {
    long min_x = min(x - l, r - x);
    long min_y = min(y - b, t - y);
    return K::FT(min(min_x, min_y) * min(min_x, min_y));
}

// long moment(const K::FT& x) {
//     double x_d = CGAL::to_double(x);
//     double sqrt_x = sqrt(x_d);
//     // while(K::FT(sqrt_x) * K::FT(sqrt_x) < x_d) {
//     //     sqrt_x++;
//     // }
//     double d = sqrt_x - 0.5;
//     if (d < 0) {
//         d = 0;
//     }
//     return ceil(sqrt(d));
// }

    long moment(const K::FT& x) {
        K::FT d = CGAL::sqrt(x);
        d -= K::FT(0.5);
        if (d < K::FT(0)) {
            d = K::FT(0);
        }
        return ceil(CGAL::to_double(CGAL::sqrt(d)));
    }

void testcase(int n) {
    int l, b, r, t;
    cin >> l >> b >> r >> t;
    vector<IPoint> bac;
    bac.reserve(n);
    vector<K::FT> min_squared_dist_4;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        bac.emplace_back(K::Point_2(x, y), i);
        min_squared_dist_4.push_back(min_squared_dist_to_bound(x, y, l, b, r, t));
    }
    Delaunay tri;
    tri.insert(bac.begin(), bac.end());

    for (auto e = tri.finite_edges_begin(); e != tri.finite_edges_end(); e++) {
        Index i1 = e->first->vertex((e->second+1)%3)->info();
        Index i2 = e->first->vertex((e->second+2)%3)->info();
        auto len = tri.segment(e).squared_length() / K::FT(4);
        if (len < min_squared_dist_4[i1]) {
            min_squared_dist_4[i1] = len;
        }
        if (len < min_squared_dist_4[i2]) {
            min_squared_dist_4[i2] = len;
        }
    }

    sort(min_squared_dist_4.begin(), min_squared_dist_4.end());
    cout << moment(min_squared_dist_4.front()) << " ";
    cout << moment(min_squared_dist_4[n / 2]) << " ";
    cout << moment(min_squared_dist_4.back()) << endl;
    // cout << CGAL::to_double(min_squared_dist_4.front()) << " ";
    // cout << CGAL::to_double(min_squared_dist_4[(n - 1) / 2]) << " ";
    // cout << CGAL::to_double(min_squared_dist_4.back()) << endl;
    // for (auto d : min_squared_dist_4) {
    //     cout << CGAL::to_double(d) << " ";
    // }
    // cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while(n) {
        testcase(n);
        cin >> n;
    }
}