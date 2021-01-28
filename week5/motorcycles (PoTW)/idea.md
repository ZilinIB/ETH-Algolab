# Idea

Two methods for now:
1. First sort the bikers according to their starting points, calculating their slopes. Iterating over all the bikers from top to bottom, using a stack to keep track of the bikers that are yet alive (be able to ride forever). For every new biker, check the first element B_s on the stack, if its track will itersect the B_s's track. 
   
   Case a): If it will, and the absolute value of its slope is larger than that of B_s's, then ignore this one and move on. 

   Case b): If it will, and the absolute value of its slope is smaller than that of B_s's, then pop the last element on the stack and check the top element of the stack again.

   Case c): If it will not, then just push the new biker on the stack.

2. Keep track of 4 variables: 
   
    curr_pos_position,
   
    curr_pos_slope,

    curr_neg_positon,

    curr_neg_slope

   Sort the bikers according the the absolute values of the slope. Starting from the one with the smallest slope. Iterating over all the bikers, updating the 4 variables and add this biker to the list if the new biker doesn't conflict with the previous ones. Ignore this biker otherwise.