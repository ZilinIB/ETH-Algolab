#include <vector>
#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>
#include <tuple>

using namespace std;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Cell {
    int x, y, z;

    Cell(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
};

bool feasible(int d, vector<Cell>& H, vector<Cell>& T) {
    Program lp (CGAL::SMALLER, false, 0, false, 0); 
    int h = H.size();
    int t = T.size();
    int index = 0;
    for (int a = 0; a <= d; a++) {
        for (int b = 0; b <= d - a; b++) {
            for (int c = 0; c <= d - a - b; c++) {
                for (int i = 0; i < h; i++) {
                    double val = pow(H[i].x, a) * pow(H[i].y, b) * pow(H[i].z, c);
                    lp.set_a(index, i, ET(val));
                }
                for (int i = 0; i < t; i++) {
                    double val = pow(T[i].x, a) * pow(T[i].y, b) * pow(T[i].z, c);
                    lp.set_a(index, i + h, ET(-val));
                }
                index++;
            }
        }
    }
    for (int i = 0; i < h + t; i++) {
        lp.set_b(i, -1);
    }

    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    Solution s = CGAL::solve_linear_program(lp, ET(), options);
    return !s.is_infeasible();
}

void testcase() {
    int h, t;
    cin >> h >> t;
    vector<Cell> H, T;
    for (int i = 0; i < h; i++) {
        int x, y, z; cin >> x >> y >> z;
        H.push_back(Cell(x, y, z));
    }
    for (int i = 0; i < t; i++) {
        int x, y, z; cin >> x >> y >> z;
        T.push_back(Cell(x, y, z));
    }

    int l = 0, r = 1;
    while(!feasible(r, H, T) && r <= 30) {
        l = r;
        r *= 2;
    }
    if (r > 30) r = 30;

    while(l < r) {
        int mid = (l + r) / 2;
        if (feasible(mid, H, T)) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }
    
    if (l == 30 && !feasible(30, H, T)) {
        cout << "Impossible!" << endl;
    } else {
        cout << l << endl;
    }

}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}