#include <stdlib.h>
#include <stdio.h>
#include "players.h"

/* copy a board */
board* copy_board(board* b){
    board* copy = board_new(b->nrows, b->ncols, b->type);
    pos p;
    for (p.r=0; p.r < copy->nrows; p.r++){
        for (p.c=0; p.c < copy->ncols; p.c++){
            board_set(copy, p, board_get(b, p));
        }
    }
    return copy;
}

/* copy a game */
 game* copy_game(game* g){
    game* copy = (game*)malloc(sizeof(game));
    copy->b = copy_board(g->b);
    copy->next = g->next;
    return copy;
} 

/* strategy: find the first available move
   from left to right; from top to the buttom 
   TESTED*/
pos first_move(game* g, strategy_config unused){
    unsigned int i, j;
    for (i=0; i<g->b->nrows; i++){
         for (j=0; j<g->b->ncols; j++){
            pos p = make_pos(i, j);
            if (board_get(g->b, p) == EMPTY){
                if (outflanks(g, p)){
                    return p;              
                }
            }
        }
    }
    fprintf(stderr, "out of legal moves\n");
    exit(1);
}

/* convert a char to int on the board */
int char_to_int(char c){
    if (c >= '0' && c <= '9'){
        return 0 + c - '0';
    }
    if (c >= 'A' && c <= 'Z'){
        return 10 + c - 'A';
    }
    if (c >= 'a' && c <= 'z'){
        return 36 + c - 'a';
    }
    return -1;
}

/* check if a char is valid 
   the char is not valid if it is out of the range of a given board */
int valid_char(char c, int range){
    if (char_to_int(c) < range && char_to_int >= 0){
        return 1;
    }
    return 0;
}

/* strategy: human player */
pos human(game* g, strategy_config unused){
    switch(g->next){
        case BLACK_NEXT:
            printf("Black:");
            break;
        case WHITE_NEXT:
            printf("White:");
            break;
    }
    char r, c;
    scanf(" %c %c", &r, &c);
    if (!valid_char(r, g->b->nrows) || !valid_char(c, g->b->ncols)){
        return human(g, unused);
    }
    unsigned int ir = char_to_int(r), ic = char_to_int(c);
    pos p = make_pos(ir, ic);
    if (!outflanks(g, p)){
        return human(g, unused);
    }
    return p;
}

/* append a position if it can be outflanked */
poslist* append(poslist* track, pos p, game* g, strategy_config unused){
    if (outflanks(g, p)){
        track = pos_append(p, track);
    }
    return track;
} 

/* find the move causing maximum flips within a category */
pos max_flips(game* g, poslist* track){
    pos res = make_pos(g->b->nrows, g->b->ncols);
    if (track){
        poslist* temp = track;
        unsigned int count = 0;
        while (temp){
            unsigned int curr = poslist_count(flips(g, temp->p));
            if (curr > count){
                count = curr;
                res = temp->p;
            }
            temp = temp->next;
        }
        poslist_free(track);
    }
    return res;
}

/* find all possible moves in the corner 
   tracked by a poslist*/
pos corner_tactics(game* g, strategy_config unused){
    poslist* track = NULL;
    track = append(track, make_pos(0, 0), g, unused);
    track = append(track, make_pos(0, g->b->ncols-1), g, unused);
    track = append(track, make_pos(g->b->nrows-1, 0), g, unused);
    track = append(track, make_pos(g->b->nrows-1, g->b->ncols-1), g, unused);
    return max_flips(g, track);
}

/* find all possible moves on the edge 
   tracked by a poslist */
pos edge_tactics(game* g, strategy_config unused){
    poslist* track = NULL;
    unsigned int i,j;
    for (j=1; j<g->b->ncols-1; j++){
        track = append(track, make_pos(0, j), g, unused);
    }
    for (i=1; i<g->b->ncols-1; i++){
        track = append(track, make_pos(i, 0), g, unused);
        track = append(track, make_pos(i, g->b->ncols-1), g, unused);
    }
    for (j=1; j<g->b->ncols-1; j++){
        track = append(track, make_pos(g->b->nrows-1, j), g, unused);
    }
    return max_flips(g, track);
}

/* find all possible interior moves 
   tracked by a poslist*/
pos interior_tactics(game* g, strategy_config unused){
    poslist* track = NULL;
    unsigned int i, j;
    for (i=1; i<g->b->nrows-1; i++){
        for (j=1; j<g->b->ncols-1; j++){
            track = append(track, make_pos(i, j), g, unused);
        }
    }
    return max_flips(g, track);
}

/* strategy: corner > edge > interior > number of flips > first_move */
pos immediate_tactics(game* g, strategy_config unused){  
    pos res = corner_tactics(g, unused);
    if (res.r != g->b->nrows){
        return res;
    }
    res = edge_tactics(g, unused);
    if (res.r != g->b->nrows){ 
        return res;
    }
    res = interior_tactics(g, unused);
    if (res.r != g->b->nrows){
        return res;
    }
    fprintf(stderr, "out of legal moves\n");
    exit(1);
}

/* determine if a position is on the cornor of a given game */
int is_corner(game* g, pos p){
    if (p.r == 0 && p.c == 0){
        return 1;
    }
    if (p.r == 0 && p.c == g->b->ncols-1){
        return 1;
    }
    if (p.r == g->b->nrows-1 && p.c == 0){
        return 1;
    }
    if (p.r == g->b->nrows-1 && p.c == g->b->ncols-1){
        return 1;
    }
    return 0;
}

/* determine if a position is on the non-corner edge of a given game */
int is_edge(game* g, pos p){
    if (is_corner(g, p)){
        return 0;
    } 
    if (p.r == 0 || p.r == g->b->nrows-1 || p.c == 0 || p.c == g->b->ncols-1){
        return 1;
    }
    return 0;
}

/* heuristic function:
   return 1 if the game is over and the black player wins
   return -1 if the game is over and the white player wins
   return 0 otherwise */
int did_i_win(game* g, heuristic_config unused){
    if (game_over(g)){
        if (game_outcome(g) == BLACK_WIN){
            return 1;
        }
        else if (game_outcome(g) == WHITE_WIN){
            return -1;
        }
    }
    return 0;
}

/* helper for the following three heuristic functions
   find the current score given a game, edge weight, and corner weight */
int general_counting(game* g, unsigned int edge, unsigned int corner){
    unsigned int blacks = 0, whites = 0;
    pos p;
    for (p.r = 0; p.r < g->b->nrows; p.r++){
        for (p.c = 0; p.c < g->b->ncols; p.c++){
            square s = board_get(g->b, p);
            if (s == BLACK){
                if (is_corner(g, p)){
                    blacks += corner;
                }
                else if (is_edge(g, p)){
                    blacks += edge;
                }
                else {
                    blacks += 1;
                }
        }
            if (s == WHITE){
                if (is_corner(g, p)){
                    whites += corner;
                }
                else if (is_edge(g, p)){
                    whites += edge;
                }
                else {
                    whites += 1;
                }
            }
        }
    }
    return blacks - whites;
}

/* heuristic function:
   score := # blacks - # whites */
int piece_counting(game* g, heuristic_config unused){
    return general_counting(g, 1, 1);
}

/* heuristic function:
   count edges with a different weight (corners included) */
int prefer_edges(game* g, heuristic_config hc){
    return general_counting(g, hc.edge_weight, hc.edge_weight);
}

/* heuristic function:
   count edges and cornors with different weights, respectively */
  int prefer_edges_and_corners(game* g, heuristic_config hc){
    unsigned int edge = hc.edge_corner_weight.edge_weight;
    unsigned int corner = hc.edge_corner_weight.corner_weight;
    return general_counting(g, edge, corner);
} 

/* return a poslist consisting all possible moves of a given game */
/* poslist* possible_moves(game* g){
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
} */

/* helper for minimax_eval and minimax */
  int minimax_eval_helper(game* g, minimax_config mc, int ply, pos* p){
    int score;
    if (mc.ply == 0 || ply > mc.ply){
        fprintf(stderr, "ply cannot be 0 and ply cannot be larger than mc.ply\n");
        exit(1);
    }
    if (game_over(g) || ply == mc.ply){
        score = mc.heuristic(g, mc.hc);
        return score;
    }
    poslist* moves = possible_moves(g);
    ply++;
    if (moves == NULL){
        game* copy = copy_game(g);
        copy->next = opposite_player(copy->next);
        score = minimax_eval_helper(copy, mc, ply, p);
        free_game(copy);
        return score;
    }
    poslist* curr = moves;
    if (g->next == BLACK_NEXT){
        score = -((((1 << 30) - 1) << 1) | 1);
        while (curr){
            game* copy = copy_game(g);
            apply_move(copy, curr->p);
            int res = minimax_eval_helper(copy, mc, ply, p);
            if (res > score){
                score = res;
                if (ply == 1){
                    *p = curr->p;
                }
            } 
            free_game(copy);
            curr = curr->next;
        }
    }
    else {
        score = ((((1 << 30) - 1) << 1) | 1);
        while (curr){
            game* copy = copy_game(g);
            apply_move(copy, curr->p);
            int res = minimax_eval_helper(copy, mc, ply, p);
            if (res < score){
                score = res;
                if (ply == 1){
                    *p = curr->p;
                }
            } 
            free_game(copy);
            curr = curr->next;   
        }
    }
    return score;
} 

/* find the best score using minimax algorithm */
  int minimax_eval(game* g, minimax_config mc){
    pos p;
    return minimax_eval_helper(g, mc, 0, &p);
} 

/* find the optimal next move using minimax algorithm */
  pos minimax(game* g, strategy_config sc){
    pos p;
    minimax_eval_helper(g, sc.minimax_config, 0, &p);
    return p;
} 

