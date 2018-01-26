#include <stdlib.h>
#include <stdio.h>
#include "logic.h"

/* tests of pos.c file */
void evidence_pos(){
    printf("*** testing pos.c ***\n");
    poslist* ps = pos_cons(make_pos(0, 0), NULL);
    poslist_show(ps);
    printf("----------------\n");
    poslist* ps2 = pos_cons(make_pos(1,2), ps);
    poslist_show(ps2);
    printf("----------------\n");
    poslist_free(ps2);
}

/* tests of board.c cells version */
void evidence_board_cells(){
    printf("*** testing board.c cells version\n");
    board* b = board_new(5, 6, CELLS);
    board_set(b, make_pos(1, 2), BLACK);
    board_set(b, make_pos(0, 4), WHITE);
    board_set(b, make_pos(2, 4), BLACK);
    printf("- showing 1:\n");
    board_show(b);
    board_set(b, make_pos(1, 2), WHITE);
    printf("- showing 2: \n");
    board_show(b);
    board_set(b, make_pos(1, 2), EMPTY);
    printf("- showing 3: \n");
    board_show(b);
    board_free(b);
}

/* tests of board.c bits version */
void evidence_board_bits(){
    printf("*** testing board.c bits version\n");
    board* b = board_new(5, 6, BITS);
    board_set(b, make_pos(1, 2), BLACK);
    board_set(b, make_pos(0, 4), WHITE);
    board_set(b, make_pos(2, 4), BLACK);
    printf("- showing 1:\n");
    board_show(b);
    board_set(b, make_pos(1, 2), WHITE);
    printf("- showing 2:\n");   
    board_show(b);
    board_set(b, make_pos(1, 2), EMPTY);
    printf("- showing 3:\n");
    board_show(b);
    board_free(b);
}

/* tests of initializing a new game */
void evidence_new_game(){
    printf("*** testing new_game\n");
    game* n1 = new_game(8, 8, CELLS);
    board_show(n1->b);
    game* n2 = new_game(5, 5, CELLS);
    board_show(n2->b);
    free_game(n1);
    free_game(n2);
}

/* tests og outflanks */
void evidence_outflanks(){
    printf("*** testing outflanks\n");
    game* n1 = new_game(8, 8, BITS);
    printf(" - expecting 1: %d\n", outflanks(n1, make_pos(3, 2)));
    printf(" - expecting 1: %d\n", outflanks(n1, make_pos(5, 4)));
    printf(" - expecting 0: %d\n", outflanks(n1, make_pos(5, 5)));
    printf(" - expecting 0: %d\n", outflanks(n1, make_pos(2, 5)));
    free_game(n1);
}

/* tests of flips */
void evidence_flips(){
    printf("*** testing flips\n");
    game* n1 = new_game(8, 8, CELLS);
    poslist* ps = flips(n1, make_pos(1,2));
    poslist_show(ps);
    free_game(n1);
}

/* tests of apply_move */
void evidence_apply_move(){
    printf("*** testing apply move\n");
    game* n1 = new_game(8, 8, BITS);
    apply_move(n1, make_pos(4,5));
    board_show(n1->b);
    free_game(n1);
}

/* tests of game_over */
void evidence_game_over(){
    printf("*** testing game over\n");
    game* n1 = new_game(8, 8, CELLS);
    apply_move(n1, make_pos(4,5));
    board_show(n1->b);
    printf("- expecting 0: %d\n", game_over(n1));
    game* n2 = new_game(3, 3, BITS);
    apply_move(n2, make_pos(1, 2));
    apply_move(n2, make_pos(2, 2));
    printf("- expecting 0: %d\n", game_over(n2));
    apply_move(n2, make_pos(2, 1));
    apply_move(n2, make_pos(2, 0));
    printf("- expecting 0: %d\n", game_over(n2));
    board_set(n2->b, make_pos(0, 2), WHITE);
    board_show(n2->b);
    printf("- expecting 1: %d\n", game_over(n2));
    free_game(n1);
    free_game(n2);
}

/* tests of the whole game */
void evidence_game(){
    printf("*** testing the whole game\n");
    game* n1 = new_game(8, 8, BITS);
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(4, 5));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(5, 3));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(4, 2));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(5, 5));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(6, 3));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(4, 1));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(4, 0));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(7, 3));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(2, 2));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(2, 3));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(6, 5));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(2, 1));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(5, 2));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(3, 0));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(2, 0));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(4, 6));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(4, 7));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(3, 5));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(2, 4));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(2, 5));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(5, 1));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(5, 4));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(5, 6));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(5, 7));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(1, 5));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(3, 6));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(2, 6));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(3, 7));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(7, 2));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(7, 4));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(7, 5));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    apply_move(n1, make_pos(6, 2));
    board_show(n1->b);
    printf("game over = %d\n", game_over(n1));
    free_game(n1);
}

int main(int argc, char* argv[]){
    evidence_pos();
    evidence_board_cells();
    evidence_board_bits();
    evidence_new_game();
    evidence_outflanks();
    evidence_flips();
    evidence_apply_move();
    evidence_game_over();
    evidence_game();
    return 0;
}
