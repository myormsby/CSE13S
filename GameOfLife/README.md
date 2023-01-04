# Assignment 4 : The Game of Life

##  Description
./life is a program that runs John Horton Conway's Game of Life. The rules are any live cell with two or three live neighbors survives, any dead cell with exactly three neighbors becomes a live cell, and any other cell will die from loneliness or overcrowding. The game of life is displayed using ncurses. 

Also included is a Makefile that will compile, build, and clean using a pre-programmed script to compile all the files together into an executable file labeled sorting.

## Arguments

-t:  Makes the grid the game of life is played on into a torus (like a bagel).

-s:  Silences ncurses, meaning there will be no display and the program will only output the final grid.

-n generations: Specifies how many generations the game of life will run through. The default is 100.

-i input: Specifies the input file that will populate the universe. The default is stdin.
 
-o output: Specifies the output file that the final universe will print into. The default is stdout. 

## How to Run

To build; type "make", "make all", or "make life" into the command line with the Makefile in the directory. This will compile and build the program using preset flags.

To clean; type "make clean" into the command line with the Makefile in the directory. This will clean the files created by the Makefile.

To format; type "make format" into the command line with the Makefile in the directory. This will format all .c and .h files using the rules specified in .clang-format.

##
