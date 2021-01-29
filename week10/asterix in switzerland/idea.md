# Idea

This problem is tricky to think, but easy to implement. Basically we need to first build a graph with source and target, with source connecting to all banks with positive balance and target to those with negative balance. We compute the minimal cut that contains the source and check if it is greater than the sum of all positive balance.

To prove it formally requires some more time, the idea is to draw some cases on the picture to convince yourself. To begin with, consider the case when the cut exactly split those with positive balance and those others, then adjusting the cut by removing one positive node or add one negative node.