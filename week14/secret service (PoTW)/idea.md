# Idea

First we compute the shortest path from all agents to all shelters. Then do a binary search on the maximal time allowed. Then construct a bipartite graph containing only the agents and the shelters, an edge (a, s) with weight w exists if and only if the shortest path from a to s has weight w and w is <= the maximal time allowed (the variable in the binary search).

For the c = 1 case, the above strategy is already enough.

For the c = 2 case, we duplicate all the shelters. We also add an edge between (a, s') with the weight being w + e, while w is the original path length between a and s. e is the time it takes for an agent entering the shelter. It means that an agent enters the shelter's second slot.

# Remark

Be careful when computing the shortest path. If there's no path from i to j, then dijstra[i][j] gives INT_MAX. Which will cause overflow if we want to operate on it.