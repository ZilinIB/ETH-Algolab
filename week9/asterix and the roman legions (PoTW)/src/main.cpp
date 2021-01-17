#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <algorithm>

using namespace std;

typedef long IT;
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
    long x, y, n; cin >> x >> y >> n;
    Program lp (CGAL::SMALLER, false, 0, false, 0);

    const int T = 0;
    const int X = 1;
    const int Y = 2;

    for (int i = 0; i < n; i++) {
        long a, b, c, v;
        cin >> a >> b >> c >> v;
        if (a * x + b * y + c > 0) {
            a = -a;
            b = -b;
        } else {
            c = -c;
        }
        long coef = v * (long)sqrt(pow(a, 2) + pow(b, 2));
        lp.set_a(T, i, coef); lp.set_a(X, i, a); lp.set_a(Y, i, b); 
        lp.set_b(i, c);
    }
    lp.set_l(T, 0);
    lp.set_c(T, -1);

    Solution s = CGAL::solve_linear_program(lp, ET());
    cout << (long) (-s.objective_value_numerator() / s.objective_value_denominator()).to_double() << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
}