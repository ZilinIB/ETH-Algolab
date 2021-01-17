#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

void testcase(bool output) {
    int m, n, k, l;
    cin >> n >> m >> k >> l;
    vector<int> police_station;
    vector<int> photos;
    vector<pair<int, int> > streets;
    for (int i = 0; i < k; i++) {
        int tmp; cin >> tmp;
        police_station.push_back(tmp);
    }
    for (int i = 0; i < l; i++) {
        int tmp; cin >> tmp;
        photos.push_back(tmp);
    }
    for (int i = 0; i < m; i++) {
        int t1, t2; cin >> t1 >> t2;
        streets.push_back(make_pair(t1, t2));
    }

    // cout << 1 << " ";
    if (output) {
        cout << 1 << endl;
        cout << n << " " << m << " " << k << " " << l << endl;
        for (int i : police_station) {
            cout << i  << " ";
        }
        
        cout << endl;
        for (int i : photos) {
            cout << i << " ";
        }
        cout << endl;
        for (auto p : streets) {
            cout << p.first << " " << p.second << endl;
        }
    }
}

int main() {
    // ios_base::sync_with_stdio(false);
    // cout << "start" << endl;
    int t, target;
    cin >> t;
    // cout << "t" << endl;
    for (int i = 0; i < t; i++) {
        // cout << 2 ;
        testcase(i == 42);
    }
}