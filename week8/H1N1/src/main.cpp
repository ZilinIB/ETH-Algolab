#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K>      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Finite_faces_iterator  Face_iterator;


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, long> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type  weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor           vertex_desc;
typedef boost::property_map<weighted_graph, boost::vertex_index_t>::type index_map;

long calc_min_width(int i, vector<long>& min_width, const vector<vertex_desc>& pred, const weight_map& weights, const weighted_graph& G) {
    if (min_width[i] != -1) {
        return min_width[i];
    }
    if (i == 0) {
        min_width[0] = LONG_MAX;
        return min_width[0];
    } 

    auto edge_pair = boost::edge(i, pred[i], G);
    // cout << i << " " << pred[i] << endl;
    assert(edge_pair.second);
    min_width[i] = min(LONG_MAX - weights[edge_pair.first], calc_min_width(pred[i], min_width, pred, weights, G));
    return min_width[i];

}

void testcase(int n) {
    vector<K::Point_2> points;
    for (int i = 0; i < n; i++) {
        int x, y; cin >> x >> y;
        points.push_back(K::Point_2(x, y));
    }
    Delaunay t;
    t.insert(points.begin(), points.end());

    // give all finite faces a positive index, and all infinite faces index 0
    int ind = 1;
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {
        f->info() = ind++;
    }
    int V = ind;
    weighted_graph G(V);
    weight_map weights = boost::get(boost::edge_weight, G);
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {
        int curr_index = f->info();
        for (int i = 0; i < 3; i++) {
            auto neighbor = f->neighbor(i);
            auto p1 = f->vertex((i + 1) % 3)->point();
            auto p2 = f->vertex((i + 2) % 3)->point();
            int neighbor_index = (t.is_infinite(neighbor)) ? 0 : neighbor->info();
            auto e = boost::edge(curr_index, neighbor_index, G);
            if (!e.second) {
                boost::add_edge(curr_index, neighbor_index, LONG_MAX - (long)CGAL::squared_distance(p1, p2), G);
            } else{
                long curr_weight = LONG_MAX - (long)CGAL::squared_distance(p1, p2);
                if (curr_weight < weights[e.first]) {
                    weights[e.first] = curr_weight;
                }
            }
        }
    }

    index_map index = boost::get(boost::vertex_index, G);
    vector<vertex_desc> predecessors(V);
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(predecessors.begin(), index), boost::root_vertex(0));

    // cout << "number of edges: " << boost::num_edges(G) << endl;
    // auto e_p = boost::edges(G);
    // for (auto e_b = e_p.first; e_b != e_p.second; e_b++) {
    //     cout << "(" << boost::source(*e_b, G) << ", " << boost::target(*e_b, G) << "): " << weights[*e_b] << endl;
    // }
    // for (auto el : predecessors) {
    //     cout << el << ' ';
    // }
    // cout << endl;


    vector<long> min_width(V, -1);
    for (int i = 0; i < V; i++) {
        calc_min_width(i, min_width, predecessors, weights, G);
    }

    int m; cin >> m; 
    for (int i = 0; i < m; i++) {
        int x, y;
        long d;
        cin >> x >> y >> d;
        K::Point_2 p(x, y);
        long curr_d = CGAL::squared_distance(t.nearest_vertex(p)->point(), p);
        if (curr_d < d) {
            cout << 'n';
        } else {
            auto start_face = t.locate(p);
            if (t.is_infinite(start_face)) {
                cout << 'y';
            }else {
                int start_face_index = start_face->info();
                long min_d = min_width[start_face_index];
                if (min_d >= 4 * d) {
                    cout << 'y';
                } else {
                    cout << 'n';
                }
            }
        }
    }
    cout << endl;

}

int main() {
    ios_base::sync_with_stdio(false);
    int n; cin >> n;
    while(n) {
        testcase(n);
        cin >> n;
    }
}
