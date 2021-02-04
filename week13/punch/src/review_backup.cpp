#include <iostream>
#include <vector>
#include <utility>
#include <climits>

using namespace std;

typedef pair<int, int> Entry;

Entry best_with_curr(int i, int j, vector<vector<Entry> >& DP, vector<vector<Entry> >& MEM, vector<Entry>& beverages) {
    if (MEM[i][j].first < 0) {
        int cost = beverages[i].first;
        int vol = beverages[i].second;
        int vol_remain = j - vol;
        if (vol_remain < 0) {
            vol_remain = 0;
        }
        int prev_cost = DP[i - 1][vol_remain].first;
        int curr_min_cost;
        if (prev_cost == INT_MAX) {
            curr_min_cost = INT_MAX;
        } else {
            curr_min_cost = prev_cost + cost;
        }
        int curr_variety = DP[i - 1][vol_remain].second + 1;

        if (vol_remain > 0) {
            Entry result = best_with_curr(i, vol_remain, DP, MEM, beverages);
            int prev_min_cost;
            if (result.first == INT_MAX) {
                prev_min_cost = INT_MAX;
            } else {
                prev_min_cost = result.first + cost;
            } 
            int prev_variety = result.second;

            if (prev_min_cost < curr_min_cost) {
                curr_min_cost = prev_min_cost;
                curr_variety = prev_variety;
            } else if (prev_min_cost == curr_min_cost) {
                curr_variety = max(curr_variety, prev_variety);
            }
        }
        
        MEM[i][j] = Entry(curr_min_cost, curr_variety);
    } 
        
    return MEM[i][j];
}

void testcase() {
    int n, k; cin >> n >> k;
    vector<Entry> beverages;
    beverages.push_back(make_pair(0, 0));
    for (int i = 0; i < n; i++) {
        int c, v; cin >> c >> v;
        beverages.push_back(make_pair(c, v));
    }

    vector<vector<Entry> >DP(n + 1, vector<Entry>(k + 1));
    vector<vector<Entry> >MEM(n + 1, vector<Entry>(k + 1, make_pair(-1, -1)));
    for (int i = 1; i <= k; i++) {
        DP[0][i] = make_pair(INT_MAX, 0);
    }
    for (int i = 0; i <= n; i++) {
        DP[i][0] = make_pair(0, 0);
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k; j++) {
            Entry with_curr = best_with_curr(i, j, DP, MEM, beverages);
            int cost_with_curr = with_curr.first;
            int var_with_curr = with_curr.second;

            int cost_without_curr = DP[i - 1][j].first;
            int var_without_curr = DP[i - 1][j].second;

            if (cost_with_curr < cost_without_curr) {
                DP[i][j].first = cost_with_curr;
                DP[i][j].second = var_with_curr;
            } else if (cost_with_curr > cost_without_curr) {
                DP[i][j].first = cost_without_curr;
                DP[i][j].second = var_without_curr;
            } else if (cost_with_curr == INT_MAX) {
                DP[i][j].first = INT_MAX;
                DP[i][j].second = 0;
            } else {
                DP[i][j].first = cost_with_curr;
                DP[i][j].second = max(var_with_curr, var_without_curr);
            }
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