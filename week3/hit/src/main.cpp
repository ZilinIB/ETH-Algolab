#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <algorithm>
#include <random>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Ray_2 R;
typedef K::Point_2 P;
typedef K::Segment_2 S;

void testcase(int n) {
  long x; cin >> x;
  long y; cin >> y;
  long a; cin >> a;
  long b; cin >> b;
  
  // cout << "2" << endl;
  
  vector<vector<long>> segs(n, vector<long>());
  for (int i = 0; i < 4 * n; i++) {
    long temp; cin >> temp;
    segs[i / 4].push_back(temp);
  }
  shuffle(segs.begin(), segs.end(), default_random_engine(7));
  // cout << "3" << endl;
  
  R ray(P(x, y), P(a, b));
  for (auto it = segs.begin();  it != segs.end(); it++) {
    S seg(P((*it)[0], (*it)[1]), P((*it)[2], (*it)[3]));
    if (CGAL::do_intersect(ray, seg)) {
      cout << "yes" << endl;
      return;
    }
  }
  cout << "no" << endl;
  
}

int main() {
  ios_base::sync_with_stdio(false);
  while (true) {
    int n; cin >> n;
    if (n) {
      // cout << "1" << endl;
      testcase(n);
    } else {
      break;
    }
  }
}

