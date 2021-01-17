#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

bool feasible(vector<int> friends, vector<int> boxes, int c) {
    int n = friends.size();
    int m = boxes.size();

    int i = 0, j = 0;
    while(i < n && j < m) {
        if (friends[i] < boxes[j]) {
            return false;
        } else {
            i++;
            j += c;
        }
    }
    return j >= m;
}

void testcase() {
    int n, m;
    cin >> n >> m;
    vector<int> friends;
    vector<int> boxes;
    for (int i = 0; i < n; i++) {
        int tmp; cin >> tmp;
        friends.push_back(tmp);
    }
    for (int i = 0; i < m; i++) {
        int tmp; cin >> tmp;
        boxes.push_back(tmp);
    }
    sort(friends.begin(), friends.end(), greater<int>());
    sort(boxes.begin(), boxes.end(), greater<int>());

    int l = 1, r = m;
    if (boxes[0] > friends[0]) {
        cout << "impossible" << endl;
    } else {
        while(l < r) {
            int c = (l + r) / 2;
            if (feasible(friends, boxes, c)) {
                r = c;
            } else {
                l = c + 1;
            }
        }
        cout << 3 * l - 1 << endl;
    }
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}
