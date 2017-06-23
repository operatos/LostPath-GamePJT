#include "MazeMaker.h"
#include "common.h"

void CreateMaze( )
{
    srand(time(NULL));
    int (*arrMap)[MAPPOS_MAXIMUM_X];
    arrMap = GAMEDATA.arrMapInfo;
    int width = MAPPOS_MAXIMUM_X / 2;
    int height = MAPPOS_MAXIMUM_Y / 2;

    maze_t maze = (maze_t)calloc(width * height, sizeof(cell_t));
    maze_t mp, maze_top;
    char paths [4];
    int visits, directions;

    visits = width * height - 1;
    mp = maze;
    maze_top = mp + (width * height) - 1;

    while (visits) {
        directions = 0;

        if ((mp - width) >= maze && cell_empty (mp - width))
            paths [directions++] = UP;
        if (mp < maze_top && ((mp - maze + 1) % width) && cell_empty (mp + 1))
            paths [directions++] = RIGHT;
        if ((mp + width) <= maze_top && cell_empty (mp + width))
            paths [directions++] = DOWN;
        if (mp > maze && ((mp - maze) % width) && cell_empty (mp - 1))
            paths [directions++] = LEFT;

        if (directions) {
            visits--;
            directions = ((unsigned) rand () % directions);

            switch (paths [directions]) {
                case UP:
                    mp->up = TRUE;
                    (mp -= width)->down = TRUE;
                    break;
                case RIGHT:
                    mp->right = TRUE;
                    (++mp)->left = TRUE;
                    break;
                case DOWN:
                    mp->down = TRUE;
                    (mp += width)->up = TRUE;
                    break;
                case LEFT:
                    mp->left = TRUE;
                    (--mp)->right = TRUE;
                    break;
                default:
                    break;
            }
        } else {
            do {
                if (++mp > maze_top)
                    mp = maze;
            } while (cell_empty (mp));
        }
    }
	
	mp = maze;
	int w, h;
	int idx = 0;
	mp->up = TRUE;
	(mp + (width * height) - 1)->down = TRUE;
	
	for (w = 0; w < MAPPOS_MAXIMUM_X; w++) {
		arrMap[0][idx++] = 1; //벽
		if ((mp + w)->up)
			arrMap[0][idx++] = ((mp + w)->path) ? 1 : 0;
		else
			arrMap[0][idx++] = 1;
	}
	arrMap[0][idx++] = 1;
	
	
	for (h = 0; h < height; h++) {
		idx = 0;
		for (w = 0; w < width; w++) {
			if ((mp + w)->left)
				arrMap[(h+1)*2-1][idx++] = ((mp + w)->path && (mp + w - 1)->path) ? 1 : 0;
			else
				arrMap[(h +1) * 2 - 1][idx++] = 1;
			arrMap[(h +1) * 2 - 1][idx++] = ((mp + w)->path) ? 1 : 0;
		}
		arrMap[(h +1) * 2 - 1][idx++] = 1;

		for (idx = 0, w = 0; w < width; w++) {
			arrMap[(h +1) * 2][idx++] =1;
			if ((mp + w)->down)
				arrMap[(h +1) * 2][idx++] = ((mp + w)->path && (h == height - 1 ||
				(mp + w + width)->path)) ? 1 : 0;
			else
				arrMap[(h + 1) * 2][idx++] = 1;
		}
		arrMap[(h + 1) * 2][idx++] = 1;
		mp += width;
	}

	free(maze);
}
