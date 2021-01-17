#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

void testcase(int p) {
    int a, b; cin >> a >> b;
    const int X = 0;
    const int Y = 1;
    const int Z = 2;

    if (p == 1) {
        Program lp(CGAL::SMALLER, true, 0, false, 0);
        lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
        lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b);
        lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);
        lp.set_c(X, a); lp.set_c(Y, -b);
        Solution s = CGAL::solve_linear_program(lp, ET());
        if (s.is_infeasible()) {
            cout << "no" << endl;
        } else if (s.is_unbounded()) {
            cout << "unbounded" << endl;
        } else {
            ET num = s.objective_value_numerator();
            ET den = s.objective_value_denominator();
            // cout << num << " " << den << endl;
            cout << -num / den << endl;
        }

    } else {
        Program lp(CGAL::SMALLER, false, 0, true, 0);
        lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1); lp.set_b(0, 4);
        lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a * b);
        lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_b(2, 1);
        lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);
        Solution s = CGAL::solve_linear_program(lp, ET());
        if (s.is_infeasible()) {
            cout << "no" << endl;
        } else if (s.is_unbounded()) {
            cout << "unbounded" << endl;
        } else {
            cout << s << end;
            cout << s.objective_value().numerator() / s.objective_value().denominator()<< endl;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t) {
        testcase(t);
        cin >> t;
    }
}   