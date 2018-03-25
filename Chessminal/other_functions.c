//
//  other_functions.c
//  Chessminal
//
//  Created by Susanna Ardigo' on 25.03.18.
//  Copyright © 2018 Susanna Ardigo'. All rights reserved.
//

#include <stdio.h>
#include "chessboard.h"
#include "chessboard.c"

// DECLARATIONS
struct move {
    char col;
    int row;
};

enum player piece_color (struct chessboard * cb, struct move mov);
enum pieces get_piece (struct chessboard * cb, struct move move);
int piece_in_cell (struct chessboard * cb, struct move move);
int is_valid_move_up (struct chessboard * cb, struct move from, struct move to);
int is_valid_move_down (struct chessboard * cb, struct move from, struct move to);
int is_valid_move_right (struct chessboard * cb, struct move from, struct move to);
int is_valid_move_left (struct chessboard * cb, struct move from, struct move to);
void pawn_promotion (struct chessboard * cb);
void check_castiling (struct chessboard * cb, struct move from, struct move to);
int col_num (char col);


// ############### MOVES ###############

// 0 false, 1 true

// ####### IS VALID FUNCTIONS #######
enum mstatus is_valid(struct chessboard * cb, struct move from, struct move to) {
    if (!piece_in_cell(cb, from))
        return INVALID;
    
    if ((piece_in_cell(cb, to)) && (piece_color(cb, from) == piece_color(cb, to)))
        return INVALID;
    
    
    return INVALID;
}


enum mstatus is_valid_bishop(struct chessboard * cb, struct move from, struct move to){
    
    int side = abs(from.row - to.row);
    int height = abs(col_num(from.col) - col_num(to.row));
    if (side != height)
        return INVALID;
    if(is_valid_move_up(cb, from, to)) {
        if (is_valid_move_right(cb, from, to)) {
            // going up right
            for (int i=1; i<side; ++i) {
                if (cb->position[from.row-i][col_num(from.col)+i] != 0)
                    return INVALID;
            }
        }
        else {
            // going up left
            for (int i=1; i<side; ++i) {
                if (cb->position[from.row-i][col_num(from.col)-i] != 0)
                    return INVALID;
            }
        }
    }
    else {
        // going down
        if (is_valid_move_right(cb, from, to)) {
            // going down right
            for (int i=1; i<side; ++i) {
                if (cb->position[from.row+i][col_num(from.col)+i] != 0)
                    return INVALID;
            }
        }
        else {
            // going down left
            for (int i=1; i<side; ++i) {
                if (cb->position[from.row+i][col_num(from.col)-i] != 0)
                    return INVALID;
            }
        }
    }
    return VALID;
}

enum mstatus is_valid_rook(struct chessboard * cb, struct move from, struct move to){
    int side = abs(from.row - to.row);
    int height = abs(col_num(from.col) - col_num(to.row));
    if (side > 0 && height > 0) {
        return INVALID;
    }
    if (is_valid_move_up(cb, from ,to)) {
        for (int i=1; i<height; ++i) {
            if (cb->position[from.row-i][col_num(from.col)] != 0)
                return INVALID;
        }
    }
    else if (is_valid_move_down(cb, from, to)) {
        for (int i=1; i<side; ++i) {
            if (cb->position[from.row+i][col_num(from.col)] != 0)
                return INVALID;
        }
    }
    else if (is_valid_move_right(cb, from, to)) {
        for (int i=1; i<side; ++i) {
            if (cb->position[from.row][col_num(from.col)+i] != 0)
                return INVALID;
        }
    }
    else if (is_valid_move_left(cb, from, to)) {
        for (int i=1; i<side; ++i) {
            if (cb->position[from.row][col_num(from.col)-i] != 0)
                return INVALID;
        }
    }
    
    return VALID;
}

enum mstatus is_valid_queen(struct chessboard * cb, struct move from, struct move to){
    if (is_valid_bishop(cb, from, to) == VALID || is_valid_rook(cb, from, to) == VALID)
        return VALID;
    return INVALID;
}

enum mstatus is_valid_knight(struct chessboard * cb, struct move from, struct move to){
    int side = abs(from.row - to.row);
    int height = abs(col_num(from.col) - col_num(to.row));
    if ((side == 1 && height == 2) || (side ==2 && height ==1))
        return VALID;
    return INVALID;
}

enum mstatus is_valid_pawn(struct chessboard * cb, struct move from, struct move to){
    int eat = piece_in_cell(cb, to); // true if it has to eat
    int promotion = 0;
    int side = abs(from.row - to.row);
    int height = abs(col_num(from.col) - col_num(to.row));
    
    if (to.row == 0) promotion = 1;
        
        if (!eat && side == 0 && ((height == 1) || (height == 2 && from.row == 6))) {
            if (promotion)
                pawn_promotion(cb); //TODO to implement
            return VALID;
        }
    if (eat && side == 1 && height == 1) {
        if (promotion)
            pawn_promotion(cb); //TODO to implement
        return VALID;
    }
    
    // TODO implement 'en passant' capture
    return INVALID;
}

enum mstatus is_valid_king(struct chessboard * cb, struct move from, struct move to){
    //TODO implement
    
    if (piece_in_cell(cb, to))
        return INVALID;
    
    int side = abs(from.row - to.row);
    int height = abs(col_num(from.col) - col_num(to.row));
    
    if (side > 2 || height > 1 || (side == 2 && (height != 0 || from.row != 0 )))
        return INVALID;
    
    if (from.row == 0 && height ==0 && side == 2)
        check_castiling(cb, from, to);
        
        return VALID;
}


// ####### HELPER MOVE FUNCTIONS #######
void pawn_promotion (struct chessboard * cb) {
    //TODO do something
}

void check_castiling (struct chessboard * cb, struct move from, struct move to) {
    //TODO do something
}


/* Returns the piece on a given chessboard */
enum pieces get_piece (struct chessboard * cb, struct move move) {
    return cb->position[move.row][col_num(move.col)];
}

/* Returns the color of a given piece on a given chessboard */
enum player piece_color (struct chessboard * cb, struct move move) {
    if (get_piece(cb, move) < 7)
        return WHITE;
    else return BLACK;
}

/* Returns 1 if there is a piece, 0 otherwise */
int piece_in_cell (struct chessboard * cb, struct move move) {
    if (get_piece(cb, move) == 0)
        return 0;
    else return 1;
}

/* Returns 1 if the direction is up, 0 otherwise */
int is_valid_move_up (struct chessboard * cb, struct move from, struct move to) {
    if(col_num(from.col) < col_num(to.row)) {
        return 1;
    }
    else return 0;
}

/* Returns 1 if the direction is down, 0 otherwise */
int is_valid_move_down (struct chessboard * cb, struct move from, struct move to) {
    if(col_num(from.col) > col_num(to.row)) {
        return 1;
    }
    else return 0;
}

/* Returns 1 if the direction is right, 0 otherwise */
int is_valid_move_right (struct chessboard * cb, struct move from, struct move to) {
    if(from.row < to.row) {
        return 1;
    }
    else return 0;
}

/* Returns 1 if the direction is left, 0 otherwise */
int is_valid_move_left (struct chessboard * cb, struct move from, struct move to) {
    if(from.row > to.row) {
        return 1;
    }
    else return 0;
}

/* Returns the number of the column */
int col_num (char col) {
    switch(col) {
        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
    }
    return 0;
}
