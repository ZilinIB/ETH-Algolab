#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int get_max_dist(int d1, int d2) {
  if (! (d1 >= 0 || d2 >= 0)) {
    cout << "wrong" << endl;
  }
  
  if (d1 >= 0 && d2 >= 0) {
    return max(d1, d2);
  } else if (d1 < 0) {
    return d2;
  } else {
    return d1;
  }
}

void testcase() {
  int n; cin >> n;
  vector<int> x;
  for (int i = 0; i < n; i++) {
    int a; cin >> a;
    x.push_back(a);
  }
  
  // reset the coordinates to start from -100
  int lo = *min_element(x.begin(), x.end());
  int hi = *max_element(x.begin(), x.end());
  int length = hi - lo + 1 + 200;
  vector<bool> beach(length, false);
  
  for (int i = 0; i < n; i++) {
    beach[x[i] - lo + 100] = true;
  }
  
  queue<int> q;
  
  // count the number and maximal distance with 0 as the starting point
  for (int i = 100; i <= 200; i++) {
    if (beach[i]) {
      q.push(i);
    }
  }
  
  int max_num = 0;
  int min_dist = 100;
  vector<int> solutions;
  // main logic, compute the number of parasols and search the optimal solutions
  for (int i = 101; i < length - 100; i++) {
    int l = i - 100;
    int r = i + 100;

    if (beach[l - 1]) {
      q.pop();
    }
    if (beach[r]) {
      q.push(r);
    }
    
    // compute the number of parasols
    int num_par = q.size();
    if (num_par > max_num) {
      max_num = num_par;
      min_dist = max(q.back() - i, i - q.front());
      solutions.clear();
      solutions.push_back(i);
    } else if (num_par == max_num) {
      int curr_dist = max(q.back() - i, i - q.front());
      if (curr_dist < min_dist) {
        min_dist = curr_dist;
        solutions.clear();
        solutions.push_back(i);
      } else if (curr_dist == min_dist) {
        solutions.push_back(i);
      }
    }
  }
  
  cout << max_num << " " << min_dist << endl;
  for (int s : solutions) {
    cout << s - 100 + lo << " ";
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