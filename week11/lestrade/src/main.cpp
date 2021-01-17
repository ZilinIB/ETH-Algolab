#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <utility>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store the information value
struct Info {
    int u, v, w;
    int cost;
    // Info(int a, int b, int c) : u(a), v(b), w(c) {};
};

typedef CGAL::Triangulation_vertex_base_with_info_2<Info,K>    Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef pair<K::Point_2, Info>                                 IPoint;

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void testcase() {
    int Z, U, V, W; 
    cin >> Z >> U >> V >> W;
    int a, g; 
    cin >> a >> g;

    vector<IPoint> points;
    points.reserve(g);
    for (int i = 0; i < g; i++) {
        int x, y, u, v, w;
        cin >> x >> y >> u >> v >> w;
        Info info = {u, v, w, INT_MAX};
        points.emplace_back(K::Point_2(x, y), info);
    }
    Delaunay t;
    t.insert(points.begin(), points.end());

    Program lp (CGAL::SMALLER, true, 0, true, 24); 

    map<K::Point_2, Info> costs; 
    for (int i = 0; i < a; i++) {
        int x, y, z;
        cin >> x >> y >> z;
        auto v = t.nearest_vertex(K::Point_2(x, y));
        Info info = v->info();
        K::Point_2 p = v->point();
        auto it = costs.find(p);
        if (it == costs.end()) {
            info.cost = min(info.cost, z);
            costs[p] = info;
        } else {
            it->second.cost = min(it->second.cost, z);
        }
    }

    int counter = 0;
    for (auto p : costs) {
        lp.set_a(counter, 0, -p.second.u);
        lp.set_a(counter, 1, -p.second.v);
        lp.set_a(counter, 2, -p.second.w);
        lp.set_c(counter, p.second.cost);
        ++counter;
    }
    lp.set_b(0, -U);
    lp.set_b(1, -V);
    lp.set_b(2, -W);

    // cout << lp << endl;

    Solution s = CGAL::solve_linear_program(lp, ET());
    double z_tot = INT_MAX;
    if (!s.is_infeasible()) {
        z_tot = (s.objective_value_numerator().to_double() / s.objective_value_denominator().to_double());
    }
    
    // cout << z_tot << " " << Z << " ### ";
    // Solution::Variable_value_iterator opt = s.variable_values_begin();
    // while(opt != s.variable_values_end()) {
    //     cout << (opt->numerator().to_double() / opt->denominator().to_double()) << " ";
    //     opt++;
    // }
    if (z_tot <= Z) {
        cout << "L" << endl;
    } else {
        cout << "H" << endl;
    }

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}
