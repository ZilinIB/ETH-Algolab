#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <limits.h>
#include <algorithm>

using namespace std;

typedef pair<int, int> Jedi;

bool compare (const Jedi j1, const Jedi j2) {
    return j1.second < j2.second;
}

// return the max number of jedis taken from jedis vector, 
// left index of starting jedi must be >= start, 
// right index of ending jedi must be <= end
int count_jedis(vector<Jedi>& jedis, int start, int end) {
    int count = 0;
    int left = start;
    for (auto j : jedis) {
        if (j.first < left) {
            continue;
        } else if (j.second > end) {
            break;
        } else {
            count++;
            left = j.second + 1;
        }
    }
    return count;
}

void testcase() {
    int n, m; cin >> n >> m;
    vector<Jedi> jedis;
    map<int, int> count;
    // count[0] represents the number of all jedis that cover 0
    // count[i] i != 0 represents the increment compared to i - 1

    for (int i = 0; i < n; i++) {
        int a, b; cin >> a >> b;
        a--; b--;
        if (a > b) {
            count[0]++;
        }
        jedis.push_back(make_pair(a, b));
        count[a]++;
        int tmp = (b + 1) % m;
        if (tmp != 0) {
            count[tmp]--;
        }
        
    }

    int least_count = INT_MAX;
    int cumu_count = 0;
    int start_point;

    for (auto p : count) {
        cumu_count += p.second;
        if (cumu_count < least_count) {
            least_count = cumu_count;
            start_point = p.first;
        }
    }

    // cout << "start: " << start_point << endl;

    // sort(jedis.begin(), jedis.end());

    vector<Jedi> jedis_over_start;
    vector<Jedi> others;
    for (auto j : jedis) {
        j.first = (j.first + m - start_point) % m;
        j.second = (j.second + m - start_point) % m;
        if (j.first > j.second || j.first == 0) {
            jedis_over_start.push_back(j);
            // cout << j.first << " : " << j.second << endl;
        } else {
            others.push_back(j);
        }
    }

    // cout << "others: " << endl;
    sort(others.begin(), others.end(), compare);
    // for (auto j : others) {
    //     cout << j.first << " : " << j.second << endl;
    // }

    int max_count = count_jedis(others, 1, m - 1);
    
    for (auto j : jedis_over_start) {
        int curr_count = 1 + count_jedis(others, (j.second + 1) % m, (j.first + m - 1) % m);
        max_count = max(curr_count, max_count);
    }

    cout << max_count << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}