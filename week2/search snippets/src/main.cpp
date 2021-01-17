#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void testcase() {
  int n; cin >> n;
  vector<int> m;
  for (int i = 0; i < n; i++) {
    int a; cin >> a;
    m.push_back(a);
  }
  vector<vector<int>> p(n, vector<int>());
  vector<pair<int, int>> pos;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m[i]; j++) {
      int a; cin >> a;
      p[i].push_back(a);
//      pos.push_back(make_pair(a, i));
    }
  }
  
  
  
  // sort the (position, id) pairs with their position in ascending order
  // sort(pos.begin(), pos.end());
  
  // sort 2
  vector<pair<int, int>> heap;
  for (int i = 0; i < n; i++) {
    heap.push_back(make_pair(p[i].back(), i));
    // p[i].pop_back();
    p[i].erase(p[i].end() - 1);
  }
  make_heap(heap.begin(), heap.end());
  
  while(!heap.empty()) {
    pos.push_back(heap.front());
    int i = heap.front().second;
    pop_heap(heap.begin(), heap.end());
    heap.pop_back();
    if (!p[i].empty()) {
      heap.push_back(make_pair(p[i].back(), i));
      // p[i].pop_back();
      p[i].erase(p[i].end() - 1);
      push_heap(heap.begin(), heap.end());
    }
  }
  
  reverse(pos.begin(), pos.end());
  
  
  int N = pos.size();
  
  int count = 0; // count of distinct words 
  vector<int> word_count(n, 0); // count of individual words
  vector<int> t(N, -1); // t[i]: least j s.t. [i, j] contains all words
  int min_dist;
  
  // manually compute the first term
  int j = -1;
  while (count < n && j < N - 1) {
    j++;
    word_count[pos[j].second]++;
    if (word_count[pos[j].second] == 1) {
      count++;
    }
  }
  t[0] = j;
  min_dist = pos[j].first - pos[0].first + 1;
  
  // DP
  for (int i = 1; i < N; i++) {
    int lost_word = pos[i - 1].second;
    word_count[lost_word]--;
    if (word_count[lost_word] == 0) {
      count--;
    }
    
    int r = t[i - 1];
    while (count < n && r < N - 1) {
      r++;
      int new_word = pos[r].second;
      word_count[new_word]++;
      if (word_count[new_word] == 1) {
        count++;
      }
    }
    
    if (count != n) {
      break;
    } else {
      t[i] = r;
      int curr_dist = pos[r].first - pos[i].first + 1;
      if (curr_dist < min_dist) {
        min_dist = curr_dist;
      }
    }
    
  }
  
  cout << min_dist << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}