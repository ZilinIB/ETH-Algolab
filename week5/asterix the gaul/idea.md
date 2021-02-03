# Idea

The idea is to binary search the potions needed. In the search, once the amount of the potion is fixed, the problem is reduced to a backpack problem. However, the time complexity will be D*T which is too high for using DP. On that end we use the split technique, to reduce the time complexity for a constant coefficient.

# Remark

It's worth noting that when using for (auto t : v) to iterate over a vector, the variable t is always a copy of an element in v. If we want to modify v. We have to explicitly get the reference of that element using &.

It's also worth noting that whenever using a sliding window / double pointers approach, always try to make the sequence monotonic. In the case of this task, remember to postprocess the tot_time and tot_dist of all possible subsets after sorting by time. We need to keep that the distance in the latter element (which indicates its time will be larger than the former ones) is always larger than that of the former ones.