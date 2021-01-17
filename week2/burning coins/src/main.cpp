#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void testcase() {
  int n; cin >> n;
  vector<int> v;
  vector<int> psum;
  int sum = 0;
  psum.push_back(sum);
  for (int i = 0; i < n; i++) {
    int a; cin >> a;
    v.push_back(a);
    sum += a;
    psum.push_back(sum);
  }
  
  vector<vector<int>> m(n, vector<int>(n));
  
  for (int i = 0; i < n; i++) {
    m[i][i] = v[i];
    // cout << i << ": " << m[i][i] << endl;
    if (i < n - 1) {
      m[i][i + 1] = max(v[i], v[i + 1]);
      // cout << i << ", " << i + 1 << ": " << m[i][i + 1] << endl;
    }
  }
  
  for (int d = 2; d < n; d++) {
    for (int i = 0; i < n - d; i++) {
      int j = i + d;
      m[i][j] = psum[j + 1] - psum[i] - min(m[i + 1][j], m[i][j - 1]);
      // cout << i << ", " << j << ": " << m[i][j] << endl;
    }
  }
  
  cout << m[0][n - 1] << endl;
  
  
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}