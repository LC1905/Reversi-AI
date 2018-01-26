#include <stdlib.h>
#include <stdio.h>
#include "pos.h"

/* make a new pos */
pos make_pos(unsigned int r, unsigned int c){
    pos p;
    p.r = r;
    p.c = c;
    return p;
}

/* construct a new poslist */
poslist* pos_cons(pos p, poslist* ps){
    poslist* new_ps = (poslist*)malloc(sizeof(poslist));
    new_ps->p = p;    
    new_ps->next = ps;
    return new_ps;
}

/* free a poslist */
void poslist_free(poslist* ps){
    if (ps != NULL){
        poslist_free(ps->next);
        free(ps);
    }
}

/* show a poslist */
void poslist_show(poslist* ps){
    if (ps == NULL){
        printf("NULL\n");
    } else {
        printf("r=%u c=%u\n", ps->p.r, ps->p.c);
        poslist_show(ps->next);
    }
}

unsigned int poslist_count(poslist* ps){
    if (ps == NULL){
        return 0;
    }
    unsigned int count = 1;
    if (ps->next){
        count = poslist_count(ps->next) + 1;
    }
    return count;
}    

poslist* pos_append(pos p, poslist* ps){
    if (ps == NULL){
        ps = pos_cons(p, NULL);
    } else {
        poslist* temp = ps;
        while (temp->next){
            temp = temp->next;
        } 
        temp->next = pos_cons(p, NULL); 
    }
    return ps;
}

int in_poss(poslist* ps, pos p){
    poslist* temp = ps;
    if (!temp){
        return 0;
    }
    while (temp){
        if (p.r == ps->p.r && p.c == ps->p.c){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}