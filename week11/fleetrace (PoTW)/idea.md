# Idea

At the first sight, this problem doesn't look like a min cost max flow problem. But similar to what we have seen in the casion royale problem, we can create a base flow and add edges that satisfy some properties with lower cost. Here we connect all the boats direct to the target vertex with some base cost. Then we connect the boats to the sailers with edges that have cost Base cost - spectular coef. Then all sailers connect to the target with edge that have 0 cost. 

The result is then the number of boats * base cost - flow cost