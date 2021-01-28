# Idea

Duplicate the graph twice to generate the new graph. The edges in subgraph 0 all have capacity infinity. The edges in subgraph 1 all have capacity 1. For every photo node, there's an edge between its vertex in subgraph 0 and subgraph 1 with capacity being the number of photos at that position. 

The problem then reduces to computing the maximal flow from all stations in subgraph 0 to all stations in subgraph 1.