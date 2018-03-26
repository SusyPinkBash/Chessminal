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
enum mstatus is_valid(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_bishop(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_rook(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_queen(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_knight(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_pawn(struct chessboard * cb, enum player player, struct move from, struct move to);
enum mstatus is_valid_king(struct chessboard * cb, enum player player, struct move from, struct move to);
int is_valid_move_up (struct chessboard * cb, struct move from, struct move to);
int is_valid_move_down (struct chessboard * cb, struct move from, struct move to);
int is_valid_move_right (struct chessboard * cb, struct move from, struct move to);
int is_valid_move_left (struct chessboard * cb, struct move from, struct move to);
void pawn_promotion (struct chessboard * cb);
void check_castiling (struct chessboard * cb, struct move from, struct move to);
enum pieces get_piece (struct chessboard * cb, struct move move);
enum player piece_color (struct chessboard * cb, struct move move);
int piece_in_cell (struct chessboard * cb, struct move move);



// ############### MAIN ###############
int main(int argc, const char * argv[]) {
    struct chessboard cb;
    init_chessboard(&cb);
    
    int game_on = 1;
    int valid_input = 1;
    char* input = malloc(7*sizeof(char));
    
    enum player current_player = WHITE;
    //    char* input = malloc(7*sizeof(char));
    do {
        if (valid_input) print_chessboard(&cb);
        valid_input = 1;
        
        
        if (current_player == WHITE) printf("\nw: ");
        else printf("\nb: ");

        
        input = get_input(input, 6);
        
        //        printf("\ninput: %s", input);
        // check draw and resign
        if (check_input(input, "draw")) {
            if (current_player == WHITE)
                printf("\nPlayer %c wants to draw, to accept type 'draw'\n", 'w');
            else printf("\nPlayer %c wants to draw, to accept type 'draw'\n", 'b');
            
            
//            input = get_input(input, 6);
            
            if (check_input(get_input(input, 6), "draw")) {
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
        
        if (check_input(input, "resign")) {
            
            printf("You resigned\n");
            if (current_player == WHITE)
                printf("Black won\n");
            else printf("White won\n");
            game_on = 0;
        }
        
        // do move
        //        printf("%s\n", input);
        for (int i=0; i<6; ++i) {
            //            printf("%c\n",input[i]);
            if ((i==0 || i==3) && ((int) input[i] < 97 || (int) input[i] > 104)) {
                valid_input = 0;
                break;
            }
            else if ((i==1 || i==4) && ((int) input[i] < 49 || (int) input[i] > 56)) {
                valid_input = 0;
                break;
            }
            else if ((i==2) && (int) input[i] != 32 ) {
                valid_input = 0;
                break;
            }
            else if ((i==5) && ((int) input[i] != 0 && ((int) input[i] != 32))) {
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
        
        enum mstatus status = move_piece(&cb, current_player, &input[0], &input[3]);
        //        printf("the move is: %u\n", status);
        if (status == INVALID) {
            printf("Invalid move, please insert a valid move");
            valid_input = 0;
            continue;
        }
        
        // switch player
        if (current_player== WHITE) current_player = BLACK;
        else current_player = WHITE;
//                current_player = (++current_player % 2);
        
    }
    while (game_on);
    
    free(input);
    
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
//        cb->position[1][i] = BLACK_PAWN;
        cb->position[1][i] = EMPTY;
        cb->position[2][i] = EMPTY;
        cb->position[3][i] = EMPTY;
        cb->position[4][i] = EMPTY;
        cb->position[5][i] = EMPTY;
        cb->position[6][i] = EMPTY;
//        cb->position[6][i] = WHITE_PAWN;
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
    struct move move_to = {to[0] - 'a', abs((int) to[1] - '1' - 7)};
    //    printf("from: %d%d, to:%d%d\n", move_from.col, move_from.row, move_to.col, move_to.row);
    //    printf("%u == %u\n", p, piece_color(cb, move_from));
    if (!piece_in_cell(cb, move_from) || (p != piece_color(cb, move_from)))
        return INVALID;
    enum mstatus validity = is_valid(cb, p, move_from, move_to);
    return validity;
    //    return CHECK_MATE;
}

// ####### IS VALID FUNCTIONS #######
enum mstatus is_valid(struct chessboard * cb, enum player player, struct move from, struct move to) {
//    printf("entro nella funzione\n");
//    printf("%u\n", piece_color(cb, from));
    if (piece_in_cell(cb, to)  && (piece_color(cb, from) == piece_color(cb, to)))
        return INVALID;
    enum mstatus validity;
    
//    printf("entro nello switch\n");
    
    switch(get_piece(cb, from)) {
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
            validity = is_valid_king(cb, player, from, to);
            break;
    }
//    printf("valid = %d\n",validity);
    if (validity == VALID || validity == CHECK) {
//        printf("from: %d%d, to:%d%d\n", (from.col), from.row, to.col, to.row);
        cb->position[to.row][to.col] = get_piece(cb, from);
        cb->position[from.row][from.col] = EMPTY;
    }
    return validity;
}

enum mstatus is_valid_pawn(struct chessboard * cb, enum player player, struct move from, struct move to){
    int eat = piece_in_cell(cb, to); // true if it has to eat
    int promotion = 0;
    int side = abs(from.col - to.col);
    int height = abs(from.row - to.row);
    
    if ((player == WHITE && is_valid_move_down(cb, from, to)) || (player == BLACK && is_valid_move_up(cb, from, to)))
        return INVALID;
    
    //    printf("eat=%d\nside=%d\nheight=%d\n", eat, side, height);
    
    if (to.row == 0) { promotion = 1; }
    
    if (!eat && side == 0 && ((height == 1) || (height == 2 && (from.row == 6 || from.row == 1)))) {
        if (promotion)
            pawn_promotion(cb); //TODO to implement
        return VALID;
    }
    if (eat && side == 1 && height == 1) {
        if (promotion)
            pawn_promotion(cb); //TODO to implement
        return VALID;
    }
    //    printf("NOPE\n");
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
    
    if(is_valid_move_up(cb, from, to)) {
        if (is_valid_move_right(cb, from, to)) {
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
        if (is_valid_move_right(cb, from, to)) {
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
    
    if (is_valid_move_up(cb, from ,to)) {
        for (int i=1; i<height; ++i) {
            if (cb->position[from.row-i][from.col] != 0)
                return INVALID;
        }
    }
    else if (is_valid_move_down(cb, from, to)) {
        for (int i=1; i<side; ++i) {
            if (cb->position[from.row+i][from.col] != 0)
                return INVALID;
        }
    }
    else if (is_valid_move_right(cb, from, to)) {
        for (int i=1; i<side; ++i) {
            if (cb->position[from.row][from.col+i] != 0)
                return INVALID;
        }
    }
    else if (is_valid_move_left(cb, from, to)) {
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

enum mstatus is_valid_king(struct chessboard * cb, enum player player, struct move from, struct move to){
    //TODO implement
    
    if (piece_in_cell(cb, to))
        return INVALID;
    
    int side = abs(from.col - to.col);
    int height = abs(from.row - to.row);
    
    if (side > 2 || height > 1 || (side == 2 && (height != 0 || from.row != 0 || from.row != 7 )))
        return INVALID;
 
    if ((from.row == 0 || from.row == 7 ) && height == 0 && side == 2)
        check_castiling(cb, from, to);
        
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

void pawn_promotion (struct chessboard * cb) {
    //TODO do something
}

void check_castiling (struct chessboard * cb, struct move from, struct move to) {
    //TODO do something
}



/* Returns 1 if the direction is up, 0 otherwise */
int is_valid_move_up (struct chessboard * cb, struct move from, struct move to) {
    return from.row > to.row;
}

/* Returns 1 if the direction is down, 0 otherwise */
int is_valid_move_down (struct chessboard * cb, struct move from, struct move to) {
    return from.row < to.row;
}

/* Returns 1 if the direction is right, 0 otherwise */
int is_valid_move_right (struct chessboard * cb, struct move from, struct move to) {
    return from.col < to.col;
}

/* Returns 1 if the direction is left, 0 otherwise */
int is_valid_move_left (struct chessboard * cb, struct move from, struct move to) {
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
    if (!strcmp(check, "draw"))
        len = 4;
    for (int i=0; i<len; ++i) {
        //    printf("%c = %c\n", input[i], draw[i]);
        if (input[i] != check[i])
            return 0;
    }
    return 1;
}



