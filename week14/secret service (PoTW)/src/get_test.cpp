#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>

using namespace std;

void testcase(int t) {
    int n, m, a, s, c, d;
    cin >> n >> m >> a >> s >> c >> d;
    vector<char> W;
    vector<int> X, Y, Z;
    for (int i = 0; i < m; i++) {
        char w;
        int x, y, z;
        cin >> w >> x >> y >> z;
        W.push_back(w);
        X.push_back(x);
        Y.push_back(y);
        Z.push_back(z);
    }

    vector<int> agents, shelters;
    for (int i = 0; i < a; i++) {
        int pos; cin >> pos;
        agents.push_back(pos);
    }

    for (int i = 0; i < s; i++) {
        int pos; cin >> pos;
        shelters.push_back(pos);
    }

    if (t == 8) {
        cout << n << " " << m << " " << a << " " << s << " " << c << " " << d << endl;
        for (int i = 0; i < m; i++) {
            cout << W[i] << " " << X[i] << " " << Y[i] << " " << Z[i] << endl;
        }
        for (int i = 0; i < a; i++) {
            cout << agents[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < s; i++) {
            cout << shelters[i] << " ";
        }
        cout << endl;
    }

}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; i++) {
        testcase(i);
    }

}