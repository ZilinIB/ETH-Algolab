#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <algorithm>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

void testcase(int n) {
    int d; cin >> d;
    Program lp (CGAL::SMALLER, false, 0, false, 0);
    lp.set_l(d, true, 0);
    for (int i = 0; i < n; i++) {
        int sq_sum = 0;
        for (int j = 0; j < d; j++) {
            int a; cin >> a;
            lp.set_a(j, i, a);
            sq_sum += a * a;
        }
        int b; cin >> b;
        int norm = sqrt(sq_sum);
        while (norm * norm < sq_sum) {
            norm++;
        }
        lp.set_a(d, i, norm);
        lp.set_b(i, b);
    }
    lp.set_c(d, -1);
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
        cout << "none" << endl;
    } else if (s.is_unbounded()) {
        cout << "inf" << endl;
    } else {
        // cout << s << endl;
        cout << -s.objective_value_numerator() / s.objective_value_denominator() << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while (n) {
        testcase(n);
        cin >> n;
    }
}   