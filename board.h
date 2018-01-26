#ifndef _BOARD_H
#define _BOARD_H

#include "pos.h"

/* structure of a square condition */
enum square {
    EMPTY,
    BLACK,
    WHITE,
    POSS
};

typedef enum square square;

/* union of board_rep */
union board_rep {
    enum square** cells;
    unsigned int* bits;
};

typedef union board_rep board_rep;

/* two different types of representation of the board */
enum type {
    CELLS, BITS
};

/* structure of a board */
struct board {
    unsigned int nrows, ncols;
    enum type type;
    board_rep u;
};

typedef struct board board;

/* initialize a new board */
board* board_new(unsigned int nrows, unsigned int ncols, enum type type);

/* free the board */
void board_free(board* b);

/* show the board */
void board_show(board* b);

/* get the square condition on a given position */
square board_get(board* b, pos p);

/* set the square on a given position to a given condition */
void board_set(board* b, pos p, square s);

#endif /* _BOARD_H */
