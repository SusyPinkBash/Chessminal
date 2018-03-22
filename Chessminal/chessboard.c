//
//  chessboard.c
//  Chessminal
//
//  Created by Susanna Ardigo' on 22.03.18.
//  Copyright © 2018 Susanna Ardigo'. All rights reserved.
//

#include <stdio.h>
#include "chessboard.h"

void print_chessboard(struct chessboard * cb) {
    printf("%s\n", "      a   b   c   d   e   f   g   h");
    printf("%s\n", "    ┌───┬───┬───┬───┬───┬───┬───┬───┐");
    for (int i=0; i < 8; i++) {
        printf(" %d  |", 8-i);
        for (int j=0; j<8; j++) {
            printf(" %s |",  utf_8_pieces[cb->position[i][j]]);
        }
        if (i!=7) {
            printf("\n%s\n", "    ├───┼───┼───┼───┼───┼───┼───┼───┤");
        }
        else {
            printf("%s","\n");
        }
        
    }
    printf("%s\n", "    └───┴───┴───┴───┴───┴───┴───┴───┘");
    printf("%s\n", "      a   b   c   d   e   f   g   h");
    
}

void init_chessboard(struct chessboard * cb) {
    
    for (int i = 0; i < 8; ++i) {
        cb->position[0][i] = BLACK_PAWN;
        cb->position[1][i] = BLACK_PAWN;
        
        cb->position[2][i] = EMPTY;
        cb->position[3][i] = EMPTY;
        cb->position[4][i] = EMPTY;
        cb->position[5][i] = EMPTY;
        
        cb->position[6][i] = WHITE_PAWN;
        cb->position[7][i] = WHITE_PAWN;
    }
    
}

