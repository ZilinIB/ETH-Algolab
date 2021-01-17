#include <iostream>
#include <vector>
#include <cmath>

void testcase() {
  int n; std::cin >> n;
  int k; std::cin >> k;
  std::vector<int> v;     // length (n)

  for (int i = 0; i < n; i++){
    int val; std::cin >> val;
    v.push_back(val);
    // std::cout << psum[i] << " ";
  }
  
  int real_diff = v[0]-k;
  int min_diff = std::abs(real_diff);
  int left = 0, right = 0;
  int res_l = 0, res_r = 0;
  while(min_diff && left < n && right < n){
    if (real_diff < 0){
      if (right < n - 1){
        right++;
        real_diff += v[right];
      } else {
        break;
      }
    } else {
      if (left < n - 1){
        real_diff -= v[left];
        left++;
      } else {
        break;
      }
      
      if (left > right){
        right++;
        real_diff += v[right];
      }
    }
    
    int diff = std::abs(real_diff);
    // std::cout << left << " " << right << " " << real_diff << std::endl;
    if (diff < min_diff){
      min_diff = diff;
      res_l = left;
      res_r = right;
    }
    
  }
  
  std::cout << res_l << " " << res_r << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for (int i = 0; i < t; i++){
    testcase();
  }
}