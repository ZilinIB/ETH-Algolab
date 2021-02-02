# Idea

Consider two scenarios,

    1. the segment is on one path(only one direction from the orgin)
    2. the segment covers two path(two direction from the origin)
   
for case 1, take one path for example. We compute all the partial sum from the origin. Using a sliding window to find the segment that exactly matches the sum of that segment. Record the maximal length of the segment.

for case 2, the segment must contain the origin. We record all the partial sums on all paths, keep the path index. Sort them according to the partial sum. If partial sums are them same, sort by the length of the sequence. Then again use a sliding window to find the match. Note that here we need to check if the two segments are on the same path. 