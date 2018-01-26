#ifndef __PLAYERS_H
#define __PLAYERS_H

#include "logic.h"

/* structure of edge_corner_weight */
struct edge_corner_weight {
    unsigned int edge_weight;
    unsigned int corner_weight;
};

/* union of heuristic_config */
union heuristic_config {
    unsigned int edge_weight;
    struct edge_corner_weight edge_corner_weight;
};

typedef union heuristic_config heuristic_config;

/* structure of minimax_config */
struct minimax_config {
    int (*heuristic)(game*, heuristic_config);
    heuristic_config hc;
    unsigned int ply;
};

typedef struct minimax_config minimax_config;

/* union of strategy_config */
union strategy_config {
    minimax_config minimax_config;
};

typedef union strategy_config strategy_config;

/* strategy: apply the first available move */
pos first_move(game* g, strategy_config unused);

/* human player */
pos human(game* g, strategy_config unused);

/* following the tactics
   corner > edge > interior > flips number > first_move */
pos immediate_tactics(game* g, strategy_config unused);

/* heuristic: black win = 1; white win = -1; other = 0 */
int did_i_win(game* g, heuristic_config unused);

/* count the difference between #black and #white */
int piece_counting(game* g, heuristic_config unused);

/* count the difference with a different edge weight */
int prefer_edges(game* g, heuristic_config hc);

/* count the difference with different edge weight and corner weight */
int prefer_edges_and_corners(game* g, heuristic_config hc);

/* determine if a point lies at the corner */
int is_corner(game* g, pos p);

/* find the poslist with all possible moves */
poslist* possible_moves(game* g);

/* find the optimal next move */
pos minimax(game* g, strategy_config sc);

/* find the best score using minimax algorithm */
int minimax_eval(game* g, minimax_config mc);

game* copy_game(game* g);
#endif
