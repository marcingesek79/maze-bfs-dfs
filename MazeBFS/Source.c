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
	{'#', '#', '.', '#', '.'},
	{'#', '.', '.', '#', '.'},
	{'.', '.', '#', '.', '.'},
	{'.', '.', 'E', '.', '#'}
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
void printMaze(struct Maze maze, struct Node** path, int pathSize) {
	for (int i = 0; i < maze.height; i++)
	{
		for (int j = 0; j < maze.width; j++)
		{
			if (maze.grid[i][j]->sign == 'S') {
				printf("S");
			}
			else if (maze.grid[i][j]->sign == 'E') {
				printf("E");
			}
			else if (isInPath(maze.grid[i][j], path, pathSize)) {
				printf("X");
			}
			else {
				printf("%c", maze.grid[i][j]->sign);
			}
			if (j == maze.width - 1) printf("\n");
			else printf(" ");
		}
	}
}

int isInPath(struct Node* node, struct Node** path, int pathSize) {
	//printf("%c", path[0]->sign);
	for (int i = 0; i < pathSize; i++)
	{
		//printf("%c", path[i]->sign);
		if (path[i] == node) {
			return 1;
		}
	}
	return 0;
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

struct Node* popFromQueue(struct Node*** queue, int* size) {
	struct Node* firstElem = queue[0];
	for (int i = 1; i < *size; i++)
	{
		queue[i - 1] = queue[i];
	}
	(*size)--;
	return firstElem;
}

struct Node** getPath(struct Maze* maze, int* pathSize, struct Node* endNode) {
	*pathSize = 0;
	struct Node* node = endNode;
	struct Node** path = malloc(MAZE_HEIGHT * MAZE_WIDTH * sizeof(struct Node*));
	path[*pathSize] = node;
	(*pathSize)++;
	while (node->parent != NULL) {
		node = node->parent;
		path[*pathSize] = node;
		(*pathSize)++;
	}
	return path;
}

struct Node** bfs(struct Maze* maze, struct Node** queue, int* queueSize, int* pathSize) {
	struct Node* startNode = getNodeBySign(*maze, 'S');
	struct Node* node;
	queue[*queueSize] = startNode;
	(*queueSize)++;
	startNode->isVisited = VISITED;
	while ((*queueSize) > 0) {
		node = popFromQueue(queue, queueSize);
		//printf("%d %d\n", node->x, node->y);
		int neighboursSize = 0;
		struct Node** neighbours = getNeighbours(*maze, node, &neighboursSize);
		for (int i = 0; i < neighboursSize; i++)
		{
			if (neighbours[i]->isVisited == NOT_VISITED) {
				neighbours[i]->parent = node;
				neighbours[i]->isVisited = VISITED;
				queue[*queueSize] = neighbours[i];
				(*queueSize)++;
				if (neighbours[i]->sign == 'E') {
					return getPath(maze, pathSize, getNodeBySign(*maze, 'E'));
				}
			}
		}
	}
}


int main() {
	struct Maze maze = { mazeInit(MAZE_HEIGHT, MAZE_WIDTH), MAZE_HEIGHT, MAZE_WIDTH };
	struct Node* myNode = getNodeByCoord(maze, 2, 2);
	int size;
	struct Node** neighbours = getNeighbours(maze, myNode, &size);
	int queueSize = 0;
	int pathSize = 0;
	struct Node** queue = malloc(MAZE_HEIGHT * MAZE_WIDTH * sizeof(struct Node*));
	struct Node** path = bfs(&maze, queue, &queueSize, &pathSize);
	printMaze(maze, path, pathSize);
	return 0;
}