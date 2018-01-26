/* CMSC 15200, The University of Chicago, Spring 2017
 * Project 1 Reference Solution
 * Matthew Wachs */

#include <stdlib.h>
#include <stdio.h>
#include "logic.h"


/* return a poslist consisting all possible moves of a given game */
poslist* possible_moves(game* g){
    poslist* moves = (poslist*)malloc(sizeof(poslist));
    moves = NULL;
    pos p;
    for (p.r = 0; p.r < g->b->nrows; p.r++){
        for (p.c = 0; p.c < g->b->ncols; p.c++){
            if (outflanks(g, p)){
                moves = pos_append(p, moves);
            }
        }
    }
    return moves;
}

enum direction {
    NW, N, NE, E, SE, S, SW, W
};

typedef enum direction direction;


/* create a new game with the specified number of rows and columns
 * and desired board representation */
game* new_game(unsigned int nrows, unsigned int ncols, enum type type) {
    if (nrows < 2 || ncols < 2) {
        fprintf(stderr, "game_new: cannot have game this small\n");
        exit(1);
    }
    game* g = (game*)malloc(sizeof(game));
    g->b = board_new(nrows, ncols, type);
    g->next = BLACK_NEXT;
    
    board_set(g->b, make_pos(nrows/2 - 1, ncols/2    ), BLACK);
    board_set(g->b, make_pos(nrows/2    , ncols/2 - 1), BLACK);
    board_set(g->b, make_pos(nrows/2 - 1, ncols/2 - 1), WHITE);
    board_set(g->b, make_pos(nrows/2    , ncols/2    ), WHITE);
    
    return g;
}

/* find the opposite player */
turn opposite_player(turn p) {
    switch (p) {
        case BLACK_NEXT:
            return WHITE_NEXT;
        case WHITE_NEXT:
            return BLACK_NEXT;
        default:
            fprintf(stderr, "opposite_player: invalid player passed in\n");
            exit(1);
    }
}

/* given a player encoded as a turn, give the square encoding for that player */
square square_for_player(turn p) {
    switch (p) {
        case BLACK_NEXT:
            return BLACK;
        case WHITE_NEXT:
            return WHITE;
        default:
            fprintf(stderr, "square_for_player: invalid player passed in\n");
            exit(1);
    }
}

/* update a pos to have moved one spot in the given direction
 * return true if this is possible
 * return false if doing so would leave the board */
int move_one(board* b, pos* p, direction d) {
    switch (d) {
        case NW:
            return move_one(b, p, N)?move_one(b, p, W):0;
        case N:
            if (p->r == 0) {
                return 0;
            }
            p->r--;
            return 1;
        case NE:
            return move_one(b, p, N)?move_one(b, p, E):0;
        case E:
            if (p->c >= b->ncols - 1) {
                return 0;
            }
            p->c++;
            return 1;
        case SE:
            return move_one(b, p, S)?move_one(b, p, E):0;
        case S:
            if (p->r >= b->nrows - 1) {
                return 0;
            }
            p->r++;
            return 1;
        case SW:
            return move_one(b, p, S)?move_one(b, p, W):0;
        case W:
            if (p->c == 0) {
                return 0;
            }
            p->c--;
            return 1;
        default:
            fprintf(stderr, "move_one: invalid direction\n");
            exit(1);
    }
}

/* free a poslist from beg(inning) to mid(dle), leaving the rest intact */
void free_until(poslist* beg, poslist* mid) {
    while (beg != mid) {
        poslist* next = beg->next;
        free(beg);
        beg = next;
    }
}

/* explore the board, starting at the given position, in the given direction,
 * looking for a chain of opposite-colored pieces, terminated by one of your
 * own color
 * return the number of opposite-colored pieces in a row, but 0 if not
 * outflanked
 * if ps (an out-parameter) is not NULL, prepend the list of opposite-colored
 * pieces to it, if they are properly outflanked */
unsigned int explore(game* g, pos p, direction d, poslist** ps) {
    unsigned spots = 0;
    poslist* newps = ps?*ps:NULL;
    
    while (move_one(g->b, &p, d)) {
        if (board_get(g->b, p) == square_for_player(g->next)) {
            if (ps) {
                *ps = newps;
            }
            return spots;
        } else if (board_get(g->b, p) == EMPTY) {
            break;
        }
        if (ps) {
            newps = pos_cons(p, newps);
        }
        spots++;
    }

    /* we speculatively prepended a chain of opponent pieces as we explored,
     * but it turns out they are not outflanked
     * undo this */
    if (ps) {
        free_until(newps, *ps);
    }
    
    return 0;
}

/* determine if placing the current player at the specified position is a legal
 * (outflanking) move */
int outflanks(game* g, pos p) {
    if (board_get(g->b, p) != EMPTY) {
        return 0;
    }
    
    return explore(g, p, NW, NULL) ||
           explore(g, p, N , NULL) ||
           explore(g, p, NE, NULL) ||
           explore(g, p,  E, NULL) ||
           explore(g, p, SE, NULL) ||
           explore(g, p, S , NULL) ||
           explore(g, p, SW, NULL) ||
           explore(g, p,  W, NULL);
}

/* given a game and a position at which a move is to be made, return a list of
 * the resulting flips */
poslist* flips(game* g, pos p) {
    poslist* ps = NULL;
    
    explore(g, p, NW, &ps);
    explore(g, p, N , &ps);
    explore(g, p, NE, &ps);
    explore(g, p,  E, &ps);
    explore(g, p, SE, &ps);
    explore(g, p, S , &ps);
    explore(g, p, SW, &ps);
    explore(g, p,  W, &ps);
    
    return ps;
}

/* make a move for the current player at the specified position and flip whose
 * turn it is */
void apply_move(game* g, pos p) {
    if (!outflanks(g, p)) {
        fprintf(stderr, "apply_move: attempt to apply illegal move\n");
        exit(1);
    }
    poslist* fs = flips(g, p);
    board_set(g->b, p, square_for_player(g->next));
    
    if (!fs) {
        fprintf(stderr, "apply_move: empty flips list for outflanking move\n");
        exit(1);
    }
    
    while (fs) {
        poslist* next = fs->next;
        
        if (board_get(g->b, fs->p) !=
            square_for_player(opposite_player(g->next))) {
            fprintf(stderr, "apply_move: flips contains non-opponent square\n");
            exit(1);
        }
        board_set(g->b, fs->p, square_for_player(g->next));
        
        free(fs);
        fs = next;
    }
    g->next = opposite_player(g->next);
}

/* determine if the player whose turn it is has any legal move */
int player_has_move(game* g) {
    pos p;

    for (p.r = 0; p.r < g->b->nrows; p.r++) {
        for (p.c = 0; p.c < g->b->ncols; p.c++) {
            square s = board_get(g->b, p);
            
            if (s == EMPTY) {
                if (outflanks(g, p)) {
                    return 1;
                }
            }
        }
    }
    
    return 0;
}

/* determine if the game is over */
int game_over(game* g) {
    /* first, see if current player has any legal moves */
    if (player_has_move(g)) {
        return 0;
    }
    
    /* but current player might have to pass */
    game g_pass = {g->b, opposite_player(g->next)};
    return !player_has_move(&g_pass);
}

/* for a completed game, determine the winner or a draw */
outcome game_outcome(game* g) {
    unsigned int blacks = 0, whites = 0;
    pos p;
    
    for (p.r = 0; p.r < g->b->nrows; p.r++) {
        for (p.c = 0; p.c < g->b->ncols; p.c++) {
            square s = board_get(g->b, p);
            switch (s) {
                case BLACK:
                    blacks++;
                    break;
                case WHITE:
                    whites++;
                    break;
                case EMPTY:
                    break;
                default:
                    fprintf(stderr, "game_outcome: invalid square\n");
                    exit(1);
            }
        }
    }
    
    if (blacks > whites) {
        return BLACK_WIN;
    } else if (whites > blacks) {
        return WHITE_WIN;
    } else {
        return DRAW;
    }
}


void show_outcome(game* g){
    outcome ot = game_outcome(g);
    if (ot == BLACK_WIN){
        printf("Black Win!\n");
    }
    if (ot == WHITE_WIN){
        printf("White Win!\n");
    }
    if (ot == DRAW){
        printf("Draw!\n");
    }
}


/* free the game */
void free_game(game* g){
    board_free(g->b);
    free(g);
}