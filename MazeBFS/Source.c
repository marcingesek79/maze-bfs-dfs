#include<stdio.h>
#include<stdbool.h>
#define MAZE_HEIGHT 5
#define MAZE_WIDTH 5
#define START 'S'
#define END 'E'

/*
	Pattern of the maze
	'.' - empty space
	'#' - blockade
	'S' - start node
	'E' - end node
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
	bool isVisited;
	char type;
	struct Node* parent;
};

struct Maze {
	struct Node* grid[MAZE_HEIGHT][MAZE_WIDTH];
	struct Node** path;
	int pathSize;
};


// Checks whether the node is in the path
bool isInPath(struct Maze* maze, struct Node* node) {
	if (maze->path != NULL) {
		for (int i = 0; i < maze->pathSize; i++) {
			if (maze->path[i] == node) return true;
		}
	}
	return false;
}

// Prints the whole maze
void printMaze(struct Maze* maze, bool withPath) {
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_WIDTH; j++) {
			if (maze->grid[i][j]->type == START) {
				printf("S");
			}
			else if (maze->grid[i][j]->type == END) {
				printf("E");
			}
			else if (withPath && isInPath(maze, maze->grid[i][j])) {
				printf("X");
			}
			else {
				printf("%c", maze->grid[i][j]->type);
			}

			j == MAZE_WIDTH - 1 ? printf("\n") : printf(" ");
		}
	}
	printf("\n\n");
}

// Initalizes the maze
void mazeInit(struct Maze* maze) {
	for (int i = 0; i < MAZE_HEIGHT; i++)
	{
		for (int j = 0; j < MAZE_WIDTH; j++)
		{
			struct Node* node = malloc(sizeof(struct Node));
			node->x = j;
			node->y = i;
			node->isVisited = false;
			node->type = mazePattern[i][j];
			node->parent = NULL;
			maze->grid[i][j] = node;
		}
	}
	maze->path = NULL;
	maze->pathSize = 0;
}

// Returns a node with 'S' or 'E' symbol
struct Node* getNodeBySign(struct Maze* maze, char type) {
	if (type != START && type != END) return NULL;

	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_WIDTH; j++) {
			if (maze->grid[i][j]->type == type) {
				return maze->grid[i][j];
			}
		}
	}
	return NULL;
}

// Returns array of neighbours of a node
struct Node** getNeighbours(struct Maze maze, struct Node* node, int* size) {
	int i = 0;
	struct Node** neighbours = malloc(4*sizeof(struct Node*));

	// up
	if (node->y > 0) {
		if (maze.grid[node->y - 1][node->x]->type != '#') {
			neighbours[i++] = maze.grid[node->y - 1][node->x];
		}
	}
	// right
	if (node->x < MAZE_WIDTH - 1) {
		if (maze.grid[node->y][node->x + 1]->type != '#') {
			neighbours[i++] = maze.grid[node->y][node->x + 1];
		}
	}
	// down
	if (node->y < MAZE_HEIGHT - 1) {
		if (maze.grid[node->y + 1][node->x]->type != '#') {
			neighbours[i++] = maze.grid[node->y + 1][node->x];
		}
	}
	// left
	if (node->x > 0) {
		if (maze.grid[node->y][node->x - 1]->type != '#') {
			neighbours[i++] = maze.grid[node->y][node->x - 1];
		}
	}

	struct Node** tmp = realloc(neighbours, i * sizeof(struct Node*));
	if (tmp == NULL) {
		*size = 0;
		return NULL;
	}

	neighbours = tmp;
	*size = i;
	return neighbours;
}

// Pops a node from a queue
struct Node* queuePop(struct Node*** queue, int* size) {
	if (*size == 0) return NULL;
	struct Node* firstElem = (*queue)[0];
	
	for (int i = 1; i < *size; i++) {
		(*queue)[i - 1] = (*queue)[i];
	}
	
	if (*size == 1) {
		(*size)--;
		return firstElem;
	}

	struct Node** tmp = realloc(*queue, --(*size) * sizeof(struct Node*));
	if (tmp == NULL) {
		printf("error - pop");
		return NULL;
	}

	*queue = tmp;
	return firstElem;
}

// Pushes a node into a queue
void queuePush(struct Node* node, struct Node*** queue, int* size) {
	struct Node** tmp = realloc(*queue, ++(*size) * sizeof(struct Node*));
	
	if (tmp == NULL)
	{
		printf("error - push;");
		return;
	}

	tmp[*size - 1] = node;
	*queue = tmp;
}

// creates array with the nodes that belong to the path
void getPath(struct Maze* maze) {
	int pathSize = 0;
	struct Node* node = getNodeBySign(maze, END);
	struct Node** path = malloc(MAZE_HEIGHT * MAZE_WIDTH * sizeof(struct Node*));
	path[pathSize++] = node;
	
	while (node->parent != NULL) {
		node = node->parent;
		path[pathSize++] = node;
	}

	struct Node** tmp = realloc(path, pathSize * sizeof(struct Node*));
	if (tmp == NULL) {
		printf("error - get path");
		return;
	}

	maze->path = tmp;
	maze->pathSize = pathSize;
}

// bfs algorithm
void bfs(struct Maze* maze) {
	struct Node* startNode = getNodeBySign(maze, START);
	startNode->isVisited = true;
	struct Node* currentNode = NULL;

	struct Node** queue = malloc(sizeof(struct Node*));
	int queueSize = 0;
	queuePush(startNode, &queue, &queueSize);
	
	while (queueSize > 0) {
		currentNode = queuePop(&queue, &queueSize);
		int neighboursSize;
		struct Node** neighbours = getNeighbours(*maze, currentNode, &neighboursSize);

		for (int i = 0; i < neighboursSize; i++) {
			if (!neighbours[i]->isVisited) {
				neighbours[i]->parent = currentNode;
				neighbours[i]->isVisited = true;
				queuePush(neighbours[i], &queue, &queueSize);
				if (neighbours[i]->type == END) {
					getPath(maze);
				}
			}
		}
	}

	printMaze(maze, true);
	free(queue);
}

// frees memory
void cleanUp(struct Maze* maze) {
	for (int i = 0; i < MAZE_HEIGHT; i++)
	{
		for (int j = 0; j < MAZE_WIDTH; j++)
		{
			free(maze->grid[i][j]);
		}
	}
	
	free(maze->path);
}


int main() {
	struct Maze maze;
	mazeInit(&maze);

	printMaze(&maze, false);
	bfs(&maze);
	cleanUp(&maze);
	return 0;
}