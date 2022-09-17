/**
 * EC60011 - Data Structures and Object Representation
 * 
 * Assignment - 2
 * 
 * Author: Utkarsh Patel
 */

#include "game.h"


/**
 * @brief Pull best metric from children to increase difficulty level
 */
void pull_metric(node_t *node) {
    if (node == NULL) return;
    int max_metric = node->metric;
    node_t *iter = node->child;
    while (iter != NULL) {
        if (max_metric < (-iter->metric)) {
            max_metric = (-iter->metric);
        }

        iter = iter->next;
    }
    node->metric = max_metric;

    /* recursively call on childrens */
    iter = node->child;
    while (iter != NULL) {
        pull_metric(iter);
        iter = iter->next;
    }
}


/**
 * @brief Simulate computer movements
 * 
 * @param node: Current state of the game
 * @return: Next state after computer movement (NULL, if failed!)
 */
node_t *play_comp(node_t *node) {
    print_node(node);

    if (node->child == NULL) {
        if (node->metric == -INF) {
            printf("[!] Unexpected error!\n");
        } else if  (node->metric == INF) {
            printf("[+] User wins the game!\n");
        } else {
            printf("[+] Game ended in a draw.\n");
        }
        return NULL;
    }

    int max_metric = -INF - 1;
    node_t *save = NULL, *iter = node->child;

    while (iter != NULL) {
        if (iter->metric > max_metric) {
            save = iter;
            max_metric = iter->metric;
        }

        iter = iter->next;
    }

    return save;
}


/**
 * @brief Simulate user movements
 * 
 * @param node: Current state of the game
 * @return: Next state after user movement (NULL, if failed!)
 */
node_t *play_user(node_t *node) {
    print_node(node);

    if (node->child == NULL) {
        if (node->metric == -INF) {
            printf("[!] Unexpected error!\n");
        } else if  (node->metric == INF) {
            printf("[+] Computer wins the game!\n");
        } else {
            printf("[+] Game ended in a draw.\n");
        }
        return NULL;
    }

    int x, y;

    printf("[*] Enter position to put an 'X': ");
    scanf("%d %d", &x, &y);

    /* Capture invalid positions */
    if (x < 0 || x >= 3 || y < 0 || y >= 3 || node->board[x][y] != 'N') {
        printf("[!] Invalid position!\n");
        return NULL;
    }

    node = node->child;
    while (node != NULL) {
        if (node->board[x][y] == 'X') return node;
        node = node->next;
    }

    printf("[!] Unexpected error!\n");
    return node;
}


/**
 * @brief Prints board for given state
 */
void print_node(node_t *node) {
    printf("+---+---+---+\n");

    for (int i = 0; i < 3; i++) {
        printf("|");
        for (int j = 0; j < 3; j++) {
            node->board[i][j] == 'N' ? printf("   |") : 
                printf(" %c |", node->board[i][j]);
        }
        printf("\n+---+---+---+\n");
    }
    printf("\n");
}


/**
 * @brief Create game tree for Tic-Tac-Toe
 * 
 * @details Recursively calls `create_node` subroutine and appropriately sets
 *          `next` and `child` pointers for the nodes in the tree
 * 
 * @param board: Board state
 * @param turn: Player's turn
 * 
 * @return: Game tree
 */
node_t *create_tree(char board[3][3], int turn) {
    node_t *node = create_node(board, turn);
    if (abs(node->metric) == INF) return node; /* return if leaf node */

    node_t *cur, *nxt = NULL;

    turn ^= 1; /* switch player */

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 'N') {
                board[i][j] = turn ? 'X' : 'O';
                cur = create_tree(board, turn);

                /* set sibling relationship */
                cur->next = nxt;
                nxt = cur;

                /* undo board modification */
                board[i][j] = 'N';
            }
        }
    }

    /* set parent-child relationship */
    node->child = nxt;
    return node;
}


/**
 * @brief Create a node for given board state
 * 
 * @param board: board state
 * @param turn: turn
 * 
 * @return: Node for given state
 */
node_t *create_node(char board[3][3], int turn) {
    node_t *node = (node_t *) malloc(sizeof(node_t));

    memcpy(node->board, board, sizeof(node->board));
    node->turn   = turn;
    node->child  = NULL;
    node->next   = NULL;

    /* evaluate this state */
    node->metric = evaluate_node(node);

    return node;
}


/**
 * @brief Recursive deallocation of resources for given node (and its subtrees,
 * and siblings)
 * @param node: Game tree node
 */
void free_node(node_t *node) {
    /* null check */
    if (node == NULL) return;

    /* try freeing its subtree and subtrees of its sibling */
    if (node->child != NULL) free_node(node->child);
    if (node->next  != NULL) free_node(node->next);

    free(node);
}

/**
 * @brief Evaluate given tic-tac-toe board state
 * 
 * @details Evaluation is performed as follows:
 *              - if current player has three symbols in a row/column/diagonal, 
 *                return INF
 *              - if opponent has three symbols in a row/column/diagonal,
 *                return -INF
 *              - otherwise, count the number of possible rows/columns/diagonals
 *                that could be filled to win for each player and return difference.
 * 
 * @param node: Board state
 * 
 * @return: Evaluation metric
 */
int evaluate_node(node_t *node) {
    int player_x_score = 0; 
    int player_o_score = 0;

    int x_count, o_count;

    /* evaluating rows */
    for (int i = 0; i < 3; i++) {
        x_count = 0;
        o_count = 0;

        for (int j = 0; j < 3; j++) {
            x_count += (node->board[i][j] == 'X');
            o_count += (node->board[i][j] == 'O');
        }

        /* check if the game is a non-draw leaf node */
        if (x_count == 3) {
            return node->turn ? INF : -INF;
        }
        if (o_count == 3) {
            return node->turn ? -INF : INF;
        }

        /* increment player score if given row doesn't contain other symbol */
        player_x_score += (o_count == 0);
        player_o_score += (x_count == 0);
    }

    /* evaluating columns */
    for (int j = 0; j < 3; j++) {
        x_count = 0;
        o_count = 0;

        for (int i = 0; i < 3; i++) {
            x_count += (node->board[i][j] == 'X');
            o_count += (node->board[i][j] == 'O');
        }

        /* check if the game is a non-draw leaf node */
        if (x_count == 3) {
            return node->turn ? INF : -INF;
        }
        if (o_count == 3) {
            return node->turn ? -INF : INF;
        }

        /* increment player score if given row doesn't contain other symbol */
        player_x_score += (o_count == 0);
        player_o_score += (x_count == 0);
    }

    /* evaluating main diagonal */
    {
        x_count = 0;
        o_count = 0;

        for (int i = 0; i < 3; i++) {
            x_count += (node->board[i][i] == 'X');
            o_count += (node->board[i][i] == 'O');
        }

        /* check if the game is a non-draw leaf node */
        if (x_count == 3) {
            return node->turn ? INF : -INF;
        }
        if (o_count == 3) {
            return node->turn ? -INF : INF;
        }

        /* increment player score if given row doesn't contain other symbol */
        player_x_score += (o_count == 0);
        player_o_score += (x_count == 0);
    }

    /* evaluating off-diagonal */
    {
        x_count = 0;
        o_count = 0;

        for (int i = 0; i < 3; i++) {
            x_count += (node->board[2 - i][i] == 'X');
            o_count += (node->board[2 - i][i] == 'O');
        }

        /* check if the game is a non-draw leaf node */
        if (x_count == 3) {
            return node->turn ? INF : -INF;
        }
        if (o_count == 3) {
            return node->turn ? -INF : INF;
        }

        /* increment player score if given row doesn't contain other symbol */
        player_x_score += (o_count == 0);
        player_o_score += (x_count == 0);
    }

    /* current state is not non-draw leaf node in the game tree */
    int delta = player_x_score - player_o_score;

    return node->turn ? delta : -delta;
}