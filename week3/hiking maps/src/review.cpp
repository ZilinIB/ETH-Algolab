#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;

bool is_contained_in(Point& p, vector<Point>& map) {
    return !CGAL::left_turn(map[0], map[1], p) &&
           !CGAL::left_turn(map[2], map[3], p) &&
           !CGAL::left_turn(map[4], map[5], p);
}

void testcase() {
    int m, n; cin >> m >> n;
    vector<Point> path;
    vector<vector<Point> > maps(n);
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        path.push_back(Point(x, y));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 6; j++) {
            int x, y;
            cin >> x >> y;
            maps[i].push_back(Point(x, y));
        }

        for (int j = 0; j < 3; j++) {
            if (CGAL::left_turn(maps[i][2 * j], maps[i][2 * j + 1], maps[i][(2 * j + 2) % 6])) {
                swap(maps[i][2 * j], maps[i][2 * j + 1]);
            }
        }
    }

    // Check which path points are contained in which map
    vector<vector<bool> > contains_point(n, vector<bool>(m, false));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            contains_point[i][j] = is_contained_in(path[j], maps[i]);
            // cout << contains_point[i][j] << " ";
        }
        // cout << endl;
    }
    // cout << endl;

    // Check which segments are contained in which map
    vector<vector<int> > contained_segments(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m - 1; j++) {
            if (contains_point[i][j] && contains_point[i][j + 1]) {
                contained_segments[i].push_back(j);
                // cout << j << " ";
            }
        }
        // cout << endl;
    }

    

    int num_segs = 0;
    vector<int> count_segs(m - 1, 0);

    int l = 0, r = 0;
    int res = n;

    while (r <= n) {
        if (num_segs < m - 1) {
            if (r == n) break;

            for (int s : contained_segments[r]) {
                if (count_segs[s] == 0) {
                    num_segs++;
                }
                count_segs[s]++;
            }
            r++;
        } else {
            for (int s : contained_segments[l]) {
                count_segs[s]--;
                if (count_segs[s] == 0) {
                    num_segs--;
                }
            }
            l++;
        }

        if (num_segs == m - 1) {
            if (r - l < res) {
                res = r - l;
            }
        }
    }

    cout << res << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}