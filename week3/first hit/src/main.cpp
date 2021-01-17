#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Ray_2 R;
typedef K::Point_2 P;
typedef K::Segment_2 S;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

void testcase(int n) {
  long x, y, a, b;
  cin >> x; cin >> y; cin >> a; cin >> b;
  
  P begin(x, y);
  P end(a, b);
  
  bool hits = false;
  R ray(begin, end);
  S curr_r;
  
  vector<S> segs;
  for (int i = 0; i < n; i++) {
    long p1, p2, p3, p4;
    cin >> p1 >> p2 >> p3 >> p4;

    segs.push_back(S(P(p1, p2), P(p3, p4)));
  }
  random_shuffle(segs.begin(), segs.end());
  
  for (auto seg : segs) {
    boost::optional<boost::variant<P, S>> o;
    if (hits && CGAL::do_intersect(seg, curr_r)) {
      o = CGAL::intersection(seg, curr_r);
    } else if (!hits && CGAL::do_intersect(seg, ray)) {
      o = CGAL::intersection(seg, ray);
      hits = true;
    } else {
      continue;
    }
    
    if (const P* op = boost::get<P>(&*o)) {
      curr_r = S(begin, *op);
    } else if (const S* os = boost::get<S>(&*o)) {
      if (CGAL::collinear_are_ordered_along_line(begin, os->source(), os->target())) {
        curr_r = S(begin, os->source());
      } else {
        curr_r = S(begin, os->target());
      }
    } else {
      throw runtime_error("strange intersection");
    }
    
  }
  
  if (hits) {
    cout << setprecision(0) << fixed << floor_to_double(curr_r.target().x()) << " " << floor_to_double(curr_r.target().y()) << endl;
  } else {
    cout << "no" << endl;
  }
  
}

int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while (n) {
    testcase(n);
    cin >> n;
  }
}

