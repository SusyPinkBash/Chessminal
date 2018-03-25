//
//  moves.h
//  Chessminal
//
//  Created by Susanna Ardigo' on 22.03.18.
//  Copyright © 2018 Susanna Ardigo'. All rights reserved.
//

#ifndef moves_h
#define moves_h

#include <stdio.h>
#include <stdlib.h>
#include "chessboard.h"

enum mstatus is_valid(struct chessboard * cb, struct move from, struct move to);

enum player piece_color (struct chessboard * cb, struct move mov);

enum pieces get_piece (struct chessboard * cb, struct move move);

int piece_in_cell (struct chessboard * cb, struct move move);

int move_up (struct chessboard * cb, struct move from, struct move to);
int move_down (struct chessboard * cb, struct move from, struct move to);
int move_right (struct chessboard * cb, struct move from, struct move to);
int move_left (struct chessboard * cb, struct move from, struct move to);

void pawn_promotion (struct chessboard * cb);
void check_castiling (struct chessboard * cb, struct move from, struct move to);

int col_num (char col);

#endif /* moves_h */
