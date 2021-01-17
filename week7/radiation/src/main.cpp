#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>
#include <algorithm>
#include <tuple>
#include <utility>

using namespace std;

typedef CGAL::Gmpzf ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef tuple<int, int, int> Location;

bool suffice_(const int d, const vector<Location>& tumor, const vector<Location>& tissue) {
    Program lp(CGAL::SMALLER, false, 0, false, 0);
       // cout << "t " << x << " " << y << " " << z << endl;
    int counter = 1;
    int m = tissue.size();
    for (int a = 0; a <= d; a++) {
        for (int b = 0; b <= d - a; b++) {
            for (int c = 0; c <= d - a - b; c++) {
                

                for (int i = 0; i < tissue.size(); i++) {
                    double coef = 1;
                    coef *= pow(get<0>(tissue[i]), a);
                    coef *= pow(get<1>(tissue[i]), b);
                    coef *= pow(get<2>(tissue[i]), c);
                    // cout << "(" << a << ", " << b << ", " << c << "): " << coef << endl;
                    lp.set_a(counter, i, ET(coef));
                }

                for (int i = 0; i < tumor.size(); i++) {
                    double coef = 1;
                    coef *= pow(get<0>(tumor[i]), a);
                    coef *= pow(get<1>(tumor[i]), b);
                    coef *= pow(get<2>(tumor[i]), c);
                    // cout << "(" << a << ", " << b << ", " << c << "): " << coef << endl;
                    lp.set_a(counter, i + m, ET(-coef));
                }
                counter++;
            }
        }
    }

    int epsilon = 0;
    for (int i = 0; i < m; i++) {
        lp.set_a(epsilon, i, 1);
        // lp.set_b(i, 0.0);
    }

    // Add strictly smaller/larger term
    for (int i = 0; i < tumor.size(); i++) {
        lp.set_a(epsilon, i + m, 1);
        // lp.set_b(i + m, 0.0);
    }
    
    // maximize epsilon
    lp.set_c(epsilon, -1);
    lp.set_l(epsilon, true, 0);
    lp.set_u(epsilon, true, 1);

    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
    return !s.is_infeasible() && (s.objective_value() != 0);
}

void testcase() {
    int h, t; cin >> h >> t;
    vector<Location> tumor;
    vector<Location> tissue;
    for (int i = 0; i < h; i++) {
        int c1, c2, c3; cin >> c1 >> c2 >> c3;
        tissue.push_back(Location(c1, c2, c3));
    }
    for (int i = 0; i < t; i++) {
        int c1, c2, c3; cin >> c1 >> c2 >> c3;
        tumor.push_back(Location(c1, c2, c3));
    }

    int l = 0, r = 1;
    // if (!suffice_(30, tumor, tissue)) {
    //     cout << "Impossible!" << endl;
    //     return;
    // }

    while(!suffice_(r, tumor, tissue) && r <= 30) {
        r *= 2; 
    }
    if (r > 30) {
        r = 30;
    }

    while(l < r) {
        int mid = (l + r) / 2;
        if (suffice_(mid, tumor, tissue)) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    if (l == 30 && !suffice_(30, tumor, tissue)) {
        cout << "Impossible!" << endl;
    } else {
        cout << l << endl;
    }
    
}

int main() {
    ios_base::sync_with_stdio(false);
    int N; cin >> N;
    while (N--) {
        testcase();
    }
}   