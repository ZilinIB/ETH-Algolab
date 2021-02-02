# Idea

Basically we need to compute all the shortest path from source to target. We can achieve that by running the dijkstra algorithm on both source and target. Then iteratively check every edge on the graph. For a edge (u, v), check if dist_to_source(u) + dist_to_target(v) + weight(u, v) is equal to the distance of the shortest path from source to target. If not, set its capacity to 0. Then compute the max flow from source to target which is then the solution.

# Remark

For some reason, the graph for flow algorithm can not be used to compute the shortest paths. Therefore we need to generate two graphs, one for shortest path and another for the flow. 

Keep in mind that the edge_adder class that is provided in the course material will actually add two edges in the graph for one add_edge operation. One normal edge and one reverse edge. Therefore when transforming undirected edge into directed edge, the graph will contain multiple edges that have the same source and target. It is now unpredictable which edge the boost::edge API will return. The better solution when the number of vertices is small is to keep a 2-d vector of the edge descriptor.