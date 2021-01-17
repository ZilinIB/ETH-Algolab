#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

void testcase(int n) {
    Triangulation t;
    vector<K::Point_2> rs;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        rs.push_back(K::Point_2(x, y));
    }
    t.insert(rs.begin(), rs.end());

    int m; cin >> m;
    for (int i = 0; i < m; i++) {
        int x, y; cin >> x >> y;
        auto p = K::Point_2(x, y);
        auto nn = t.nearest_vertex(p);
        cout << long(CGAL::to_double(CGAL::squared_distance(p, nn->point()))) << endl;
    }
}

int main() {
    int n; cin >> n;
    while(n) {
        testcase(n);
        cin >> n;
    }
}