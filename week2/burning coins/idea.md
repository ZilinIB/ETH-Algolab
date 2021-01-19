# Idea

2d DP

DP[i][j] means the maximal profit of current player when the indices of the remaining coins range from i to j(inclusive).

Initialization: DP[i][i] = value of coin i

Iteration: DP[i][j] = psum[j] - psum[i - 1] - min(DP[i + 1][j], DP[i][j - 1])
