# Idea

Solve the problem with DP. Imagine that there are only two players. Player A (the combination of all other player) moves m - 1 times consecutively, player B moves only once (the k-th player). DP[i][j] represents the maximal total income of the current player. The transition rule is as following:

    DP[i][j] = 
        max(val[i] + DP[i + 1][j], val[j] + DP[i][j - 1]),if this round and next round are both A's turn
        
        psum[i][j] - min(DP[i + 1][j], DP[i][j - 1]), otherwise

# Remark

The behavior of % (mod operator) in c++ is not well defined when operand is negative! Try to make both operands positive before operation!
     