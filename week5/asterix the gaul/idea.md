# Idea

The idea is to binary search the potions needed. In the search, once the amount of the potion is fixed, the problem is reduced to a backpack problem. However, the time complexity will be D*T which is too high for using DP. On that end we use the split technique, to reduce the time complexity for a constant coefficient.