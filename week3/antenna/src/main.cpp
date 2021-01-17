#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
// #include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Cartesian_converter.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/number_utils.h>
#include <vector>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
// typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K_sqrt;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;

double ceil_to_double(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

double ceil_to_square(const K::FT& x)
{
  double a = CGAL::to_double(x);
  a = ceil(sqrt(a));
  while(K::FT(a) * K::FT(a) < x) a++;
  while(K::FT(a - 1) * K::FT(a - 1) >= x) a--;
  return a;
}

void testcase(int n){
  vector<P> citizens;
  for (int i = 0; i < n; i++) {
    long x, y;
    cin >> x; cin >> y;
    citizens.push_back(P(x, y));
  }
  
  Min_circle mc(citizens.begin(), citizens.end(), true);
  Traits::Circle c = mc.circle();
  
  // std::cout << setprecision(0) << fixed << ceil(r) << endl;
  
  std::cout << setprecision(0) << fixed << ceil_to_square(c.squared_radius()) << endl;
  
}

int main() {
  ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while(n) {
    testcase(n);
    cin >> n;
  }
}
