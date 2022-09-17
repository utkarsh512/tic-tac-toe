/**
 * EC60011 - Data Structures and Object Representation
 * 
 * Assignment - 2
 * 
 * Author: Utkarsh Patel
 */

#include "game.h"

int main() {
    char board[3][3];
    memset(board, 'N', sizeof(board));
    
    printf("[*] Constructing game tree..\n");
    node_t *root = create_tree(board, 0);
    printf("[+] Constructed game tree.\n\n");

    printf("[*] Enter difficult level: ");
    int diff_level;
    scanf("%d", &diff_level);
    
    for (int i = 0; i < diff_level; i++) {
        pull_metric(root);
    }
    printf("[+] Difficulty level applied.\n\n");

    node_t *node = root, *response;

    while (1) {
        response = play_user(node);
        if (response == NULL) {
            break;
        }
        node = response;

        response = play_comp(node);
        if (response == NULL) {
            break;
        }
        node = response;
    }

    free_node(root);
    return 0;
}