# Idea

This one is quite tricky. We should use DP. While for this particular task, the recursive form is easier than the table form. 

From the information we know that this graph is a directed tree.

We have DP[curr][status] means the minimal cost for the subtree starting from the city with index curr to have status. Status can be either 0, 1 or 2. 

    status 0: we repair the road in city curr and the condition for the whole subtree should hold
    status 1: we don't repair the road in city curr, but the condition still holds
    status 2: we don't repair the road in city curr, and the condition for this subtree doesn't hold, however, the condition for all the children of the subtree hold.(they all have status 1)

in the end, we return the min(DP[0][0], DP[0][1]), which is the minimal cost s.t. the condition for the whole tree holds.