# Idea

We want to calculate the global min cut for every possible pair of s and t. Since there has to be at least one figure in the cut, with symmetry, we assume 0 is in S. Then iterate over all indices from 1 to n - 1. Compute the min cut from 0 to i and i to 0. Record the minumum value.