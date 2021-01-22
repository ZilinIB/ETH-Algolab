#include <iostream> 
#include <vector>

using namespace std;

void testcase() {
    int n, m, k;
    cin >> n >> m >> k;

    vector<int> def_val;
    vector<int> psum;
    int sum = 0;
    psum.push_back(sum);
    for (int i = 0; i < n; i++) {
        int temp; cin >> temp;
        def_val.push_back(temp);
        sum += temp;
        psum.push_back(sum);
    }
    
    vector<int> find_lo(n + 1, -1);
    int l = 0;
    for (int j = 1; j <= n; j++) {
        while(psum[j] - psum[l] > k && l <= j) {
            l++;
        }
        if (psum[j] - psum[l] == k) {
            find_lo[j] = l;
        }
    }

    vector<vector<int> > DP(m + 1, vector<int>(n + 1, -1));
    // DP[i][j] is the maximal value when there're i attackers and j defenders
    for (int j = 0; j <= n; j++) {
        DP[0][j] = 0;
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int with_last_defender = -1;
            if (psum[j] >= k) {
                int lo = find_lo[j];
                if (lo >= 0 && DP[i - 1][lo] >= 0) {
                    with_last_defender = DP[i - 1][lo] + j - lo;
                }
            }
            DP[i][j] = max(DP[i][j - 1], with_last_defender);
        }
    }

    if (DP[m][n] <= 0) {
        cout << "fail" << endl;
    } else {
        cout << DP[m][n] << endl;
    }
    

}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}