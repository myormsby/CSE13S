# Assignment 3 : Sorting (Putting your affairs in order)

##  Description
Four sorts have been implemented into C files labeled heap.c, batcher.c, quick.c, and insert.c. The header files to these sorts are then used in sorting.c to create random arrays which are then sorted using the aforementioned C programs. The sorting.c file uses a getopt loop to take arguments and add them into a set to determine which sorts will be used. Optional arguments are the seed used to create the random arrays, the size of the arrays, and the elements printed from said arrays.

Also included is a Makefile that will compile, build, and clean using a pre-programmed script to compile all the files together into an executable file labeled sorting.

## How to Run

To build; type "make", "make all", or "make sorting" into the command line with the Makefile in the directory. This will compile and build the program using preset flags.

To clean; type "make clean" into the command line with the Makefile in the directory. This will clean the files created by the Makefile.

To format; type "make format" into the command line with the Makefile in the directory. This will format all .c and .h files using the rules specified in .clang-format.

##
