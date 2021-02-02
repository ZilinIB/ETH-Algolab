# Idea

The idea is to first compute the prim MST from that point. Then compute the bottleneck (weights of the most weighted segment on that path) between a random pair of points. We can achieve that by iterating over all points and do BFS from each of them.