#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

int find_ancestor(int limit, 
                  vector<int>& path, 
                  vector<int>& ages) {
    int l = 0; 
    int r = path.size() - 1;
    while (l < r) {
        int mid = (l + r) / 2;
        if (ages[path[mid]] <= limit) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    return path[l];
}

void dfs(int root, 
         vector<int>& path, 
         vector<vector<int> >& G, 
         vector<int>& ages,
         vector<vector<int> >& queries, 
         map<pair<int, int>, int>& answers) {
    path.push_back(root);
    for (int limit : queries[root]) {
        answers[make_pair(root, limit)] = find_ancestor(limit, path, ages);
    }
    for (int child : G[root]) {
        dfs(child, path, G, ages, queries, answers);
    }
    path.pop_back();
}

void testcase() {
    int n, q; cin >> n >> q;
    vector<string> ind2name;
    map<string, int> name2ind;
    vector<int> ages;
    int max_age = 0;
    int root = 0;
    for (int i = 0; i < n; i++) {
        string s; cin >> s;
        int a; cin >> a;
        name2ind[s] = i;
        ind2name.push_back(s);
        ages.push_back(a);
        if (a > max_age) {
            root = i;
            max_age = a;
        }
    }

    vector<vector<int> > G(n);
    for (int i = 0; i < n - 1; i++) {
        string s, p; cin >> s >> p;
        // cout << s << " " << p << endl;
        G[name2ind[p]].push_back(name2ind[s]);
    }

    vector<pair<int, int> > queries_with_order;
    vector<vector<int> > queries(n);
    for (int i = 0; i < q; i++) {
        string name; cin >> name;
        int b; cin >> b;
        queries_with_order.push_back(make_pair(name2ind[name], b));
        queries[name2ind[name]].push_back(b);
    }

    map<pair<int, int>, int> answers;
    vector<int> path;

    dfs(root, path, G, ages, queries, answers);
    for (int i = 0; i < q; i++) {
        cout << ind2name[answers[queries_with_order[i]]];
        if (i != q - 1) {
            cout << " ";
        }
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        testcase();
    }
}