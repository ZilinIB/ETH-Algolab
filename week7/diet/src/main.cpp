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

void testcase(int n, int m) {
    vector<int> min_amount;
    vector<int> max_amount;
    vector<int> price;
    vector<vector<int> > amount(m, vector<int>());
    
    for (int i = 0; i < n; i++) {
        int a, b; cin >> a >> b;
        min_amount.push_back(a);
        max_amount.push_back(b);
    }

    for (int i = 0; i < m; i++) {
        int a; cin >> a;
        price.push_back(a);
        for (int j = 0; j < n; j++) {
            int b; cin >> b;
            amount[i].push_back(b);
        }
    }

    Program lp (CGAL::SMALLER, true, 0, false, 0);
    for (int i = 0; i < n; i++) {
        // cout << "A: ";
        for (int j = 0; j < m; j++) {
            lp.set_a(j, 2 * i, amount[j][i]);
            lp.set_a(j, 2 * i + 1, -amount[j][i]);
            // cout << -amount[j][i] << " ";
        }
        lp.set_b(2 * i, max_amount[i]);
        lp.set_b(2 * i + 1, -min_amount[i]);
    }
    for (int i = 0; i < m; i++) {
        lp.set_c(i, price[i]);
    }
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
        cout << "No such diet." << endl;
    } else {
        // cout << s << endl;
        cout << s.objective_value_numerator() / s.objective_value_denominator() << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int n, m; cin >> n >> m;
    while (n || m) {
        testcase(n, m);
        cin >> n;
        cin >> m;
    }
}   