#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef pair<int, int> position;

void testcase() {
    int n, m;
    long s;
    cin >> n >> m >> s;
    // const int a = 0; assume a is always 1
    const int b = 0;
    const int c1 = 1;
    const int c2 = 2;
    const int d = 3;
    vector<position> noble, common;

    // Program for seperation
    Program lp1 (CGAL::SMALLER, false, 0, false, 0);
    // Program for optimization
    Program lp2 (CGAL::SMALLER, false, 0, false, 0);

    long sum_noble_y = 0;
    long sum_noble_x = 0;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        noble.push_back(make_pair(x, y));
        
        sum_noble_y += y;
        sum_noble_x += x;

        lp1.set_a(b, i, y);
        lp1.set_a(c1, i, 1);
        lp1.set_b(i, -x);

        lp2.set_a(b, i, y);
        lp2.set_a(c1, i, 1);
        lp2.set_b(i, -x);

        lp2.set_a(b, m + n + i, -x);
        lp2.set_a(c2, m + n + i, 1);
        lp2.set_a(d, m + n + i, -1);
        lp2.set_b(m + n + i, -y);

        lp2.set_a(b, 2*m + 2*n + i, x);
        lp2.set_a(c2, 2*m + 2*n + i, -1);
        lp2.set_a(d, 2*m + 2*n + i, -1);
        lp2.set_b(2*m + 2*n + i, y);
    }

    long sum_common_y = 0;
    long sum_common_x = 0;
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        common.push_back(make_pair(x, y));

        sum_common_y += y;
        sum_common_x += x;

        lp1.set_a(b, i + n, -y);
        lp1.set_a(c1, i + n, -1);
        lp1.set_b(i + n, x);

        lp2.set_a(b, i + n, -y);
        lp2.set_a(c1, i + n, -1);
        lp2.set_b(i + n, x);

        lp2.set_a(b, m + 2*n + i, -x);
        lp2.set_a(c2, m + 2*n + i, 1);
        lp2.set_a(d, m + 2*n + i, -1);
        lp2.set_b(m + 2*n + i, -y);

        lp2.set_a(b, 2*m + 3*n + i, x);
        lp2.set_a(c2, 2*m + 3*n + i, -1);
        lp2.set_a(d, 2*m + 3*n + i, -1);
        lp2.set_b(2*m + 3*n + i, y);
    }



    Solution s1 = CGAL::solve_linear_program(lp1, ET());
    if (s1.is_infeasible()) {
        cout << "Yuck!" << endl;
        return;
    }

    lp2.set_a(b, 3 * (m + n), sum_common_y - sum_noble_y);
    lp2.set_a(c1, 3 * (m + n), m - n);
    if (s != -1) {
        lp2.set_b(3 * (m + n), sum_noble_x - sum_common_x + s);
    } else {
        lp2.set_b(3 * (m + n), LONG_MAX);
    }
    

    lp2.set_l(d, true, 0);
    lp2.set_c(d, 1);
    Solution s2 = CGAL::solve_linear_program(lp2, ET());
    if (s2.is_infeasible()) {
        cout << "Bankrupt!" << endl;
    } else {
        double result = s2.objective_value_numerator().to_double() / s2.objective_value_denominator().to_double();
        // cout << s2.objective_value_numerator().to_double() << " " << s2.objective_value_denominator().to_double() << endl;
        // cout << 0 << endl;
        cout << (long)ceil(result) << endl;
    }


}

int main() {
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}