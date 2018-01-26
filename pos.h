#ifndef _POS_H
#define _POS_H

typedef struct pos pos;

/* structure of a position */
struct pos {
    unsigned int r, c;
};

typedef struct poslist poslist;

/* structure of a poslist */
struct poslist {
    pos p;
    poslist* next;
};

/* make a new pos */
pos make_pos(unsigned int r, unsigned int c);

/* construct a new poslist */
poslist* pos_cons(pos p, poslist* ps);

/* free a poslist */
void poslist_free(poslist* ps);

/* show a poslist */
void poslist_show(poslist* ps);

unsigned int poslist_count(poslist* ps);

poslist* pos_append(pos p, poslist* ps);

int in_poss(poslist* ps, pos p);

#endif /* _POS_H */
