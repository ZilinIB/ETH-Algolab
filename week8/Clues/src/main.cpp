// STL includes
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <stack>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/pending/disjoint_sets.hpp>

// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

using namespace std;
// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef std::pair<K::Point_2,Index> IPoint;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
        >					graph;
typedef boost::graph_traits<graph>::vertex_descriptor		vertex_desc;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<graph>::edge_iterator		edge_it;		// to iterate over all edges

EdgeV compute_emst(Index n, const Delaunay& t) {
  // extract edges and sort by (squared) length
  // This step takes O(n log n) time (for the sorting).
  EdgeV edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  sort(edges.begin(), edges.end(),
	    [](const Edge& e1, const Edge& e2) -> bool {
	      return get<2>(e1) < get<2>(e2);
            });

  // Compute EMST using Kruskal's algorithm. This step takes O(n alpha(n)) time
  // in theory; for all practical purposes alpha(n) is constant, so linear time.

  // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  EdgeV emst;
  // ... and process edges in order of increasing length
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // determine components of endpoints
    Index c1 = uf.find_set(get<0>(*e));
    Index c2 = uf.find_set(get<1>(*e));
    if (c1 != c2) {
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
      emst.push_back(*e);
      if (--n_components == 1) break;
    }
  }
  return emst;
}

void testcase() {
    int n, m, r;
    cin >> n >> m >> r;
    K::FT sq_r = K::FT(r) * K::FT(r);
    vector<IPoint> points;
    points.reserve(n);
    for (int i = 0; i < n; i++) {
        int x, y; cin >> x >> y;
        points.emplace_back(K::Point_2(x, y), i);
    }
    Delaunay t_tot;
    t_tot.insert(points.begin(), points.end());
    EdgeV emst = compute_emst(n, t_tot);
    graph G(n);
    // cout << 1;
    for (auto e : emst) {
        if (get<2>(e) <= sq_r) {
            Index c1 = get<0>(e);
            Index c2 = get<1>(e);
            boost::add_edge(c1, c2, G);
        }
    }

    stack<int> s;
    vector<bool> vis(n, false);
    vector<bool> is_white(n);
    vector<int> component_map(n);
    int ncc = 0;
    for (int i = 0; i < n; i++) {
        // cout << 2;
        if (!vis[i]) {
            s.push(i);
            vis[i] = true;
            is_white[i] = false;
            component_map[i] = ncc;
            while(!s.empty()) {
                // cout << 3;
                int top = s.top();
                s.pop();
                auto adj_pair = boost::adjacent_vertices(top, G);
                for (auto adj_curr = adj_pair.first; adj_curr != adj_pair.second; adj_curr++) {
                    int curr = *adj_curr;
                    if (!vis[curr]) {
                        s.push(curr);
                        vis[curr] = true;
                        is_white[curr] = !is_white[top];
                        component_map[curr] = ncc;
                    }       
                }
            }
            ncc++;
        }
    }

    vector<IPoint> white, black;
    for (auto p : points) {
        if (is_white[p.second]) {
            white.push_back(p);
            // cout << p.second << " ";
        } else {
            black.push_back(p);
        }
    }
    // cout << endl;

    // cout << white.size() << endl;
    // cout << black.size() << endl;

    Delaunay t_white, t_black;
    t_white.insert(white.begin(), white.end());
    t_black.insert(black.begin(), black.end());

    bool is_bipartite = true;
    for (auto e = t_white.finite_edges_begin(); e != t_white.finite_edges_end(); ++e) {
        if (is_bipartite) {
            if (t_white.segment(e).squared_length() <= sq_r) {
                is_bipartite = false;
                // cout << 1;
                break;
                
            }       
        } else {
            break;
        }
    }

    for (auto e = t_black.finite_edges_begin(); e != t_black.finite_edges_end(); ++e) {
        if (is_bipartite) {
            if (t_black.segment(e).squared_length() <= sq_r) {
                is_bipartite = false;
                // cout << 2;
                break;
                
            }       
        } else {
            break;
        }
    }

    for (int i = 0; i < m; i++) {
        int x_a, y_a, x_b, y_b; 
        cin >> x_a >> y_a >> x_b >> y_b;
        if (!is_bipartite) {

            cout << "n";
            continue;
        }
        K::Point_2 a(x_a, y_a);
        K::Point_2 b(x_b, y_b);
        if (CGAL::squared_distance(a, b) <= sq_r) {
            cout << 'y';
            continue;
        }
        auto neighbor_a = t_tot.nearest_vertex(a);
        auto neighbor_b = t_tot.nearest_vertex(b);
        if (CGAL::squared_distance(a, neighbor_a->point()) <= sq_r
            && CGAL::squared_distance(b, neighbor_b->point()) <= sq_r
            && component_map[neighbor_a->info()] == component_map[neighbor_b->info()]) {
            cout << 'y';
        } else {
            cout << "n";
        }
    }
    cout << endl;

}

int main() {
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    while(t--) {
        testcase();
    }
}