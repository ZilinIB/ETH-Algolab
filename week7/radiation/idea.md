# Idea

This problem also needs the binary search strategy to find the best parameter. However, since the computation cost increases exponentially with the degree of polynomial. It's better to first find an approximate range of the degree with exponential search. 

# Remark

One important idea in this problem is about how to force a margin in linear programs(no points on the boundary). The trick is to introduce force a margin with constant 1 for every inequality. (since all coef can scale accordingly, 1 can actually represent all positive numbers)
