#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <climits>
#include <algorithm>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Line_2 L;

void testcase() {
  int m; cin >> m;
  int n; cin >> n;
  vector<P> p;
  for (int i = 0; i < m; i++) {
    int x; cin >> x;
    int y; cin >> y;
    p.push_back(P(x, y));
  }

  
  // read all map pieces and find all the path segments on every map piece
  vector<vector<bool>> point_in_map(n, vector<bool>());
  vector<vector<int>> segs_in_map(n, vector<int>());
  for (int i = 0; i < n; i++) {
    vector<P> tp;
    for (int j = 0; j < 6; j++) {
      int x; cin >> x;
      int y; cin >> y;
      tp.push_back(P(x, y));
    }
    
    for (int j = 0; j < 3; j++){
      if (CGAL::left_turn(tp[2*j], tp[2*j+1], tp[(2*j+2)%6])) {
        swap(tp[2*j], tp[2*j+1]);
      }
    }
    
    for (int j = 0; j < m; j++) {
      point_in_map[i].push_back(!CGAL::left_turn(tp[0], tp[1], p[j]) &&
                                !CGAL::left_turn(tp[2], tp[3], p[j]) &&
                                !CGAL::left_turn(tp[4], tp[5], p[j]));
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m - 1; j++) {
      if (point_in_map[i][j] && point_in_map[i][j + 1]) {
        segs_in_map[i].push_back(j);
      }
    }
  }
  
  int min_dist = INT_MAX;
  int e = 0;
  vector<int> count_seg(m - 1, 0);
  int count = 0;
  
  // iterate through the maps
  for (int i = 0; i < n; i++) {
    // remove the segs in t[i - 1]
    if (i > 0) {
      for (int seg : segs_in_map[i - 1]) {
        count_seg[seg]--;
        if (count_seg[seg] == 0) {
          count--;
        }
      }
    }
    
    while(count != m - 1 && e != n) {
      for (int seg : segs_in_map[e]) {
        count_seg[seg]++;
        if (count_seg[seg] == 1) {
          count++;
        }
      }
      e++;
    }
    
    if (count == m - 1 && min_dist > e - i) {
      min_dist = e - i;
    } else if (count < m - 1 && e >= n) {
      break;
    }
  }
  
  cout << min_dist << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int c; cin >> c;
  for (int i = 0; i < c; i++) {
    testcase();
  }
}