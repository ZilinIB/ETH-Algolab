# Idea

If this problem is on an interval, the solution would just be a trivial greedy strategy. However, on a ring, it's not clear on the first sight how to find where to start. For this task, it's guaranteed that there exists at least one point where fewer than 10 jedis are guarding. We can start then by iterating over all these option.

# Remark

The trick for this task is how to find the point where the least Jedi's are overlapping. To achieve that, we keep a vector ***counts*** that keeps track of how many Jedis enter and leave a position. We iterate over all the jedis, ***counts[start]++***, ***counts[end + 1]--***, also record how many jedis cover the position 0 using the variable ***overlap***. After that, we iterate over all the positions, starting from 0. The position 0 is visited ***overlap*** times, and for position 1, it's visited ***overlap + counts[1]*** times. In this way, we can find exactly how many times a position is visited. Therefore find the least visited one.