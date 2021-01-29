# Idea

Conduct a modified version of dfs, where we always keep track of the last m days. Their temps are kept in a multiset. If there's exacatly m elements in the temp, check the diff between the max and min, mark it as safe if the diff is within k.