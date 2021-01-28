# Idea

First we need to check if this graph is a bipartit graph at all. We do that by compute a D-Trig on the whole point set. Then we compute the EMST. Based on the EMST we use the DFS to conduct a 2-coloring and compute the connected components.

Collect all the black points and white points repectively. Compute two Delaunay Triangulation. Iterate over all edges, find if any edge is smaller than the radius. If there is, then the graph is not bipartit.

For every pair of consult, check if the two points are within the range of some station. If they are, and those stations are in the same component, and the graph is bipartite. Then return true. Reture false elsewise.