# Idea

This problem is very similar to the H1N1 problem. The only difference is that, in this problem, there're multiple exits instead of only one. We can simply connect every exit node to the infinite node with an edge of proper weight.

# Remark

remember to sort the edge in reverse order to compute the maximal spanning tree.

remember to ignore repeating edge when iterating over the faces. Altougth the algorithm still works with repeated edges, the time complexity grows quite fast.