//
//  chessboard.c
//  Chessminal
//
//  Created by Susanna Ardigo' on 22.03.18.
//  Copyright © 2018 Susanna Ardigo'. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <assert.h>
#include "chessboard.h"

// ############### EXTRA DECLARATIONS ###############
struct move {
    int col;
    int row;
};

char* get_input(char * input, int len);
int check_input(char * input, char * check);
enum mstatus move_piece(struct chessboard * cb, enum player p, const char * from, const char * to);
enum mstatus is_valid(struct chessboard * cb, enum player player, struct move from, struct move to, const char * array);
enum mstatus is_valid_bishop(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_rook(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_queen(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_knight(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_pawn(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_king(struct chessboard * cb, enum player player, struct move from, struct move to, const char * array);
void pawn_promotion (struct chessboard * cb);
int check_castling (struct chessboard * cb, enum player player, struct move from, struct move to, const char * array);
void update_king_coordinats (enum player player, struct move to, char * pointer);
void update_piece_was_moved(int piece, char * pointer);
enum pieces get_piece (struct chessboard * cb, struct move move);
enum player piece_color (struct chessboard * cb, struct move move);

int is_going_up (struct chessboard * cb, struct move from, struct move to);
int is_going_down (struct chessboard * cb, struct move from, struct move to);
int is_going_right (struct chessboard * cb, struct move from, struct move to);
int is_going_left (struct chessboard * cb, struct move from, struct move to);
int piece_in_cell (struct chessboard * cb, struct move move);



// ############### MAIN ###############
int main(int argc, const char * argv[]) {
    struct chessboard cb;
    init_chessboard(&cb);
    
    int game_on = 1;
    int valid_input = 1;
    char* array = malloc(16*sizeof(char));
    /* char * array structure:
     ┌───┬───┬───┬───┬───┬───┬───┬───┐
     └───┴───┴───┴───┴───┴───┴───┴───┘
     0 -  5 => input from stdio
     positions:
     6 -  7 => white king positions
     8 -  9 => black king positions
     if pieces were moved: 0 not moved 1 was moved:
     10 => white king
     11 => black king
     12 => white left rook
     13 => white right rook
     14 => black left rook
     15 => black right rook
     */
    
    
    array[6] = 4;
    array[7] = 7;
    array[8] = 4;
    //    array[9] = 0;
    for (int i=9; i<16; ++i) {
        array[i] = 0;
    }
    
    //
    enum player current_player = WHITE;
    
    
    do {
        if (valid_input) print_chessboard(&cb);
        valid_input = 1;
        
        if (current_player == WHITE) printf("\nw: ");
        else printf("\nb: ");
        
        array = get_input(array, 6);
        
        // check draw and resign
        if (check_input(array, "draw")) {
            if (current_player == WHITE)
                printf("\nPlayer %c wants to draw, to accept type 'draw'\n", 'w');
            else printf("\nPlayer %c wants to draw, to accept type 'draw'\n", 'b');
            
            if (check_input(get_input(array, 6), "draw")) {
                printf("Game ended with DRAW\n");
                
                game_on = 0;
                break;
            }
            else {
                printf("Draw not accpeted, please enter a move\n");
                valid_input = 0;
                continue;
            }
        }
        
        if (check_input(array, "resign")) {
            printf("You resigned\n");
            if (current_player == WHITE)
                printf("Black won\n");
            else printf("White won\n");
            game_on = 0;
            break;
        }
        
        // do move
        for (int i=0; i<6; ++i) {
            if ((i==0 || i==3) && ((int) array[i] < 97 || (int) array[i] > 104)) {
                valid_input = 0;
                break;
            }
            else if ((i==1 || i==4) && ((int) array[i] < 49 || (int) array[i] > 56)) {
                valid_input = 0;
                break;
            }
            else if ((i==2) && (int) array[i] != 32 ) {
                valid_input = 0;
                break;
            }
            else if ((i==5) && ((int) array[i] != 0 && ((int) array[i] != 32))) {
                valid_input = 0;
                break;
            }
        }
        if (!valid_input) {
            printf("Invalid input, please insert a valid move");
            valid_input = 0;
            continue;
        }
        
        //        const char * from = strsep(&input, " ");
        //        const char * to  = strsep(&input, " ");
        
        enum mstatus status = move_piece(&cb, current_player, &array[0], &array[3]);
        if (status == VALID) {
            struct move move_from = {array[0] - 'a', abs((int) array[1] - '1' - 7)};
            struct move move_to = {array[3] - 'a', abs((int) array[4] - '1' - 7)};;
            enum pieces piece = get_piece(&cb, move_to);
            if (piece == WHITE_KING) {
                update_king_coordinats(current_player, move_to, &array[6]);
                if (move_from.row == 7 && move_from.col == 4 && array[10] == 0) {
                    update_piece_was_moved(0, &array[10]);
                }
            }
            else if (piece == BLACK_KING) {
                update_king_coordinats(current_player, move_to, &array[6]);
                if (move_from.row == 0 && move_from.col == 4 && array[11] == 0) {
                    update_piece_was_moved(1, &array[10]);
                }
            }
            
            else if (piece == WHITE_ROOK && move_from.row == 7) {
                if (array[12] == 0 && move_from.col == 0) {
                    update_piece_was_moved(2, &array[10]);
                }
                else if (array[13] == 0 && move_from.col == 7) {
                    update_piece_was_moved(3, &array[10]);
                }
            }
            else if (piece == BLACK_ROOK && move_from.row == 0) {
                if (array[14] == 0 && move_from.col == 0) {
                    update_piece_was_moved(4, &array[10]);
                }
                else if (array[15] == 0 && move_from.col == 7) {
                    update_piece_was_moved(5, &array[10]);
                }
            }
            
            //            printf("AFTER:\n");
            //            printf("white king: %d%d\n", input[6], input[7]);
            //            printf("black king: %d%d\n", input[8], input[9]);
            
            // check if king was moved
        }
        else if (status == INVALID) {
            printf("INVALID MOVE\nPlease insert a valid move");
            valid_input = 0;
            continue;
        }
        else if (status == CHECK) {
            printf("CHECK");
        }
        else if (status == CHECK_MATE) {
            printf("CHECKMATE");
            game_on = 1;
            break;
        }
        
        
        // switch player
        if (current_player == WHITE) current_player = BLACK;
        else current_player = WHITE;
        
    }
    while (game_on);
    
    free(array);
    
    return 0;
}


// ############### CHESSBOARD ###############

/* print the current position of the pieces on the board */
void print_chessboard(struct chessboard * cb) {
    printf("\n%s\n", "      a   b   c   d   e   f   g   h");
    printf("%s\n", "    ┌───┬───┬───┬───┬───┬───┬───┬───┐");
    for (int i=0; i < 8; ++i) {
        printf(" %d  |", 8-i);
        for (int j=0; j<8; ++j) {
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

/* set the initial position of the pieces on the board */
void init_chessboard(struct chessboard * cb) {

    
        cb->position[0][0] = BLACK_ROOK;
        cb->position[0][1] = BLACK_KNIGHT;
        cb->position[0][2] = BLACK_BISHOP;
        cb->position[0][3] = BLACK_QUEEN;
        cb->position[0][4] = BLACK_KING;
        cb->position[0][5] = BLACK_BISHOP;
        cb->position[0][6] = BLACK_KNIGHT;
        cb->position[0][7] = BLACK_ROOK;
    
        for (int i = 0; i < 8; ++i) {
            cb->position[1][i] = BLACK_PAWN;
            cb->position[2][i] = EMPTY;
            cb->position[3][i] = EMPTY;
            cb->position[4][i] = EMPTY;
            cb->position[5][i] = EMPTY;
            cb->position[6][i] = WHITE_PAWN;
        }
    
        cb->position[7][0] = WHITE_ROOK;
        cb->position[7][1] = WHITE_KNIGHT;
        cb->position[7][2] = WHITE_BISHOP;
        cb->position[7][3] = WHITE_QUEEN;
        cb->position[7][4] = WHITE_KING;
        cb->position[7][5] = WHITE_BISHOP;
        cb->position[7][6] = WHITE_KNIGHT;
        cb->position[7][7] = WHITE_ROOK;
    
}


// ####### MOVE FUNCTIONS #######
enum mstatus move_piece(struct chessboard * cb, enum player p, const char * from, const char * to) {
    struct move move_from = {from[0] - 'a', abs((int) from[1] - '1' - 7)};
    struct move move_to = {to[0] - 'a', abs((int) to[1] - '1' - 7)};;
    //    struct move white_king = {from[6], from[7]};
    //    struct move black_king = {from[8], from[9]};
    //    printf("BEFORE:\n");
    //    printf("white king: %d%d\n", white_king.col, white_king.row);
    //    printf("black king: %d%d\n", black_king.col, black_king.row);
    
    if (!piece_in_cell(cb, move_from) || (p != piece_color(cb, move_from)))
        return INVALID;
    enum mstatus validity = is_valid(cb, p, move_from, move_to, &to[3]);
    //    if (validity && get_piece(cb, move_from) == WHITE_KING && get_piece(cb, move_from) == BLACK_KING)
    //        update_king_coordinates(p, move_to, &from[6]);
    //    printf("AFTER:\n");
    //    printf("white king: %d%d\n", white_king.col, white_king.row);
    //    printf("black king: %d%d\n", black_king.col, black_king.row);
    return validity;
}

// ####### IS VALID FUNCTIONS #######
enum mstatus is_valid(struct chessboard * cb, enum player player, struct move from, struct move to, const char * array) {
    if (piece_in_cell(cb, to)  && (piece_color(cb, from) == piece_color(cb, to)))
        return INVALID;
    enum mstatus validity;
    enum pieces piece_from = get_piece(cb, from);
    
    switch(piece_from) {
        case EMPTY:
            return INVALID;
        case WHITE_PAWN:
        case BLACK_PAWN:
            validity = is_valid_pawn(cb, player, from, to);
            break;
        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
            validity = is_valid_knight(cb, player, from, to);
            break;
        case WHITE_BISHOP:
        case BLACK_BISHOP:
            validity = is_valid_bishop(cb, player, from, to);
            break;
        case WHITE_ROOK:
        case BLACK_ROOK:
            validity = is_valid_rook(cb, player, from, to);
            break;
        case WHITE_QUEEN:
        case BLACK_QUEEN:
            validity = is_valid_queen(cb, player, from, to);
            break;
        case WHITE_KING:
        case BLACK_KING:
            validity = is_valid_king(cb, player, from, to, &array[4]);
            break;
    }
    if (validity == VALID || validity == CHECK) {
        cb->position[to.row][to.col] = get_piece(cb, from);
        cb->position[from.row][from.col] = EMPTY;
//        printf("piece from: %u\n", piece_from);
        
        if (piece_from == WHITE_PAWN && to.row == 0) {
            int piece_name_not_valid;
            do {
                piece_name_not_valid = 0;
                printf("You reached the opposite rank, type: queen, rook, bishop or knight.\n");
                char* new_input = malloc(6*sizeof(char));
                new_input = get_input(new_input, 6);
                if (check_input(new_input, "queen"))
                    cb->position[to.row][to.col] = WHITE_QUEEN;
                else if (check_input(new_input, "rook"))
                    cb->position[to.row][to.col] = WHITE_ROOK;
                else if (check_input(new_input, "bishop"))
                    cb->position[to.row][to.col] = WHITE_BISHOP;
                else if (check_input(new_input, "knight"))
                    cb->position[to.row][to.col] = WHITE_KNIGHT;
                else {
                    printf ("invalid input, please provide a valid piece.\n");
                    piece_name_not_valid = 1;
                }
                free(new_input);
            }
            while(piece_name_not_valid);
            
        }
        else if (piece_from == BLACK_PAWN && to.row == 7) {
            int piece_name_not_valid;
            printf("You reached the opposite rank, type: queen, rook, bishop or knight.\n");
            do {
                piece_name_not_valid = 0;
//                printf("You reached the opposite rank, type: queen, rook, bishop or knight.\n");
                char* new_input = malloc(6*sizeof(char));
                new_input = get_input(new_input, 6);
                if (check_input(new_input, "queen"))
                    cb->position[to.row][to.col] = BLACK_QUEEN;
                else if (check_input(new_input, "rook"))
                    cb->position[to.row][to.col] = BLACK_ROOK;
                else if (check_input(new_input, "bishop"))
                    cb->position[to.row][to.col] = BLACK_BISHOP;
                else if (check_input(new_input, "knight"))
                    cb->position[to.row][to.col] = BLACK_KNIGHT;
                else {
                    printf ("invalid input, please provide a valid piece.\n");
                    piece_name_not_valid = 1;
                }
                free(new_input);
            }
            while(piece_name_not_valid);
            
        }
        
    }
    
    return validity;
}

enum mstatus is_valid_pawn(struct chessboard * cb, enum player player, struct move from, struct move to){
    int eat = piece_in_cell(cb, to); // true if it has to eat
    //    int promotion = 0;
    int side = abs(from.col - to.col);
    int height = abs(from.row - to.row);
    
    if ((player == WHITE && is_going_down(cb, from, to)) || (player == BLACK && is_going_up(cb, from, to)))
        return INVALID;
    
    
    //    if (to.row == 0 || to.row == 7) { promotion = 1; }
    
    if (!eat && side == 0 && ((height == 1) || (height == 2 && (from.row == 6 || from.row == 1)))) {
        //        if (promotion)
        //            pawn_promotion(cb); //TODO to implement
        return VALID;
    }
    if (eat && side == 1 && height == 1) {
        //        if (promotion)
        //            pawn_promotion(cb); //TODO to implement
        return VALID;
    }
    // TODO implement 'en passant' capture
    return INVALID;
}


enum mstatus is_valid_knight(struct chessboard * cb, enum player player, struct move from, struct move to){
    int side = abs(from.col - to.col);
    int height = abs(from.row - to.row);
    if ((side == 1 && height == 2) || (side ==2 && height ==1))
        return VALID;
    return INVALID;
}


enum mstatus is_valid_bishop(struct chessboard * cb, enum player player, struct move from, struct move to){
    int side = abs(from.col - to.col);
    int height = abs(from.row - to.row);
    //    int eat = piece_in_cell(cb, to); // true if it has to eat
    if (side != height) {
        return INVALID; }
    
    if(is_going_up(cb, from, to)) {
        if (is_going_right(cb, from, to)) {
            // going up right
            for (int i=1; i<side; ++i) {
                if (cb->position[from.row-i][from.col+i] != 0)
                    return INVALID;
            }
        }
        else {
            // going up left
            for (int i=1; i<side; ++i) {
                if (cb->position[from.row-i][from.col-i] != 0)
                    return INVALID;
            }
        }
    }
    else {
        // going down
        if (is_going_right(cb, from, to)) {
            // going down right
            for (int i=1; i<side; ++i) {
                if (cb->position[from.row+i][from.col+i] != 0)
                    return INVALID;
            }
        }
        else {
            // going down left
            for (int i=1; i<side; ++i) {
                if (cb->position[from.row+i][from.col-i] != 0)
                    return INVALID;
            }
        }
    }
    return VALID;
}

enum mstatus is_valid_rook(struct chessboard * cb, enum player player, struct move from, struct move to){
    int side = abs(from.col - to.col);
    int height = abs(from.row - to.row);
    if (side < 0 || height < 0 || (side > 0 && height > 0))
        return INVALID;
    
    if (is_going_up(cb, from ,to)) {
        for (int i=1; i<height; ++i) {
            if (cb->position[from.row-i][from.col] != 0)
                return INVALID;
        }
    }
    else if (is_going_down(cb, from, to)) {
        for (int i=1; i<side; ++i) {
            if (cb->position[from.row+i][from.col] != 0)
                return INVALID;
        }
    }
    else if (is_going_right(cb, from, to)) {
        for (int i=1; i<side; ++i) {
            if (cb->position[from.row][from.col+i] != 0)
                return INVALID;
        }
    }
    else if (is_going_left(cb, from, to)) {
        for (int i=1; i<side; ++i) {
            if (cb->position[from.row][from.col-i] != 0)
                return INVALID;
        }
    }
    
    return VALID;
}

enum mstatus is_valid_queen(struct chessboard * cb, enum player player, struct move from, struct move to){
    if (is_valid_bishop(cb, player, from, to) == VALID || is_valid_rook(cb, player, from, to) == VALID)
        return VALID;
    return INVALID;
}

enum mstatus is_valid_king(struct chessboard * cb, enum player player, struct move from, struct move to, const char * pointer){
    //TODO implement
    int castling = 0;
    if (piece_in_cell(cb, to))
        return INVALID;
    
    int side = abs(from.col - to.col);
    int height = abs(from.row - to.row);
    
    //    printf("side= %d, height=%d\n", side, height);
    //    if (side > 2 || height > 1 || (side == 2 && (height != 0 || from.row != 0 || from.row != 7 )))
    if (side == 2 && height == 0 && (!(from.row == 0 || from.row == 7)))
        return INVALID;
    
    
    
    if (height == 0 && side == 2 && ((from.row == 0 && player == BLACK ) || (from.row == 7 && player == WHITE)))
        castling = check_castling(cb, player, from, to, pointer);
        
        
        if (castling)
            return VALID;
    
    if (castling == 0 && side == 2)
        return INVALID;
    
    
    if (side > 1 || height > 1)
        return INVALID;
    
    return VALID;
}


// ####### MOVE HELPER FUNCTIONS #######
/* Returns the color of a given piece on a given chessboard */
enum pieces get_piece (struct chessboard * cb, struct move move) {
    return cb->position[move.row][move.col];
}

/* Returns the piece on a given chessboard */
enum player piece_color (struct chessboard * cb, struct move move) {
    //    printf("piece: %d\n", get_piece(cb, move));
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

//void pawn_promotion (struct chessboard * cb) {
//    printf("You have reach");
//    //TODO do something
//
//}

int check_castling (struct chessboard * cb, enum player player, struct move from, struct move to, const char * pointer) {
    // TODO: check if the pieces were not moved;
    if (from.col != 4) return 0;
    if (player == WHITE) {
        if  (pointer[0] == 1) return 0;
        // riga 7
        if (is_going_right(cb, from, to)) {
            if  (pointer[3] == 1) return 0;
            struct move torre = {7, 7};
            if (get_piece(cb, torre) == WHITE_ROOK) {
                for (int i=from.col+1; i<torre.col; ++i) {
                    if (cb->position[i][7] != EMPTY)
                        return 0;
                }
                cb->position[7][7] = EMPTY;
                cb->position[7][5] = WHITE_ROOK;
            }
        }
        else if (is_going_left(cb, from, to)) {
            if  (pointer[2] == 1) return 0;
            struct move torre = {0, 7};
            if (get_piece(cb, torre) == WHITE_ROOK) {
                for (int i=torre.col+1; i<from.col; ++i) {
                    if (cb->position[i][7] != EMPTY)
                        return 0;
                }
                cb->position[7][0] = EMPTY;
                cb->position[7][3] = WHITE_ROOK;
            }
        }
        
    }
    else {
        if  (pointer[1] == 1) return 0;
        // riga 0
        if (is_going_right(cb, from, to)) {
            if  (pointer[5] == 1) return 0;
            struct move torre = {7, 0};
            if (get_piece(cb, torre) == BLACK_ROOK) {
                for (int i=from.col+1; i<torre.col; ++i) {
                    if (cb->position[i][0] != EMPTY)
                        return 0;
                }
                cb->position[0][7] = EMPTY;
                cb->position[0][5] = BLACK_ROOK;
            }
        }
        else if (is_going_left(cb, from, to)) {
            if  (pointer[4] == 1) return 0;
            struct move torre = {0, 0};
            if (get_piece(cb, torre) == BLACK_ROOK) {
                for (int i=torre.col+1; i<from.col; ++i) {
                    if (cb->position[i][7] != EMPTY)
                        return 0;
                }
                cb->position[0][0] = EMPTY;
                cb->position[0][3] = BLACK_ROOK;
            }
        }
    }
    return 1;
}

void update_king_coordinats (enum player player, struct move to, char * pointer) {
    if (player == WHITE) {
        pointer[0] = to.col;
        pointer[1] = to.row;
    }
    else {
        pointer[2] = to.col;
        pointer[3] = to.row;
    }
}

void update_piece_was_moved(int piece, char * pointer) {
    pointer[piece] = 1;
}


/* Returns 1 if the direction is up, 0 otherwise */
int is_going_up (struct chessboard * cb, struct move from, struct move to) {
    return from.row > to.row;
}

/* Returns 1 if the direction is down, 0 otherwise */
int is_going_down (struct chessboard * cb, struct move from, struct move to) {
    return from.row < to.row;
}

/* Returns 1 if the direction is right, 0 otherwise */
int is_going_right (struct chessboard * cb, struct move from, struct move to) {
    return from.col < to.col;
}

/* Returns 1 if the direction is left, 0 otherwise */
int is_going_left (struct chessboard * cb, struct move from, struct move to) {
    return from.col > to.col;
}

// ####### OTHER HELPER FUNCTIONS #######

/* read the input */
char* get_input(char * input, int len) {
    char c;
    int i =0;
    while ((c=getchar()) !=  '\n') {
        if (i>len-1)
            continue;
        input[i] = c;
        ++i;
    }
    while (i < len) {
        input[i] = (char) 32;
        ++i;
    }
    return input;
}
/* checks if the input is equal to a string */
int check_input(char * input, char * check) {
    int len = 6;
    if (!strcmp(check, "draw") || !strcmp(check, "rook"))
        len = 4;
    if (!strcmp(check, "queen"))
        len = 5;
    for (int i=0; i<len; ++i) {
        if (input[i] != check[i])
            return 0;
    }
    return 1;
}



