#include <iostream> 
#include <vector>
#include <climits>

using namespace std;

void dfs(int root, vector<vector<int> >& G, vector<int>& cost, vector<vector<int> >& DP) {
    for (int child : G[root]) {
        dfs(child, G, cost, DP);
    }

    // for state 0
    int tot_cost_0 = cost[root];
    for (int child : G[root]) {
        tot_cost_0 += min(min(DP[child][0], DP[child][1]), DP[child][2]);
    }
    DP[root][0] = tot_cost_0;

    // for state 1, at least one child is 0
    int tot_cost_1 = 0;
    bool condition_holds = false;
    for (int child : G[root]) {
        if (DP[child][0] <= DP[child][1]) {
            tot_cost_1 += DP[child][0];
            condition_holds = true;
        } else {
            if (DP[child][1] == INT_MAX) {
                tot_cost_1 = INT_MAX;
                break;
            }
            tot_cost_1 += DP[child][1];
        }
    }

    if (!condition_holds) {
        tot_cost_1 = INT_MAX;
        for (int child : G[root]) {
            int curr_tot = 0;
            curr_tot += DP[child][0];
            for (int child_2 : G[root]) {
                if (child == child_2) continue;
                if (DP[child_2][1] == INT_MAX) {
                    curr_tot = INT_MAX;
                    break;
                }
                curr_tot += DP[child_2][1];
            }
            tot_cost_1 = min(tot_cost_1, curr_tot);
        }
    }
    DP[root][1] = tot_cost_1;

    // for state 2, all children needs to be 1;
    int tot_cost_2 = 0;
    for (int child : G[root]) {
        if (DP[child][1] == INT_MAX) {
            tot_cost_2 = INT_MAX;
            break;
        }
        tot_cost_2 += DP[child][1];
    }
    DP[root][2] = tot_cost_2;
    return;
}

void testcase() {
    int n; cin >> n;
    vector<vector<int> > G(n);
    for (int i = 0; i < n - 1; i++) {
        int from, to; cin >> from >> to;
        G[from].push_back(to);
    }
    vector<int> cost;
    for (int i = 0; i < n; i++) {
        int c; cin >> c;
        cost.push_back(c);
    }

    // DP[i][j], i is the index of city, j is the state
    // State 0: city i is repaired, the whole tree from i satisfies the condition
    // State 1: city i is not repaired, the whole tree from i satisfies the condition
    // State 2: city i is not repaired, the tree doesn't satisfy the condition, however all its subtrees do
    vector<vector<int> > DP(n, vector<int>(3));
    dfs(0, G, cost, DP);

    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < 3; j++) {
    //         cout << DP[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    cout << min(DP[0][0], DP[0][1]) << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}