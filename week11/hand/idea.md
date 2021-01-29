# Idea

This problem is quite tedious. First compute the D-Trig, list all the edges and their length, order them. Assume we have a helper function that given s, return the maximal number of families. Then we can do binary search to find the proper s as well given f.

The routine works as following, first we compute the CC of the graph. List all the CCs and their cardinality. Since k < 4, we can iterate over all the possibilities and return the results accordingly.