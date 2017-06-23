#ifndef __MAZEMAKER_H__
#define __MAZEMAKER_H__

#define cell_empty(a) (!(a)->up && !(a)->right && !(a)->down && !(a)->left)
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

typedef struct {
	unsigned int up : 1;
	unsigned int right : 1;
	unsigned int down : 1;
	unsigned int left : 1;
	unsigned int path : 1;
	unsigned int visited : 1;
} cell_t;
typedef cell_t* maze_t;
void CreateMaze();

#endif
