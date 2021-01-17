#include <iostream>
#include <vector>
#include <limits>
#include <utility>

using namespace std;

int MAX_COST = 2000000;

pair<int, int> aux(vector<vector<pair<int, int> > >& DP, 
                   vector<vector<pair<int, int> > >& MEM, 
                   vector<int>& cost, 
                   vector<int>& volume,
                   int i, 
                   int j) {
    if (MEM[i][j].first == -1) {

        int v_remain = j - volume[i - 1];
        v_remain = max(v_remain, 0);
        int cost_with_curr = DP[i - 1][v_remain].first + cost[i - 1];
        int variety_with_curr = DP[i - 1][v_remain].second + 1;

        auto result = aux(DP, MEM, cost, volume, i, v_remain);
        int min_cost = result.first + cost[i - 1];
        int max_variety = result.second;
        if (cost_with_curr < min_cost) {
            min_cost = cost_with_curr;
            max_variety = variety_with_curr;
        } else if (cost_with_curr == min_cost) {
            max_variety = max(variety_with_curr, max_variety);
        }
        
        MEM[i][j] = make_pair(min_cost, max_variety);
    }
    return MEM[i][j];
}

void testcase() {
    int n, k; 
    cin >> n >> k;
    vector<int> cost, volume;
    for (int i = 0; i < n; i++) {
        int t1, t2;
        cin >> t1 >> t2;
        cost.push_back(t1);
        volume.push_back(t2);
    }

    // DP[i][j] represents the minimal cost when using the first i beverage
    // and reaching at least k liters

    vector<vector<pair<int, int> > > DP(n + 1, vector<pair<int, int> >(k + 1, make_pair(MAX_COST, 0)));
    vector<vector<pair<int, int> > > MEM(n + 1, vector<pair<int, int> >(k + 1, make_pair(-1, 0)));

    // initialization
    for (int i = 0; i <= n; i++) {
        DP[i][0].first = 0;
        MEM[i][0].first = 0;
    }

    // DP
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= k; j++) {

            auto tmp_result = aux(DP, MEM, cost, volume, i, j);
            int cost_with_curr = tmp_result.first;
            int variety_with_curr = tmp_result.second;


            int cost_without_curr = DP[i - 1][j].first;
            int variety_without_curr = DP[i - 1][j].second;
            if (cost_with_curr >= MAX_COST && cost_without_curr >= MAX_COST) {
                break;
            }

            if (cost_with_curr < cost_without_curr) {
                DP[i][j].first = cost_with_curr;
                DP[i][j].second = variety_with_curr;
            } else if (cost_with_curr > cost_without_curr) {
                DP[i][j].first = cost_without_curr;
                DP[i][j].second = variety_without_curr;
            } else {
                DP[i][j].first = cost_with_curr;
                DP[i][j].second = max(variety_with_curr, variety_without_curr);
            }
        }
    }

    cout << DP[n][k].first << " " << DP[n][k].second << endl;

}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}