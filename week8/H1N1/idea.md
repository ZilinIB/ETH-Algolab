# Idea

The problem is indeed to find the maximal radius with which a circle can reach the infinite face from its current location. The optimal solution will always be a path on the edge of a voronoi graph. 

We treat the face of the original graph as a vertex in the new graph, two vertices are connected if they are neighbors in the original graph. The width of the edge is the length of the edge between the two faces in the original graph. 

Then the problem reduced to finding the widest path from every vertex to the vertex that represents the infinite face. 

For that we can use MST with the edge weight being the inverse of the width (LONG_MAX - width). Since MST is used to finding the most narrow path(the bottleneck is then the minimal edge weight on the path in MST).

# Remark
EPICK uses double to compute the distance, therefore can be directly casted into long. However, more precise kernel can't.

When iterating over all neighboring pairs of faces, we should iterate use the face iterator instead of the edge iterator. Using the edge iterator will generate some bugs for unknown reason. (A guess is that the f->neighbor operation is not allowed for infinite face);