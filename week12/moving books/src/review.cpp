#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool feasible(vector<int>& strength, vector<int>& weights, int rounds) {
    int curr = 0;
    for (int s : strength) {
        if (curr >= weights.size()) {
            return true;
        } else if (s >= weights[curr]) {
            curr += rounds;
        } else {
            break;
        }
    }
    return curr >= weights.size();
}

void testcase() {
    int n, m; cin >> n >> m;
    vector<int> strength, weights;
    for (int i = 0; i < n; i++) {
        int tmp; cin >> tmp;
        strength.push_back(tmp);
    }
    for (int i = 0; i < m; i++) {
        int tmp; cin >> tmp;
        weights.push_back(tmp);
    }

    
    sort(strength.rbegin(), strength.rend());
    sort(weights.rbegin(), weights.rend());

    if (strength[0] < weights[0]) {
        cout << "impossible" << endl;
        return;
    }

    int l = 1, r = m;
    while (l < r) {
        int mid = (l + r) / 2;
        if (feasible(strength, weights, mid)) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }
    cout << l * 3 - 1 << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}