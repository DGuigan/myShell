#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024		// maximum length of each line of input
#define MAX_CMDS 64		// maximun number of tokens taken from input 
#define DELIM " \n\t"		// token deliminators
