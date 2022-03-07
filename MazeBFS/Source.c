#include<stdio.h>
#define VISITED 1
#define NOT_VISITED 0
#define MAZE_HEIGHT 5
#define MAZE_WIDTH 5

/*
	Pattern of the maze
	'.' - empty space
	'#' - blockade
*/
char mazePattern[MAZE_HEIGHT][MAZE_WIDTH] = {
	{'S', '.', '.', '.', '.'},
	{'.', '.', '.', '.', '.'},
	{'.', '.', '.', '.', '.'},
	{'.', '.', 'E', '.', '.'},
	{'.', '.', '.', '.', '.'}
};

struct Node {
	int x;
	int y;
	int isVisited;
	char sign;
	struct Node* parent;
};

struct Maze {
	struct Node*** grid;
	int height;
	int width;
};

// Prints the whole maze
void printMaze(struct Maze maze) {
	for (int i = 0; i < maze.height; i++)
	{
		for (int j = 0; j < maze.width; j++)
		{
			printf("%c", maze.grid[i][j]->sign);
			if (j == maze.width - 1) printf("\n");
			else printf(" ");
		}
	}
}

struct Node*** mazeInit(int height, int width) {
	// Initializing 2d array
	struct Node*** maze = malloc(height * sizeof(struct Node*));
	for (int i = 0; i < height; i++)
	{
		maze[i] = malloc(width * sizeof(struct Node*));
	}

	// Filling up the array
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			struct Node* node = malloc(sizeof(struct Node));
			node->x = j;
			node->y = i;
			node->isVisited = NOT_VISITED;
			node->sign = mazePattern[i][j];
			node->parent = NULL;
			maze[i][j] = node;
		}
	}

	return maze;
}

// Returns a node at (x,y)
struct Node* getNodeByCoord(struct Maze maze, int x, int y) {
	for (int i = 0; i < maze.height; i++)
	{
		for (int j = 0; j < maze.width; j++)
		{
			if (i == y && j == x) {
				return maze.grid[i][j];
			}
		}
	}
}

// Returns a node with 'S' or 'E' symbol
struct Node* getNodeBySign(struct Maze maze, char sign) {
	if (sign != 'S' && sign != 'E') return NULL;
	for (int i = 0; i < maze.height; i++)
	{
		for (int j = 0; j < maze.width; j++)
		{
			if (maze.grid[i][j]->sign == sign) {
				return maze.grid[i][j];
			}
		}
	}
	return NULL;
}

// Returns array of neighbours of a node
struct Node** getNeighbours(struct Maze maze, struct Node* node, int* size) {
	int idx = 0;
	struct Node** neighbours = malloc(4 * sizeof(struct Node*));
	// up
	if (node->y > 0) {
		if (maze.grid[node->y - 1][node->x]->sign != '#') {
			neighbours[idx] = maze.grid[node->y - 1][node->x];
			idx += 1;
		}
	}
	// right
	if (node->x < maze.width - 1) {
		if (maze.grid[node->y][node->x + 1]->sign != '#') {
			neighbours[idx] = maze.grid[node->y][node->x + 1];
			idx += 1;
		}
	}
	// down
	if (node->y < maze.height - 1) {
		if (maze.grid[node->y + 1][node->x]->sign != '#') {
			neighbours[idx] = maze.grid[node->y + 1][node->x];
			idx += 1;
		}
	}
	// left
	if (node->x > 0) {
		if (maze.grid[node->y][node->x - 1]->sign != '#') {
			neighbours[idx] = maze.grid[node->y][node->x - 1];
			idx += 1;
		}
	}

	*size = idx;
	return neighbours;
}


int main() {
	struct Maze maze = { mazeInit(MAZE_HEIGHT, MAZE_WIDTH), MAZE_HEIGHT, MAZE_WIDTH };
	struct Node* myNode = getNodeByCoord(maze, 2, 2);
	int size;
	struct Node** neighbours = getNeighbours(maze, myNode, &size);
	for (int i = 0; i < size; i++)
	{
		neighbours[i]->sign = 'B';
	}
	printMaze(maze);
	return 0;
}