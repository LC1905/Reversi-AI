#ifndef _LOGIC_H
#define _LOGIC_H

#include "board.h"

/* player turn */
enum turn {
    BLACK_NEXT,
    WHITE_NEXT
};

typedef enum turn turn;

/* outcome of the game */
enum outcome {
    BLACK_WIN,
    WHITE_WIN,
    DRAW
};

typedef enum outcome outcome;

/* structure of a game */
struct game {
    board* b;
    turn next;
};

typedef struct game game;

/* initialize a new game */
game* new_game(unsigned int nrows, unsigned int ncols, enum type type);

/* determine if a move is legal */
int outflanks(game* g, pos p);

/* construct a poslist containing all positions that need to be flipped */
poslist* flips(game* g, pos p);

/* apply a move */
void apply_move(game* g, pos p);

/* determine if a game is over */
int game_over(game* g);

/* return the result of the game */
outcome game_outcome(game* g);

/* free the game */
void free_game(game* g);

/* show the outcome of a game */
void show_outcome(game* g);

turn opposite_player(turn p);

int player_has_move(game* g);

turn opposite_player(turn p);
#endif /* _LOGIC_H */
