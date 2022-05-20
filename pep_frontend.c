#include <ncurses.h>
#include <stdio.h>

// This function prints a line of the horizotal section of the screen.
void printHorizontalLine(unsigned int size_x){
    unsigned int i = 0;
    unsigned int k = 0;

    for(i = 0; i < 8; i++){
        printw(" ");
        for(k = 0; k < size_x; k++){
            addch(ACS_CKBOARD);
        }
    }
    printw(" ");
    printw("\n");
}

// This function prints the next set of vertical segments. 
void printVerticalLine(unsigned int size_x, unsigned int size_y){
    unsigned int i = 0;
    unsigned int k = 0;
    unsigned int j = 0;

    for(j = 0; j < size_y; j++){
        for(i = 0; i < 8; i++){
            addch(ACS_CKBOARD);
            for(k = 0; k < size_x; k++){
                printw(" ");
            }
        }
        addch(ACS_CKBOARD);
        printw("\n");
    }

}

// This function prints the game screen to the center of the terminal window.
// size_x and size_y give how many characters a 7 segment cell should be.
void printGameScreen(unsigned int size_x, unsigned int size_y){

    unsigned int row, col;
    
    printHorizontalLine(size_x);
    printVerticalLine(size_x, size_y);
    printHorizontalLine(size_x);
    printVerticalLine(size_x, size_y);
    printHorizontalLine(size_x);

    refresh();


    

}


int main(void){
    // coordinate variables
    unsigned int x = 0;
    unsigned int y = 0;


    initscr();          // start curses mode
    raw();              // disable line buffering


    printGameScreen(6, 8);

    getch();
    endwin();           // end curses mode
}
