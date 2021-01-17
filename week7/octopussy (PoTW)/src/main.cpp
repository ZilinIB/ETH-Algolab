#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <utility>

using namespace std;

int time_needed(int target, vector<int>& t, vector<bool>& is_removed) {
    if (is_removed[target]) {
        return 0;
    }

    int n = t.size() - 1;
    int c1 = 2 * target;
    int c2 = 2 * target + 1;
    is_removed[target] = true;
    if (c2 > n) {
        return 1;
    } else {
        return 1 + time_needed(c1, t, is_removed) + time_needed(c2, t, is_removed); 
    }
}

void testcase() {
    int n; cin >> n;
    vector<int> t;
    vector<bool> is_removed(n + 1, false);
    vector<pair<int, int> > ti_pair;
    t.push_back(0);
    // ti_pair.push_back(make_pair(0, 0));
    for (int i = 0; i < n; i++) {
        int tmp; cin >> tmp;
        t.push_back(tmp);
        ti_pair.push_back(make_pair(tmp, i + 1));
    }

    sort(ti_pair.begin(), ti_pair.end());
    int curr_time = 0;
    for (auto p : ti_pair) {
        if (is_removed[p.second]) {
            continue;
        }
        int t_inc = time_needed(p.second, t, is_removed);
        curr_time += t_inc;
        if (curr_time > p.first) {
            cout << "no" << endl;
            return;
        }
    }

    cout << "yes" << endl;

}

int main() {
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}