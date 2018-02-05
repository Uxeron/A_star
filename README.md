# A_star
A* pathfinding implementation in C++

Only the "a_star.cpp" and "a_star.h" files are needed.
When compiling with g++ add -O2 flag for a speedup of up to 6x (tested: 0.36s vs 1.36s)

Usage:
	First, call the initAStar function with height and width as arguments to generate a node grid
	Then, create a 2D char vector, storing "walls" as 1's (true) and empty space as 0's (false)
		Note: a 2D bool vector shouldn't be used, since it offers very little memory saving but decreases the performance by a lot
	Then, an empty pair<int, int> vector should be created, for storing the path
	Finally, the AStar function can be called, passing the path vector, the wall 2D vector, the start coordinate pair and end coordinate pair as arguments
	The path will be written into the path vector