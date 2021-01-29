# Idea

This task is not difficult. One first needs to find all the ovservable gangsters and the minimal cost to spy on them using D-Trig. Then just run LP to find the minimal cost.

# Remark

Two things worth mentioning: 

    1. To check if a map contains a key, use m.find(key) == end, or m.count(key) == 0 to test
    2. Bear in mind that the results in the lp are all integers, we need to transform them into double manually if we want precise results.