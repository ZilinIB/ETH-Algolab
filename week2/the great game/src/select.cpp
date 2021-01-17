#include <iostream>
#include <vector>
#include <climits>

using namespace std;

void testcase(bool print) {
  int n; cin >> n;
  int m; cin >> m;
  int r; cin >> r;
  int b; cin >> b;

  if (print) {
    cout << 1 << endl;
    cout << n << " " << m << endl;
    cout << r << " " << b << endl;
  }

  for (int i = 0; i < m; i++) {
    int from; cin >> from;
    int to; cin >> to;

    if (print) {
      cout << from << " " << to << endl;
    }
  }
  
}

int main() {
  int t; cin >> t;
  for (int i = 0; i < t; i++) {
    testcase(i == 22);
  }
}