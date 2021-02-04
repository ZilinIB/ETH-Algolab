#include <iostream>
#include <vector>
#include <utility>

using namespace std;
typedef vector<pair<int, int> > VP;

int MAX_COST = 100000000;

void testcase() {
    int n, k; cin >> n >>k;

    // DP[i][j] stores the min cost and corresponding variety
    // MEM[i][j] stroes the min cost and corresponding variety that must have used beverage i
    vector<VP> DP(n + 1, VP(k + 1, make_pair(MAX_COST, 0)));
    vector<VP> MEM(n + 1, VP(k + 1, make_pair(MAX_COST, 0)));
    VP beverages(n + 1, make_pair(0, 0));
    for (int i = 1; i <= n; i++) {
        int c, v; cin >> c >> v;
        beverages[i].first = c;
        beverages[i].second = v;
    }

    for (int i = 0; i <= n; i++) {
        DP[i][0].first = 0;
        MEM[i][0].first = 0;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k; j++) {
            // 1. compute the min cost with current beverage
            int c = beverages[i].first;
            int v = beverages[i].second;
            int vol_remain = max(0, j - v);
            // 1.1 the cost and variety with only one portion of current beverage
            int cost_with_one_curr = DP[i - 1][vol_remain].first + c;
            int var_with_one_curr = DP[i - 1][vol_remain].second + 1;
            // 1.2 the cost and variety with multiple current beverage
            int cost_with_multi_curr = MEM[i][vol_remain].first + c;
            int var_with_multi_curr = MEM[i][vol_remain].second;
            int cost_with_curr, var_with_curr;
            if (cost_with_one_curr > cost_with_multi_curr) {
                cost_with_curr = cost_with_multi_curr;
                var_with_curr = var_with_multi_curr;
            } else if (cost_with_one_curr < cost_with_multi_curr) {
                cost_with_curr = cost_with_one_curr;
                var_with_curr = var_with_one_curr;
            } else {
                cost_with_curr = cost_with_one_curr;
                var_with_curr = max(var_with_one_curr, var_with_multi_curr);
            }
            MEM[i][j] = make_pair(cost_with_curr, var_with_curr);

            // 2. compute the min cost without current beverage
            int cost_without_curr = DP[i - 1][j].first;
            int var_without_curr = DP[i -1 ][j].second;

            // 3. compute the global min cost
            int min_cost, var;
            if (cost_with_curr > cost_without_curr) {
                min_cost = cost_without_curr;
                var = var_without_curr;
            } else if (cost_with_curr < cost_without_curr) {
                min_cost = cost_with_curr;
                var = var_with_curr;
            } else {
                min_cost = cost_with_curr;
                var = max(var_with_curr, var_without_curr);
            }
            DP[i][j] = make_pair(min_cost, var);

        }
    }
    cout << DP[n][k].first << " " << DP[n][k].second << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}