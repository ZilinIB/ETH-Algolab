#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// #include "printlp.hpp"


using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>                   Delaunay;
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


struct warehouse
{
    K::Point_2 position;
    int supply;
    int alcohol;
};

struct stadium
{
    K::Point_2 position;
    int demand;
    int bound;
};

void testcase() {
    int n, m, c;
    cin >> n >> m >> c;
    vector<warehouse> W;
    vector<stadium> S;
    vector<K::Point_2> points;
    vector<vector<int> > revenues(n, vector<int>(m));
    vector<vector<int> > num_crossing_lines(n, vector<int>(m, 0));
    Delaunay t;
    for (int i = 0; i < n; i++) {
        int x, y, s, a;
        cin >> x >> y >> s >> a;
        K::Point_2 p(x, y);
        warehouse w = {p, s, a};
        W.push_back(w);
        points.push_back(p);
        // W.emplace_back(warehouse{K::Point_2(0, 0), 0, 0});
    }
    for (int i = 0; i < m; i++) {
        int x, y, d, u;
        cin >> x >> y >> d >> u;
        K::Point_2 p(x, y);
        stadium s = {p, d, u};
        S.push_back(s);
        points.push_back(p);
    }

    t.insert(points.begin(), points.end());

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int r; cin >> r;
            revenues[i][j] = r;
        }
    }

    int num_tot_lines = 0;
    vector<vector<bool> > w_in_line(n);
    vector<vector<bool> > s_in_line(m);

    for (int i = 0; i < c; i++) {
        int x, y;
        K::FT r;
        cin >> x >> y >> r;
        K::FT sq_r = r * r;
        K::Point_2 center(x, y);
        K::Point_2 nn = t.nearest_vertex(center)->point();
        if (CGAL::squared_distance(nn, center) < sq_r) {
            num_tot_lines++;
            for (int j = 0; j < n; j++) {
                bool in_line = (CGAL::squared_distance(center, W[j].position) < sq_r);
                w_in_line[j].push_back(in_line);
            }
            for (int j = 0; j < m; j++) {
                bool in_line = (CGAL::squared_distance(center, S[j].position) < sq_r);
                s_in_line[j].push_back(in_line);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < num_tot_lines; k++) {
                if (w_in_line[i][k] != s_in_line[j][k]) {
                    num_crossing_lines[i][j]++;
                }
            }
        }
    }

    Program lp (CGAL::SMALLER, true, 0, false, 0);

    int counter = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            lp.set_a(i * m + j, counter, 1);  
        }
        lp.set_b(counter, W[i].supply);
        counter++;
    }
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            lp.set_a(i * m + j, counter, 1);
            lp.set_a(i * m + j, counter + 1, -1);
            lp.set_a(i * m + j, counter + 2, W[i].alcohol);
        }
        lp.set_b(counter, S[j].demand);
        lp.set_b(counter + 1, -S[j].demand);
        lp.set_b(counter + 2, S[j].bound * 100);
        counter += 3;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            lp.set_c(i * m + j, num_crossing_lines[i][j] - 100 * revenues[i][j]);
        }
    }

    // cout << lp << endl;
    // int h = 0;

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
        cout << "RIOT!" << endl;
    } else {
        double res = -(s.objective_value_numerator() / s.objective_value_denominator()).to_double() / 100.0;
        cout << (int)floor(res) << endl;
    }

}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}
