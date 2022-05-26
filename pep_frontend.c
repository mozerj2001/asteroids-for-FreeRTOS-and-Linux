#include <ncurses.h>
#include <stdio.h>

// define color pairs
#define SHIP_PAIR 1
#define ASTEROID_PAIR 2

// This function prints a line of the horizotal section of the screen.
// segment_coloring is a 7 integer long array, value 1 if the corresponding digit has ship, 2 if asteroid
void printHorizontalLine(unsigned int size_x, unsigned int segment_coloring[7]){
    unsigned int i = 0;
    unsigned int k = 0;

    for(i = 0; i < 7; i++){
        printw(" ");

        // turn coloring on if needed
        if(segment_coloring[i] == 1){
            attron(COLOR_PAIR(SHIP_PAIR));
        }
        else if(segment_coloring[i] == 2){
            attron(COLOR_PAIR(ASTEROID_PAIR));
        }

        // print segment
        for(k = 0; k < size_x; k++){
            addch(ACS_CKBOARD);
        }

        // turn coloring off if needed
        if(segment_coloring[i] == 1){
            attroff(COLOR_PAIR(SHIP_PAIR));
        }
        else if(segment_coloring[i] == 2){
            attroff(COLOR_PAIR(ASTEROID_PAIR));
        }
        
    }
    printw(" ");
    printw("\n");
}

// This function prints the next set of vertical segments. 
void printVerticalLine(unsigned int size_x, unsigned int size_y, unsigned int segment_coloring[7]){
    unsigned int i = 0;
    unsigned int k = 0;
    unsigned int j = 0;

    for(j = 0; j < size_y; j++){
        for(i = 0; i < 7; i++){
        
            // turn coloring off if needed
            if(segment_coloring[i] == 1){
                attron(COLOR_PAIR(SHIP_PAIR));
            }
            else if(segment_coloring[i] == 2){
                attron(COLOR_PAIR(ASTEROID_PAIR));
            }
            
            addch(ACS_CKBOARD);
            for(k = 0; k < size_x; k++){
                printw(" ");
            }
        
            // turn coloring off if needed
            if(segment_coloring[i] == 1){
                attroff(COLOR_PAIR(SHIP_PAIR));
            }
            else if(segment_coloring[i] == 2){
                attroff(COLOR_PAIR(ASTEROID_PAIR));
            }
        }
        addch(ACS_CKBOARD);
        printw("\n");
    }

}

// This function prints the game screen to the top of the terminal window.
// size_x and size_y give how many characters a 7 segment cell should be.
void printGameScreen(unsigned int size_x, unsigned int size_y, unsigned int horizontal_coloring[3][7], unsigned int vertical_coloring[2][7]){

    unsigned int row, col;
    
    printHorizontalLine(size_x, horizontal_coloring[0]);
    printVerticalLine(size_x, size_y, vertical_coloring[0]);
    printHorizontalLine(size_x, horizontal_coloring[1]);
    printVerticalLine(size_x, size_y, vertical_coloring[1]);
    printHorizontalLine(size_x, horizontal_coloring[2]);

    refresh();
}


int main(void){
    initscr();          // start curses mode

    // coordinate variables
    unsigned int x = 0;
    unsigned int y = 0;

    // create color pairs
    start_color();
    init_pair(SHIP_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(ASTEROID_PAIR, COLOR_RED, COLOR_BLACK);

    // arrays containing coloring data
    unsigned int horizontalColoring[3][7];
    unsigned int verticalColoring[2][7];

    // test horizontal colors:
    unsigned int i = 0;
    for(; i < 7; i++){ 
        horizontalColoring[0][i] = i%3;
        verticalColoring[0][i] = i%3;
    }
    for(i = 0; i < 7; i++){ 
        horizontalColoring[1][i] = (i+1)%3;
        verticalColoring[1][i] = (i+1)%3;
    }
    for(i = 0; i < 7; i++){ 
        horizontalColoring[2][i] = (i+2)%3;
    }



    raw();              // disable line buffering


    printGameScreen(6, 4, horizontalColoring, verticalColoring);

    getch();
    endwin();           // end curses mode
}
