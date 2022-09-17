/**
 * EC60011 - Data Structures and Object Representation
 * 
 * Assignment - 2
 * 
 * Author: Utkarsh Patel
 */

#ifndef _GAME_H
#define _GAME_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INF 100


/**
 * Structure to represent tic-tac-toe state as a node in the game tree
 */
typedef struct _node_t {
    char board[3][3];        /* tic-tac-toe board for current state */
    int turn;                /* turn = 1 for `X` and turn = 0 for `O` */
    int metric;              /* evaluation metric for current state */
    struct _node_t *child;   /* pointer to leader of its children */
    struct _node_t *next;    /* pointer to its next sibling */
} node_t;


node_t   *create_node      (char[3][3], int);
node_t   *create_tree      (char[3][3], int);
void     print_node        (node_t *);
void     free_node         (node_t *);

int      evaluate_node     (node_t *);
void     pull_metric       (node_t *);

node_t   *play_user        (node_t *);
node_t   *play_comp        (node_t *);


#endif /* _GAME_H */