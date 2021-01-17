#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <climits>

using namespace std;

void testcase() {
    int n; cin >> n;
    vector<pair<int, int> > boats;
    for (int i = 0; i < n; i++) {
        int l, p; 
        cin >> l >> p;
        boats.push_back(make_pair(p, l));
    }
    // cout << boats.size() << endl;
    sort(boats.begin(), boats.end());
    // cout << "here" << boats.size() << endl;
    int end_pos = boats.back().first;

    int count = 0;
    int curr_margin = INT_MIN;
    int curr_boat = -1;

    while (curr_margin <= end_pos && curr_boat != n - 1){
        // cout << count << " " << curr_margin << " " << end_pos << endl;
        int next_margin = INT_MAX;
        int next_boat = -1;
        for (int i = curr_boat + 1; i < n; i++) {
            int pos = boats[i].first;
            int length = boats[i].second;
            if (pos < curr_margin) {
                continue;
            } else if (pos >= next_margin) {    
                break;
            }

            int possible_margin = curr_margin + length;
            if (possible_margin < pos) {
                possible_margin = pos;
            }
            if (possible_margin < next_margin) {
                next_margin = possible_margin;
                next_boat = i;
            }
        }
        count++;
        curr_margin = next_margin;
        curr_boat = next_boat;
    }

    cout << count << endl;

}

int main() {
    ios_base::sync_with_stdio(false);

    int t; cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
}
