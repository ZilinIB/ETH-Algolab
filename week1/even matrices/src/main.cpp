#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<std::vector<int>> m(n + 1, std::vector<int>(n + 1, 0));
  std::vector<std::vector<int>> psum(n + 1, std::vector<int>(n + 1, 0));
  
  int a, res = 0;
  for (int i = 1; i <= n; i++){
    int col_sum = 0;
    for (int j = 1; j <= n; j++){
      std::cin >> a;
      m[i][j] = a;
      col_sum += a;
      psum[i][j] = col_sum;
    }
  }
  
  for (int j_1 = 0; j_1 < n; j_1++){
    for (int j_2 = j_1 + 1; j_2 <= n; j_2++){
      int E = 0;
      std::vector<int> temp;
      temp.push_back(0);
      for(int i = 1; i <= n; i++){
        temp.push_back(temp[i - 1] + psum[i][j_2] - psum[i][j_1]);
        if (temp[i] % 2 == 0){
          E++;
        }
      }
      int O = n - E;
      res += E + E * (E - 1) / 2 + O * (O - 1) / 2;
    }
  }
  
  std::cout << res << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for (int i = 0; i < t; i++){
    testcase();
  }
}