#include<stdio.h>
#define VISITED 1
#define NOT_VISITED 0
#define MAZE_HEIGHT 5
#define MAZE_WIDTH 5

struct Node;

char mazePattern[MAZE_HEIGHT][MAZE_WIDTH] = {
	{'.', '.', '.', '.', '.'},
	{'.', '.', '.', '.', '.'},
	{'.', '.', '.', '.', '.'},
	{'.', '.', '.', '.', '.'},
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
	struct Node*** maze = malloc(height * sizeof(struct Node*));
	for (int i = 0; i < height; i++)
	{
		maze[i] = malloc(width * sizeof(struct Node*));
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			struct Node node = { j, i, NOT_VISITED, mazePattern[i][j], NULL };
			maze[i][j] = &node;
		}
	}

	return maze;
}


int main() {
	struct Maze maze = { mazeInit(MAZE_HEIGHT, MAZE_WIDTH), MAZE_HEIGHT, MAZE_WIDTH };
	printMaze(maze);
	return 0;
}