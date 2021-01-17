#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <map>

#include <boost/graph/adjacency_list.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>	graph;
typedef boost::graph_traits<graph>::out_edge_iterator		out_edge_it;
typedef boost::graph_traits<graph>::edge_iterator		edge_it;

int search(vector<int>& path, int age, vector<int>& age_map) {
    int l = 0, r = path.size() - 1;
    while(l < r) {
        int mid = (l + r) / 2;
        // cout << mid << " ";
        if (age_map[path[mid]] <= age) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }
    // cout << endl;
    return path[l];
}

void dfs(graph& G, 
         int root, 
         vector<int>& path, 
         vector<vector<int> >& queries, 
         map<pair<int, int>, int>& results, 
         vector<bool>& has_query, 
         vector<int>& age_map) {

    path.push_back(root);

    if (!queries[root].empty()) {
        for (int age : queries[root]) {
            results[make_pair(root, age)] = search(path, age, age_map);
        }
    }

    out_edge_it oebeg, oeend;
    for (boost::tie(oebeg, oeend) = boost::out_edges(root, G); oebeg != oeend; oebeg++) {
        int child = boost::target(*oebeg, G);
        dfs(G, child, path, queries, results, has_query, age_map);
    }
    path.pop_back();
}

void testcase() {
    int n, q; 
    cin >> n >> q;
    vector<string> name_map;
    vector<int> age_map;
    map<string, int> index_map;

    int root;
    int max_age = 0;
    for (int i = 0; i < n; i++) {
        string name; cin >> name;
        int age; cin >> age;
        name_map.push_back(name);
        index_map[name] = i;
        age_map.push_back(age);
        if (age > max_age) {
            root = i;
            max_age = age;
        }
    }

    graph G(n);

    for (int i = 0; i < n - 1; i++) {
        string s, p; cin >> s >> p;
        boost::add_edge(index_map[p], index_map[s], G);
    }

    // for (int i = 0; i < n; i++) {
    //     cout << i << name_map[i] << endl;
    // }

    // edge_it ebeg, eend;
    // for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ebeg++) {
    //     int from = boost::source(*ebeg, G);
    //     int to = boost::target(*ebeg, G);
    //     cout << name_map[from] << " " << name_map[to] << endl;
    // }

    // cout << "root: " << name_map[root] << endl;

    vector<vector<int> > queries(n);
    map<pair<int, int>, int> results;
    vector<pair<int, int> > query_order;
    vector<bool> has_query(n, false);
    for (int i = 0; i < q; i++) {
        string name; cin >> name;
        int age; cin >> age;
        int ind = index_map[name];
        queries[ind].push_back(age);
        has_query[ind] = true;
        query_order.push_back(make_pair(ind, age));
    }

    vector<int> path;
    // path.push_back(root);
    dfs(G, root, path, queries, results, has_query, age_map);

    for (int i = 0; i < q; i++) {
        cout << name_map[results[query_order[i]]];
        if (i < q - 1) {
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