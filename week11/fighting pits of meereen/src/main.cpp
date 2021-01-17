#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <cmath>

using namespace std;

typedef vector<int> VI;
typedef pair<int, int> status;

int n, k, m, M;
vector<int> fighters;
vector<int> POW_2;

int variety(int v) {
    VI vis(k + 1, 0);
    while(v != 0) {
        vis[v % 10] = 1;
        v /= 10;
    }
    int sum = 0;
    for (int elem : vis) {
        sum += elem;
    }
    return sum;
}

// keep only the last m-1 fighters
int cut(int q) {
    return q % M;
}

void testcase() {
    cin >> n >> k >> m;
    M = (int)pow(10, m - 1);
    fighters.clear();
    fighters.push_back(0);
    for (int i = 0; i < n; i++) {
        int type; cin >> type;
        type++;
        fighters.push_back(type);
    }

    // DP[i][j][status] represents the score after setting the i-th (i in 1~n) fighter with status while
    // j = the number of fighters in the left queue - the nunber of fighters in the right queue + 11
    vector<vector<map<status, int> > > DP(n + 1, vector<map<status, int> >(23));
    DP[0][11][make_pair(0, 0)] = 0;

    for (int i = 0; i < n; i++) {
        int next_type = fighters[i + 1];
        for (int j = 0; j < 23; j++) {
            for (auto p : DP[i][j]) {
                int l, r;
                int inc;

                // if add the next fighter to the left queue
                l = 10 * p.first.first + next_type;
                inc = 1000 * variety(l) - POW_2[abs(j - 11 + 1)];
                if (inc >= 0) {
                    int tmp = inc + p.second;
                    if (tmp > DP[i + 1][j + 1][make_pair(cut(l), p.first.second)]) {
                        DP[i + 1][j + 1][make_pair(cut(l), p.first.second)] = tmp;
                        // cout << "DP[" << i + 1 << "][" << j + 1 << "][(" << l << ", " << p.first.second << ")] = " 
                        //     << p.second << " + " << 1000 * variety(l) << " - " << (int)pow(2, abs(j - 12 + 1)) << endl;
                    }
                }

                // if add the next fighter to the right queue
                r = 10 * p.first.second + next_type;
                inc = 1000 * variety(r) - POW_2[abs(j - 11 - 1)];
                if (inc >= 0) {
                    int tmp = inc + p.second;
                    if (tmp > DP[i + 1][j - 1][make_pair(p.first.first, cut(r))]) {
                        DP[i + 1][j - 1][make_pair(p.first.first, cut(r))] = tmp;
                        // cout << "DP[" << i + 1 << "][" << j - 1 << "][(" << p.first.first  << ", " << r << ")] = " 
                        //     << p.second << " + " << 1000 * variety(r) << " - " << (int)pow(2, abs(j - 12 - 1)) << endl;
                    }
                }
            }
        }
    }

    int result = 0;
    for (int j = 0; j < 23; j++) {
        for (auto p: DP[n][j]) {
            if (p.second > result) {
                result = p.second;
            }
        }
    }
    
    cout << result << endl;


}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    int curr = 1;
    for (int i = 0; i <= 12; i++) {
        POW_2.push_back(curr);
        curr *= 2;
    }
    while(t--) {
        testcase();
    }
}
