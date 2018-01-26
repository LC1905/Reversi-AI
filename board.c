#include <stdlib.h>
#include <stdio.h>
#include "board.h"

/* initialize a new board */
board* board_new(unsigned int nrows, unsigned int ncols, enum type type){
    board* b = (board*)malloc(sizeof(board));
    b->nrows = nrows;
    b->ncols = ncols;
    b->type = type;
    if (b->type == CELLS){
        b->u.cells = (square**)malloc(sizeof(square*)*nrows);
        unsigned int i, j;
        for (i=0; i<nrows;i++){
            b->u.cells[i] = (square*)malloc(sizeof(square)*ncols);
            for (j=0; j<ncols; j++){
                b->u.cells[i][j] = EMPTY;
            }
        }
    }
    if (b->type == BITS){
        unsigned int count = ((nrows * ncols) / 16) + 1;
        b->u.bits = (unsigned int*)malloc(sizeof(unsigned int) * count);
        unsigned int i;
        for (i=0; i<count; i++){
            b->u.bits[i] = 0;
        }    
    }
    return b;
}

/* given a position, find the corresponding bits of the integers*/
void find_position(board* b, pos p, unsigned int* lc, unsigned int* shift){
        unsigned int position = b->ncols * p.r + p.c;
        *lc = position / 16;
        *shift = (position % 16) * 2;
}

/* set the square on a given position to a given condition */
void board_set(board* b, pos p, square s){
    if (p.r >= b->nrows || p.c >= b->ncols){
        fprintf(stderr, "position exceeds the board!\n");
        exit(1);
    }
    if (b->type == CELLS){
        b->u.cells[p.r][p.c] = s;
    } 
    if (b->type == BITS){
        unsigned int lc, shift, value;
        find_position(b, p, &lc, &shift);
        switch(s){
            case EMPTY:
            value = 0;
            break;
            case BLACK:
            value = 1;
            break;
            case WHITE:
            value = 2;
            break;
            default:
            break;
        }
        unsigned int temp = ((((1 << 31) - 1) << 1) | 1) ^ (3 << shift);
        b->u.bits[lc] &= temp;
        b->u.bits[lc] |= (value << shift);
    }
}

/* get the square condition on a given position */
square board_get(board* b, pos p){
    //printf("calling board_get\n");
    if (p.r >= b->nrows || p.c >= b->ncols){
        fprintf(stderr, "position exceeds the board!\n");
        exit(1);
    }
    if (b->type == CELLS){
        return b->u.cells[p.r][p.c];
    }
    if (b->type == BITS){
        unsigned int lc, shift, value;
        find_position(b, p, &lc, &shift);
        value = (b->u.bits[lc] >> shift) & 3;
        switch(value){
            case 0:
            return EMPTY;
            case 1:
            return BLACK;
            case 2:
            return WHITE;
            default:
            break;
        }
    }
}

/* helper for print a board */
void print_board_helper(unsigned int i){
    if (i < 10){
            printf("%u ", i);
    }
    if (i >= 10 && i < 36){
        printf("%c ", 'A'+i-10);
    }
    if (i >=36 && i < 62){
        printf("%c ", 'a'+i-36);
    }
    if (i >= 62){
        printf("? ");
    }
} 

/* show the board */
void board_show(board* b){
    unsigned int i, j;
    printf("  ");
    for (i=0; i<b->ncols; i++){
        print_board_helper(i);
    }
    printf("\n"); 
    for (i=0; i<b->nrows; i++){
        print_board_helper(i);
        for (j=0; j<b->ncols; j++){
            pos p = make_pos(i, j);
            switch(board_get(b, p)){
                case EMPTY:
                printf("  ");
                break;
                case BLACK:
                printf("X ");
                break;
                case WHITE:
                printf("O ");
                break;
                case POSS:
                printf("* ");
                break;
             }
        }
        printf("\n");
    }
}

/* free the board */
void board_free(board* b){
    if (b == NULL){
        fprintf(stderr, "I don't wanna work with NULL Board!\n");
        exit(1);
    }
    if (b->type == CELLS){
        unsigned int i;
        for (i=0; i<b->nrows; i++){
            free(b->u.cells[i]);
        }
        free(b->u.cells);
        free(b);
    }
    if (b->type == BITS){
        free(b->u.bits);
        free(b);
    }
}


           
