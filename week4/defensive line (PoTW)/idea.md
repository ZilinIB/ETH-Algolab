# Idea

Using dynamic programming, DP[i][j] is the maximal value that can be achieved with exactly i attackers and j defenders. 

    ##Initialization: 
    
    DP[i][j] == -1, DP[0][j] = 0;

    ## Iteration
    DP[i][j] = max(DP[i][j - 1], t)

    where 
        t = DP[i - 1][lo(j)] + j - lo (j), if lo(j) exists and DP[i - 1][lo(j)] > 0
          = -1 otherwise
    # lo(j) is the value such that psum(j) - psum(lo(j)) == k