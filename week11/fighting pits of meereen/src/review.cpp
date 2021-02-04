#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <cmath>

using namespace std;

struct State {
    // contains state encodings of the left and right queues. for example, 132 means the last 3 fighters were 1, 3 and 2
    int l, r;
    State(int l_, int r_) : l(l_), r(r_) {};
};

bool operator < (const State s1, const State s2) {
    if (s1.l == s2.l) {
        return s1.r < s2.r;
    } else {
        return s1.l < s2.l;
    }
}

// return the variety of the state encoding
int variety(int k) {
    set<int> s;
    while (k != 0) {
        int remainder = k % 10;
        s.insert(remainder);
        k /= 10;
    }
    return s.size();
}

// cut the state encoding so that it contains at most m - 1 fighters. (p = 10 ^ (m - 1))
int cut(int k, int p) {
    return k % p;
}

void testcase() {
    int n, k, m; cin >> n >> k >> m;
    vector<int> fighters;
    for (int i = 0; i < n; i++) {
        int x; cin >> x;
        fighters.push_back(x + 1);
    }

    int pow_10 = pow(10, m - 1);
    vector<int> pow_2; 
    int curr = 1;
    for (int i = 0; i <= 12; i++) {
        pow_2.push_back(curr);
        curr *= 2;
    }

    // DP[i][j] contains the state-score pair when first i fighters have entered and the current length difference p-q = j - 11
    // where p is the length of left queue, q is the lenght of the right queue
    vector<vector<map<State, int> > > DP(n + 1, vector<map<State, int> >(23));
    DP[0][11][State(0, 0)] = 0;
    for (int i = 0; i < n; i++) {
        int next_fighter = fighters[i];
        for (int j = 0; j <= 22; j++) {
            int curr_diff = j - 11;
            for (auto p : DP[i][j]) {
                State s = p.first;
                int score = p.second;

                // add current fighter to the left queue
                int diff_l = curr_diff + 1;
                int next_l = s.l * 10 + next_fighter;
                int variety_l = variety(next_l);
                int inc_l = 1000 * variety_l - pow_2[abs(diff_l)];
                State state_l = State(cut(next_l, pow_10), s.r);
                if (inc_l > 0) {
                    DP[i + 1][diff_l + 11][state_l] = max(inc_l + score, DP[i + 1][diff_l + 11][state_l]);
                    // cout << i + 1 << ", " << diff_l + 11 << ", (" << cut(next_l, pow_10) << " " << s.r << "): " << inc_l + score << endl;
                }

                // add current fighter to the right queue
                int diff_r = curr_diff - 1;
                int next_r = s.r * 10 + next_fighter;
                int variety_r = variety(next_r);
                int inc_r = 1000 * variety_r - pow_2[abs(diff_r)];
                State state_r = State(s.l, cut(next_r, pow_10));
                if (inc_r > 0) {
                    DP[i + 1][diff_r + 11][state_r] = max(inc_r + score, DP[i + 1][diff_r + 11][state_r]);
                    // cout << i + 1 << ", " << diff_r + 11 << ", (" << s.l << " " << cut(next_r, pow_10) << "): " << inc_r + score << endl;
                }
            }
        }
    }

    int max_score = 0;
    for (int i = 0; i <= 22; i++) {
        for (auto p : DP[n][i]) {
            if (p.second > max_score) {
                max_score = p.second;
            }
        }
    }

    cout << max_score << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}