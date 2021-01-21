#include <iostream>
#include <vector>

using namespace std;

void testcase() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> val;
    vector<int> psum;
    val.push_back(0);
    psum.push_back(0);

    int curr_sum = 0;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        val.push_back(x);
        curr_sum += x;
        psum.push_back(curr_sum);
    }

    vector<vector<int> > DP(n + 1, vector<int>(n + 1, 0));
    for (int i = 0; i <= n; i++) {
        DP[i][i] = val[i];
    }

    for (int len = 1; len < n; len++) {
        for (int i = 1; i <= n - len; i++) {
            int j = i + len;
            int curr_player = (n - len - 1) % m;
            if (curr_player == k || curr_player == (m + k - 1) % m) {
                DP[i][j] = psum[j] - psum[i - 1] - min(DP[i + 1][j], DP[i][j - 1]);
            } else {
                DP[i][j] = max(DP[i][j - 1] + val[j], DP[i + 1][j] + val[i]);
            }
        }
    }

    int res = DP[1][n];
    if (k != 0) {
        res = psum[n] - res;
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