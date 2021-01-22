# Idea

It is actually about how to compute bridges in undirected graph, sadly there's no direct API provided in the BGL.

One can exploit the articulation points api to get the discovery time and low points value and use a dedicated algorithm to compute the bridge(introduced in the course Algorithmen und Wahrscheinlichkeit), which has a better aymptotic complexity. 

Or one can do a trick, build a new bipartit graph with the vertices being the vertices and edges in the original graph, a vert-vertex and an edge-vertex is connected if and only if this edge is connected to the that vertex in the original graph. Then one can compute the articulation points on the new graph, and check which of them are edge-vertex, which lead the solution.