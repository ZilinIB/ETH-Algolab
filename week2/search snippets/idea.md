# Idea

Slidng window with two pointers, representing the start and end repectively. The program begins with both pointers at the beginning, then move the right pointer towards the end if the current snippet doesn't contain all words. Move the left pointer if it does. Keep track of the window size, return the smallest window size.

# Remark

Don't make too much assumptions on the limit values. Use built-in limits (for example INT_MAX) instead.