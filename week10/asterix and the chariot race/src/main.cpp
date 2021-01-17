#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <utility>
#include <climits>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

VVI G;
VI cost;
VVI mem;

/*
    input -> curr, status
    curr: index of the current city
    status: 0 if repair[curr] is true and the subtree satisfy the condition
            1 if repair[curr] is false and the subtree satisfy the condition
            2 if repair[curr] is false, the subtree doesn't satisfy the condition but all children of this subtree satisfy the condition
    
    return: the optimal cost of the subtree start from curr while status is either 0, 1, 2,
            INT_MAX if the subtree with required status doesn't exist
*/
int helper(int curr, int status) {
    if (mem[curr][status] == -1) {
        int tot_cost;
        if (G[curr].size() == 0) {
            if (status == 0) {
                tot_cost = cost[curr];
            } else if (status == 2) {
                tot_cost = 0;
            } else {
                tot_cost = INT_MAX;
            }
        } else if (status == 0) {
            tot_cost = cost[curr];
            for (int child : G[curr]) {
                int c0 = helper(child, 0);
                int c1 = helper(child, 1);
                int c2 = helper(child, 2);
                int inc = min(c0, min(c1, c2));
                tot_cost += inc;
            }
        } else if (status == 1) {
            tot_cost = INT_MAX;

            for (int choice : G[curr]) {
                int curr_cost = 0;
                for (int child : G[curr]) {
                    if (child == choice) {
                        curr_cost += helper(child, 0);
                    } else {
                        curr_cost += min(helper(child, 0), helper(child, 1));
                    }
                }
                tot_cost = min(curr_cost, tot_cost);
            }
 
        } else {
            tot_cost = 0;
            for (int child : G[curr]) {
                int c = helper(child, 1);
                if (c == INT_MAX) {
                    tot_cost = INT_MAX;
                    break;
                } else {
                    tot_cost += c;
                }
            }
        }
        mem[curr][status] = tot_cost;
    }
    // cout << "[" << curr << ", " << status << "]: " << tot_cost << endl;
    return mem[curr][status];
}

void testcase() {
    int n; cin >> n;
    G.clear();
    G = VVI(n);
    cost.clear();
    cost.reserve(n);
    mem.clear();
    mem = VVI(n, VI(3, -1));
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        G[u].push_back(v);
    }
    for (int i = 0; i < n; i++) {
        int c; cin >> c;
        cost.push_back(c);
    }

    int tot_cost = min(helper(0, 0), helper(0, 1));
    cout << tot_cost << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}