# Idea

The main challenge is to compute the number of contour lines that need to be crossed when going from warehouse i to stadion j. For that we first compute the D-Trig for all the points including warehouses and stadions. Then we iterate over all contour lines, find the nearest neighbor of its center, check if the distance to its nearest neighbor is within the radius. If it is, mark it as a valid contour line, for every point, check if it's in the contour line. Just continue otherwise. Then iterating over all pairs of warehouse-stadion, all valid contour line to compute the number of contour lines that need to be crossed for each pair of warehouse-stadion.

Then what it remains is just formulate a LP problem and solve it. 