# Idea

DP on the number of different drinks and the total consumption. Each entry contains the minmimal cost and the maximal number of drink types under such cost.

We can keep two tables to clarify the process. 
DP[i][j] stores the min cost with the first i drinks that has at least j liters, as well as its . 
MEM[i][j] stores the min cost with the first i dirnks that has at least j liters, while the i-th drink must be used.