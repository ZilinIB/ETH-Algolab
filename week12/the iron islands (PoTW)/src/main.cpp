#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>

using namespace std;

struct subpath {
    int tot_demand;
    int path_ind;
    int pos;

    subpath(int t, int i, int p) : tot_demand(t), path_ind(i), pos(p) {}
};

bool operator < (const subpath& s1, const subpath& s2) {
    if (s1.tot_demand == s2.tot_demand) {
        return s1.pos < s2.pos;
    }
    return s1.tot_demand < s2.tot_demand;
}

void testcase() {
    int n, k, w;
    cin >> n >> k >> w;
    vector<int> capacity;
    for (int i = 0; i < n; i++) {
        int tmp; cin >> tmp;
        capacity.push_back(tmp);
    }

    int ind = 1;

    int max_len = 0;
    vector<vector<subpath> > data(w);
    vector<subpath> all_data;
    for (int i = 0; i < w; i++) {
        int l; cin >> l;
        int tot_cap = 0;
        data[i].push_back(subpath(0, i, -1));
        for (int j = 0; j < l; j++) {
            int island; cin >> island;
            tot_cap += capacity[island];
            subpath curr(tot_cap, i, j);
            data[i].push_back(curr);
            all_data.push_back(curr);
        }

        if (data[i].back().tot_demand >= k) {
            int l = 0; 
            int r = 0;
            
            while(r < data[i].size()) {
                int curr_demand = data[i][r].tot_demand - data[i][l].tot_demand;
                if (curr_demand == k) {
                    if (r - l > max_len) {
                        ind = 1;
                    }
                    max_len = max(max_len, r - l);
                    r++;
                } else if (curr_demand < k) {
                    r++;
                } else {
                    l++;
                }
            }
        }
    }

    sort(all_data.begin(), all_data.end());
    int l = 0;
    int r = all_data.size() - 1;
    int goal = capacity[0] + k;
    while(l < r) {
        int curr_demand = all_data[l].tot_demand + all_data[r].tot_demand;
        if (curr_demand == goal) {
            if (all_data[l].path_ind != all_data[r].path_ind) {
                if (all_data[l].pos + all_data[r].pos + 1 > max_len &&
                    all_data[l].pos > 0 &&
                    all_data[r].pos > 0) {
                    ind = 2;
                }
                max_len = max(max_len, all_data[l].pos + all_data[r].pos + 1);
            }
            if (l + 1 < all_data.size() && all_data[l + 1].tot_demand == all_data[l].tot_demand) {
                l++;
            } else if (r - 1 >= 0 && all_data[r - 1].tot_demand == all_data[r].tot_demand) {
                r--;
            } else {
                l++;
            }
            
        } else if (curr_demand < goal) {
            l++;
        } else {
            r--;
        }
    }

    cout << max_len << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}
