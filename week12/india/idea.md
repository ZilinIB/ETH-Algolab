# Idea

Binary search + min cost max flow.

If we have a fixed number of luggages and want to compute the minimal cost, the problem is then trivially a min cost max flow problem. However we now have the maximal budget and want to compute the max flow. We then conduct an exponential search on the number of luggage to approximate a range. Then a binary search to find the maximal number of luggage that can be carried under the required budget.