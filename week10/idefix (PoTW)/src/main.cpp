#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <limits>
#include <utility>
#include <stdexcept>
#include <map>
#include <tuple>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

// BGL includes 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel    K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>     Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Edge_iterator                                Edge_iterator;
typedef pair<K::Point_2,int> IPoint;

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS    // Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
    >          graph;
typedef boost::graph_traits<graph>::vertex_descriptor    vertex_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<graph>::edge_iterator    edge_it;    // to iterate over all edges

int helper(int n, long s, const Delaunay& t, vector<pair<int, long> >& distance_to_trees, vector<tuple<int, int, long> >& edges) {
    vector<int> bones_in_shadow(n, 0);

    for (auto p : distance_to_trees){
        long dist = p.second;
        if (4 * dist <= s) {
            bones_in_shadow[p.first]++;
        }
    }

    // graph G(n);
    // for (auto t : edges) {
    //     long dist = get<2>(t);
    //     int from = get<0>(t);
    //     int to = get<1>(t);
    //     if (dist <= s) {
    //         boost::add_edge(from, to, G);
    //     }
    // }

    // std::vector<int> component_map(n);  // We MUST use such a vector as an Exterior Property Map: Vertex -> Component
  // int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 

    boost::disjoint_sets_with_storage<> uf(n);
    for (auto e : edges) {
        long dist = get<2>(e);
        int from = get<0>(e);
        int to = get<1>(e);     
        int c1 = uf.find_set(from);
        int c2 = uf.find_set(to);
        if (dist <= s) {
            uf.link(c1, c2);
        }  
    }

    map<int, int> bones_per_component;
    for (int i = 0; i < n; i++) {
        int comp = uf.find_set(i);
        bones_per_component[comp] += bones_in_shadow[i];
    }

    int a = 0;
    for (auto num : bones_per_component) {
        if (num.second > a) {
            a = num.second;
        }
    }
    return a;
}

void testcase() {
    int n, m, k;
    long s;
    cin >> n >> m >> s >> k;
    vector<IPoint> trees;
    vector<K::Point_2> bones;
    trees.reserve(n);
    bones.reserve(m);
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        trees.emplace_back(K::Point_2(x, y), i);
    }
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        bones.push_back(K::Point_2(x, y));
    }

    Delaunay t;
    t.insert(trees.begin(), trees.end());

    vector<pair<int, long> > distance_to_trees;
    distance_to_trees.reserve(m);
    for (auto bone : bones){
        auto tree = t.nearest_vertex(bone);
        long dist = (long)CGAL::squared_distance(bone, tree->point());
        distance_to_trees.push_back(make_pair(tree->info(), dist));
    }

    vector<tuple<int, int, long> > edges;
    edges.reserve(3 * n);
    for (Edge_iterator e = t.finite_edges_begin(); e!= t.finite_edges_end(); ++e) {
        long dist = (long)t.segment(e).squared_length();
        int i1 = e->first->vertex((e->second+1)%3)->info();
        int i2 = e->first->vertex((e->second+2)%3)->info();
        edges.emplace_back(i1, i2, dist);
    }

    int a = helper(n, s, t, distance_to_trees, edges);


    long l, r;
    if (a < k) {
        l = s;
        if (l == 0) {
            r = 1;
        } else {
            r = 2 * l;
        }
        
        while (helper(n, r, t, distance_to_trees, edges) < k) {
            l = r;
            r = 2 * l;      
        }
    } else {
        r = s;
        l = r / 2;
        while (l > 0 && helper(n, l, t, distance_to_trees, edges) >= k) {
            r = l;
            l = r / 2;
        }
    }
  
    while(l < r) {
        long mid = (l + r) / 2;
        if (helper(n, mid, t, distance_to_trees, edges) >= k) {
            r = mid;
        } else {
            l = mid + 1;
        }
    }

    assert (helper(n, l, t, distance_to_trees, edges) >= k);

    cout << a << " " << l << endl;
}

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}
