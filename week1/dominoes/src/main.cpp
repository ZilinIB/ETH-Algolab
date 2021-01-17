#include <iostream>
#include <vector>

void testcase(){
  int n; std::cin >> n;
  int max_reach = 0;
  std::vector<int> h;
  
  for (int i = 0; i < n; i++){
    int a; std::cin>>a;
    h.push_back(a);
  }
  
  for (int i = 0; i < n; i++){
    if (i + h[i] > max_reach){
      max_reach = i + h[i];
    }
    if (max_reach <= i + 1){
      std::cout << i + 1 << std::endl;
      return;
    }
  }
  std::cout << n << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for (int i = 0; i < t; i++){
    testcase();
  }
}