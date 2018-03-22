//
//  chessboard.h
//  Chessminal
//
//  Created by Susanna Ardigo' on 22.03.18.
//  Copyright © 2018 Susanna Ardigo'. All rights reserved.
//

#ifndef CHESSBOARD_H_INCLUDED
#define CHESSBOARD_H_INCLUDED

/* players
 */
enum player {WHITE, BLACK};

/* status return by move()
 */
enum mstatus {INVALID, VALID, CHECK, CHECK_MATE};

enum pieces {
    EMPTY=0,
    WHITE_PAWN=1, WHITE_KNIGHT=2, WHITE_BISHOP=3, WHITE_ROOK=4, WHITE_QUEEN=5, WHITE_KING=6,
    BLACK_PAWN=7, BLACK_KNIGHT=8, BLACK_BISHOP=9, BLACK_ROOK=10, BLACK_QUEEN=11, BLACK_KING=12 };

/* UTF8 representation of the pieces, should be used to print the board
 */
const static char* utf_8_pieces[13] = {
    " ", "♙", "♘", "♗", "♖", "♕", "♔", "♟", "♞", "♝", "♜", "♛", "♚" };

/* chess board with values defined in enum pieces
 */
struct chessboard {
    enum pieces position[8][8];
};

/* set the initial position of the pieces on the board
 */
void init_chessboard(struct chessboard * cb);

/* print the board like this:
 
 a   b   c   d   e   f   g   h
 ┌───┬───┬───┬───┬───┬───┬───┬───┐
 8  │ ♜ │ ♞ │ ♝ │ ♛ │ ♚ │ ♝ │ ♞ │ ♜ │
 ├───┼───┼───┼───┼───┼───┼───┼───┤
 7  │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │ ♟ │
 ├───┼───┼───┼───┼───┼───┼───┼───┤
 6  │   │   │   │   │   │   │   │   │
 ├───┼───┼───┼───┼───┼───┼───┼───┤
 5  │   │   │   │   │   │   │   │   │
 ├───┼───┼───┼───┼───┼───┼───┼───┤
 4  │   │   │   │   │   │   │   │   │
 ├───┼───┼───┼───┼───┼───┼───┼───┤
 3  │   │   │   │   │   │   │   │   │
 ├───┼───┼───┼───┼───┼───┼───┼───┤
 2  │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │ ♙ │
 ├───┼───┼───┼───┼───┼───┼───┼───┤
 1  │ ♖ │ ♘ │ ♗ │ ♕ │ ♔ │ ♗ │ ♘ │ ♖ │
 └───┴───┴───┴───┴───┴───┴───┴───┘
 a   b   c   d   e   f   g   h
 
 */
void print_chessboard(struct chessboard * cb);

/* apply the move informed by player p, from/to positions and returns
 * the corresponding status
 */
enum mstatus move(struct chessboard * cb, enum player p, const char * from, const char * to);

#endif
