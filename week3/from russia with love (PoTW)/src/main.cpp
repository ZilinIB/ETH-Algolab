#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void testcase() {
  int n; cin >> n;
  int m; cin >> m;
  int k; cin >> k;
  vector<int> x;
  vector<int> psum;
  int sum = 0;
  psum.push_back(sum);
  for (int i = 0; i < n; i++) {
    int a; cin >> a;
    x.push_back(a);
    sum += a;
    psum.push_back(sum);
  }
  
  vector<vector<int>> p(n, vector<int>(n, 0));
  // vector<vector<int>> o(n, vector<int>(n, 0));
  
  for (int i = 0; i < n; i++) {
    p[i][i] = x[i];
    // o[i][i] = x[i];
  }
  
  for (int dist = 1; dist < n; dist++) {
    int round = n - dist - 1;
    // cout << round << " " << m << " " << k << " " << (round % m == k) << endl;
    for (int i = 0; i < n - dist; i++) {
      int j = i + dist;
      int curr_sum = psum[j + 1] - psum[i];
      
      if (round % m == k  || round % m == (m + k - 1) % m ) {
        p[i][j] = curr_sum - min(p[i + 1][j], p[i][j - 1]);
      } else {
        p[i][j] = max(x[i] + p[i + 1][j], x[j] + p[i][j - 1]);
      }
    }
  }
  
  if (k == 0) {
    cout << p[0][n - 1] << endl;
  } else {
    cout << psum[n] - p[0][n - 1] << endl;
  }
  
}

int main() {
  int t; cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}