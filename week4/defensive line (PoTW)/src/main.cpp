#include <iostream>
#include <vector>
//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

int find_lo(vector<int>& v, vector<int>& psum, int j, int k, int& prev_lo) {

    int lo = prev_lo;
    if (lo < 0) {
        lo = 1;
    }
    int sum = psum[j] - psum[prev_lo - 1];
    while(lo > 0 && sum > k) {
        sum = psum[j] - psum[lo];
        lo++; 
    }
    if (sum == k) {
        prev_lo = lo;
        return lo;
    } else {
        return -1;
    }
}

void testcase() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> v;
    vector<int> psum;
    psum.push_back(0);
    int sum = 0;
    for (int i = 0; i < n; i++) {
        int a; cin >> a;
        v.push_back(a);
        sum += a;
        psum.push_back(sum);
    }
    
    //cout << "here" << endl;
    vector<vector<int> > DP(m+1, vector<int>(n+1, -1));
    vector<vector<int> > aux(m+1, vector<int>(n+1, -1));

    for (int j = 0; j <= n; j++) {
        DP[0][j] = 0;
        aux[0][j] = 0;
    }
    int res = 0;
    for (int i = 1; i <= m; i++) {
        int prev_lo = 1;
        for (int j = 1; j <= n; j++) {
            
            if (psum[j] < k) {
                DP[i][j] = -1;
            } else {
                int lo = find_lo(v, psum, j, k, prev_lo);
                if (lo > 0) {
                    /*
                    bool found = false;
                    int max_prev = 0;
                    for (int hi = lo - 1; hi >= 0; hi--) {
                        if (DP[i - 1][hi] >= 0) {
                            found = true;
                            if (DP[i - 1][hi] > max_prev) {
                                max_prev = DP[i - 1][hi];
                            }
                        }
                    }
                    if (found) {
                        DP[i][j] = max_prev + j - lo + 1;
                        //cout << i << ", " << j << ", " << lo << ", " << max_prev << endl;
                    }
                    */

                    if (aux[i - 1][lo - 1] >= 0) {
                        DP[i][j] = aux[i - 1][lo - 1] + j - lo + 1;
                    }
                }
            }
            aux[i][j] = max(aux[i][j - 1], DP[i][j]);

            //cout << DP[i][j] << " ";
        }
        
        
    }

    for (int j = 0; j <= n; j++) {
        if (DP[m][j] > res) {
            res = DP[m][j];
        }
    }
    if (res == 0) {
        cout << "fail" << endl;
    } else {
        cout << res << endl;
    }


}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
}