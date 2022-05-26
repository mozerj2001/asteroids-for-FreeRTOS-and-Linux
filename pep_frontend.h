#ifndef PEP_FRONTEND_H
#define PEP_FRONTEND_H

// printHorizontalLine: prints all segments for a single horizontal line of the 7 digit display
// INPUTS:  size_x: how many characters wide is a horizontal segment?
//          segment_coloring[7]: contains coloring instructions for the segments corresponding to the indexes (0: default, 1: green, 2: red)
void printHorizontalLine(unsigned int size_x, unsigned int segment_coloring[7]);


// printVerticalLine: prints all vertical segments in a single line (8)
// INPUTS:  size_x: how many characters wide is a horizontal segment?
//          size_y: how many characters high is a vertical segment?
//          segment_coloring[7]: contains coloring instructions for the segments corresponding to the indexes (0: default, 1: green, 2: red)
void printVerticalLine(unsigned int size_x, unsigned int size_y, unsigned int segment_coloring[7]);


// printGameScreen: prints the entire 7 digit 7 segment display into the terminal
// INPUTS:  size_x: how many characters wide is a horizontal segment?
//          size_y: how many characters high is a vertical segment?
//          horizontal_coloring[3][7]: contains coloring instructions for the printHorizontalLine function calls
//          vertical_coloring[2][7]: contains coloring instructions for the printVerticalLine function calls
void printGameScreen(unsigned int size_x, unsigned int size_y, unsigned int horizontal_coloring[3][7], unsigned int vertical_coloring[2][7]);














#endif
