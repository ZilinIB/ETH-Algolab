#include <iostream>
#include <vector>
#include <climits>

using namespace std;

void testcase() {
  int n; cin >> n;
  int m; cin >> m;
  int r; cin >> r;
  int b; cin >> b;
  
  vector<vector<int> > transition(n);
  for (int i = 0; i < m; i++) {
    int from; cin >> from;
    int to; cin >> to;
    transition[from].push_back(to);
  }
  
  vector<int> op(n + 1, -1);  // optimistic player: tries to minimize the steps
  vector<int> pe(n + 1, -1);  // pessimistic player: tries to maximize the steps
  
  op[n] = 0;
  pe[n] = 0;
  
  for (int u = n - 2; u > 0; u--) {
    if (transition[u].size() == 0) {
      continue;
    }
    
    int min = INT_MAX;
    int max = INT_MIN;
    for (int v : transition[u]) {
      min = pe[v] < min ? pe[v] : min;
      max = op[v] > max ? op[v] : max;
    }
    
    op[u] = 1 + min;
    pe[u] = 1 + max;
  }
  
  // cout << "r: " << op[r] << endl;
  // cout << "b: " << op[b] << endl;
  
  int res;
  if (op[r] > op[b]) {
    res = 1;
  } else if (op[r] == op[b]) {
    if (op[r] % 2 == 1) {
      res = 0;
    } else {
      res = 1;
    }
  } else {
    res = 0;
  }
  cout << res << endl;
  
}

int main() {
  int t; cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}