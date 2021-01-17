#include <iostream>
#include <vector>
#include <tuple>
#include <stack>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <stdlib.h>

using namespace std;
typedef tuple<long, long, long, int> Route;
typedef CGAL::Exact_predicates_exact_constructions_kernel K;

// return -1 if curr biker fails
// return 0 if both biker remains
// return 1 if curr biker wins
int compare(Route curr_biker, Route last_biker) {
    long y0c = get<0>(curr_biker);
    long x1c = get<1>(curr_biker);
    long y1c = get<2>(curr_biker);
    long y0l = get<0>(last_biker);
    long x1l = get<1>(last_biker);
    long y1l = get<2>(last_biker);
    
    bool conflict = double(y1c - y0c) * double(x1l) < double(y1l - y0l) * double(x1c);
    if (!conflict) {
      return 0;
    } else {
      bool win = double(abs(y1c - y0c)) * double(x1l) < double(abs(y1l - y0l)) * double(x1c);
      if (win) {
        return 1;
      } else {
        return -1;
      }
    }
}

void add_biker(Route curr_biker, stack<Route>& remaining_bikers) {
    if (remaining_bikers.empty()) {
      remaining_bikers.push(curr_biker);
      return;
    }
    Route last_biker = remaining_bikers.top();
    while ( compare(curr_biker, last_biker) == 1 && 
            !remaining_bikers.empty()) {
        remaining_bikers.pop();
        if (!remaining_bikers.empty()) {
            last_biker = remaining_bikers.top();
        }
    }

    if (!(compare(curr_biker, last_biker) == -1)) {
        remaining_bikers.push(curr_biker);
    }
 
}

void testcase() {
    int n; cin >> n;
    vector<Route> bikers;
    for (int i = 0;i < n; i++) {
        long y0, x1, y1;
        cin >> y0 >> x1 >> y1;
        bikers.push_back(make_tuple(y0, x1, y1, i));
    }
    sort(bikers.begin(), bikers.end());
    stack<Route> remaining_bikers;
    for (auto biker_it = bikers.begin(); biker_it != bikers.end(); biker_it++) {
        auto biker = *biker_it;
        add_biker(biker, remaining_bikers);
    }
    vector<int> res;
    while (!remaining_bikers.empty()) {
        Route biker = remaining_bikers.top();
        remaining_bikers.pop();
        res.push_back(get<3>(biker));
    }
    sort(res.begin(), res.end());
    for (int ind : res) {
        cout << ind << " ";
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; i++) {
        testcase();
    }
}