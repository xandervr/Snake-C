//
//  main.c
//  Snake
//
//  Created by Xander Van Raemdonck on 13/12/16.
//  Copyright © 2016 TnTap. All rights reserved.
//

#include <stdlib.h>
#include "game.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    srand((unsigned int) time(NULL));
    
    if(argc == 3) {
        initialize_game(atoi(argv[1]), atoi(argv[2]));
        run_game();
    }
    
    
    
    return 0;
}
