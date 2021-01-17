#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>

using namespace std;
typedef pair<int, int> Jedi;

bool compare(Jedi j1, Jedi j2) {
    return j1.second < j2.second;
}

// begin and end are exclusive
int max_count(vector<Jedi>& normal, int begin, int end) {
    int count = 0;
    int left = begin;
    for (Jedi j: normal) {
        if (j.first <= left || j.second >= end) {
            continue;
        }
        count++;
        left = j.second;
    }
    return count;
}

void testcase() {
    int n, m;
    cin >> n >> m;
    vector<Jedi> J;
    // helper vector to record the overlap. 
    map<int, int> count;
    int overlap = 0;
    for (int i = 0; i < n; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--; // reindex from 0
        J.push_back(make_pair(a, b));
        count[a]++;
        count[b+1]--;
        if (a > b || b == m - 1) {
            overlap++;
        }
    }

    int min_overlaps = 11;
    int start = 0;
    for (auto c : count) {
        // the number of overlaps at segments i
        overlap += c.second;
        if (overlap < min_overlaps) {
            start = c.first;
            min_overlaps = overlap;
        }
    }

    vector<Jedi> normal;
    vector<Jedi> over_start; 
    // reindex all segments from start
    for (auto j: J) {
        j.first = (j.first - start + m) % m;
        j.second = (j.second - start + m) % m;
        if (j.first <= j.second) {
            normal.push_back(j);
        } else {
            over_start.push_back(j);
        }
    }
    sort(normal.begin(), normal.end(), compare);

    int res = max_count(normal, 0, m);
    for (Jedi j: over_start) {
        res = max(res, max_count(normal, j.second, j.first) + 1);
    }

    cout << res << endl;

}

int main() {
    ios_base::sync_with_stdio(false);

    int t; cin >> t;
    while(t--){
        testcase();
    }
}
