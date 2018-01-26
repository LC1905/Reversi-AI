#include <stdlib.h>
#include <stdio.h>
#include "players.h"
#include "string.h"

/* helper for the main function */
void main_helper(pos (*player)(game*, strategy_config), game* g, strategy_config psc){
    if (!player_has_move(g)){
        g->next = opposite_player(g->next);
    } else {
        pos p = player(g, psc);
        printf("about to apply: p.r = %u, p.c = %u\n", p.r, p.c);
        apply_move(g, p);
    }
        game* temp = copy_game(g);
        poslist* poss = possible_moves(temp);
        poslist* curr = poss;
         while (curr){
            board_set(temp->b, curr->p, POSS);
            curr = curr->next;
        } 
        board_show(temp->b); 
        poslist_free(poss);
        free_game(temp);
}

/* play the game!
   -r nrows default 8 ; -c ncols default 8; 
   -bp black ply default 3; -wp white ply default 3*/
int main(int argc, char* argv[]){
    unsigned int nrows = 8, ncols = 8, i;
    pos (*bp)(game*, strategy_config) = &minimax;
    pos (*wp)(game*, strategy_config) = &human;
    strategy_config bsc, wsc;
    bsc.minimax_config.heuristic = &prefer_edges_and_corners;
    bsc.minimax_config.hc.edge_weight = 2;
    bsc.minimax_config.hc.edge_corner_weight.edge_weight = 2;
    bsc.minimax_config.hc.edge_corner_weight.corner_weight = 5;
    bsc.minimax_config.ply = 5;
    wsc.minimax_config.heuristic = &prefer_edges_and_corners;
    wsc.minimax_config.hc.edge_weight = 2;
    wsc.minimax_config.hc.edge_corner_weight.edge_weight = 4;
    wsc.minimax_config.hc.edge_corner_weight.corner_weight = 9;
    wsc.minimax_config.ply = 3;


    for (i=1; i<argc; i+=2){
        printf("%s\n", argv[i]);
        if (!strcmp(argv[i], "-r")){
            nrows = atoi(argv[i+1]);
        }
        else if (!strcmp(argv[i], "-c")){
            ncols = atoi(argv[i+1]);
        }
        else if (!strcmp(argv[i], "-bp")){
            bsc.minimax_config.ply = atoi(argv[i+1]);
        }
        else if (!strcmp(argv[i], "-wp")){
            wsc.minimax_config.ply = atoi(argv[i+1]);
        }
    }

    game* g = new_game(nrows, ncols, CELLS);
    while (!game_over(g)){
        if (g->next == BLACK_NEXT){ 
            main_helper(bp, g, bsc);
        }
        else if (g->next == WHITE_NEXT){
            main_helper(wp, g, wsc);
        }
    } 
    board_show(g->b);
    show_outcome(g); ;
    free_game(g);
    return 0;
}
