#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <deque>
#include <set>

using namespace std;

vector<int> predecessors;
vector<int> temperature;
vector<bool> safe;
vector<vector<int> > G;
multiset<int> pred_temp;
int m;
int k;

void dfs(int curr) {
    predecessors.push_back(curr);
    int curr_temp = temperature[curr];
    pred_temp.insert(curr_temp);

    int popped_point = -1;
    int popped_temp = -1;
    if (pred_temp.size() > m) {
        popped_point = predecessors.rbegin()[m];
        popped_temp = temperature[popped_point];
        auto it = pred_temp.find(popped_temp);
        pred_temp.erase(it);
    }

    if (pred_temp.size() == m) {
        int max_temp = *pred_temp.rbegin();
        int min_temp = *pred_temp.begin();
        if (max_temp - min_temp <= k){
            safe[predecessors.rbegin()[m - 1]] = true;
        }
    }

    for (int child : G[curr]) {
        dfs(child);
    }

    predecessors.pop_back();
    auto it = pred_temp.find(curr_temp);
    pred_temp.erase(it);

    if (popped_point >= 0) {
        pred_temp.insert(popped_temp);
    }
}

void testcase() {
    int n, a, b; 
    cin >> n >> a >> b;
    m = a;
    k = b;

    temperature = vector<int>(n);
    for (int i = 0; i < n; i++) {
        int tmp; cin >> tmp;
        temperature[i] = tmp;
    }
    G = vector<vector<int> >(n);
    for (int i = 0; i < n - 1; i++) {
        int from, to;
        cin >> from >> to;
        G[from].push_back(to);
    }

    pred_temp.clear();
    predecessors.clear();
    predecessors.reserve(n);
    safe = vector<bool>(n, false);
    dfs(0);
    bool abort = true;
    for (int i = 0; i < n; i++) {
        if (safe[i]) {
            cout << i << " ";
            abort = false;
        }
    }
    if (abort) {
        cout << "Abort mission";
    }
    cout << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}
