# Idea

This problem is quite similar to the Deck of Cards problem in week2. First we check which segements are in which map parts. Then moving the two pointers based on the updating rule: the end pointer moves if the map doesn't yet contain all segments while the left pointer moves otherwise. We keep the cost during the whole run and output the least cost.

# Remark

The CGAL::left_turn predicates check if a point is to the left of the ray that is formed by other two points. It provides a great way to judge if a point is in the triangle without really constructing one. 

Check the edge values of a loop. For example: start at 0 or 1? End at n or n - 1?