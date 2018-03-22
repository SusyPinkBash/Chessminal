//
//  main.c
//  Chessminal
//
//  Created by Susanna Ardigo' on 22.03.18.
//  Copyright © 2018 Susanna Ardigo'. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chessboard.h"

int main(int argc, const char * argv[]) {
    
    struct chessboard cb;
    
//    white_board(&cb);
    init_chessboard(&cb);

    print_chessboard(&cb);
    

//    init_board(&cb);
    
    
    
    return 0;
}
