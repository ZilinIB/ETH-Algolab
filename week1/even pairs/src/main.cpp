#include <vector>
#include <iostream>

using namespace std;

void testcase() {
    int n; cin >> n;
    // vector<int> psum;
    int curr_sum = 0;
    int E = 0;

    for (int i = 0; i < n; i++) {
        int curr; cin >> curr;
        curr_sum += curr;
        // psum.push_back(curr_sum);
        if (curr_sum % 2 == 0) {
            E++;
        }
    }
    int O = n - E;
    
    cout << O * (O - 1) / 2 + E * (E + 1) / 2 << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}