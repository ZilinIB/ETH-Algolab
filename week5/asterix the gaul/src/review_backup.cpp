#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool compare (pair<long, long> p1, pair<long, long> p2) {
    if (p1.second == p2.second) {
        return p1.first > p2.first;
    }
    return p1.second < p2.second;
}

bool feasible(long inc, long D, long T, vector<pair<long, long> > movements) {
    for (auto &p : movements) {
        p.first += inc;
    }

    int m = movements.size();
    vector<pair<long, long> > S1 = vector<pair<long, long> > (movements.begin(), movements.begin() + m / 2);
    vector<pair<long, long> > S2 = vector<pair<long, long> > (movements.begin() + m / 2, movements.end());

    vector<pair<long, long> > sub_S1, sub_S2;
    for (int s = 0; s < 1<<S1.size(); ++s) { // Iterate through all subsets
        long tot_D = 0, tot_T = 0;
        for (int i = 0; i < S1.size(); ++i) {
            // If i-th element in subset
            if (s & 1<<i) {
                tot_D += S1[i].first;
                tot_T += S1[i].second;
            } 
        }
        sub_S1.push_back(make_pair(tot_D, tot_T));
    }
    for (int s = 0; s < 1<<S2.size(); ++s) { // Iterate through all subsets
        long tot_D = 0, tot_T = 0;
        for (int i = 0; i < S2.size(); ++i) {
            // If i-th element in subset
            if (s & 1<<i) {
                tot_D += S2[i].first;
                tot_T += S2[i].second;
            } 
        }
        sub_S2.push_back(make_pair(tot_D, tot_T));
    }

    sort(sub_S1.begin(), sub_S1.end(), compare);
    sort(sub_S2.begin(), sub_S2.end(), compare);

    long max_d = 0; 
    for (auto & p : sub_S1) {
        if (max_d > p.first) {
            p.first = max_d;
        } else {
            max_d = p.first;
        }
    }

    max_d = 0; 
    for (auto & p : sub_S2) {
        if (max_d > p.first) {
            p.first = max_d;
        } else {
            max_d = p.first;
        }
    }


    bool found = false;
    int ptr1 = 0, ptr2 = sub_S2.size() - 1;
    // cout << "! ";
    while (ptr1 < sub_S1.size() && ptr2 >= 0) {
        long tot_D = sub_S1[ptr1].first + sub_S2[ptr2].first;
        long tot_T = sub_S1[ptr1].second + sub_S2[ptr2].second;
        // cout << ptr1 << " " << ptr2 << " " << tot_D << " " << tot_T << endl;
        if (tot_D >= D) {
            ptr2--;
            if (tot_T < T) {
                found = true;
                break;
            }
        } else {
            ptr1++;
        }
    }
    return found;
}

void testcase() {
    int n, m;
    long D, T;
    cin >> n >> m >> D >> T;
    vector<pair<long, long> > movements;
    for (int i = 0; i < n; i++) {
        long d, t; cin >> d >> t;
        movements.push_back(make_pair(d, t));
    }
    vector<long> reduction;
    reduction.push_back(0);
    for (int i = 0; i < m; i++) {
        long s; cin >> s;
        reduction.push_back(s);
    }

    int l = 0, r = m;
    while (l < r) {
        int mid = (l + r) / 2;
        if (feasible(reduction[mid], D, T, movements)) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    if (feasible(reduction[l], D, T, movements)) {
        cout << l << endl;
    } else {
        cout << "Panoramix captured" << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}