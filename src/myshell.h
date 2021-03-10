/*
*  Name: David McGuigan
*  Student ID: 19425942
*
*  I hereby acknowledge the DCU Academic Integrity Policy
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 1024		// maximum length of each line of input
#define MAX_CMDS 64		    // maximun number of tokens taken from input 
#define DELIM " \n\t"		// token deliminators

extern char** environ;      // array of environment variables

typedef void (*func_ptr)(char*[], int); // custom type to easily store internal commands in array

void set_environment_variables(char* path);  // set SHELL and create MAN_PATH environment variables

void get_cmds(char* buffer, char* cmds[], int* cmdc);   // tokenise buffer and store in cmds

void free_array(char* array[], int start, int end);     // free dynamically allocated memory in array

void get_function(char* cmd, char* function_names[], int* func_i);  // match cmd to a string in function_names

void background_execution_check(char* cmds[], int *cmdc, int *wait); // check for & at end of input and set wait accordingly

void redirection_check(char* cmds[], int *cmdc, char* redirections[]); // check for redirection symbols and store filenames in redirections array

void new_process(char* cmds[], int cmdc, func_ptr cur_function, char* redirections[], int wait); // forks new process and executes desired command

void change_streams(char* redirections[]); // sets stdin and stdout to files stored in redirections array

void valid_cmd(char* cmd); // checks if command exists on system path

void report_error(char* msg, int severity); // prints msg to stderr and exits process if severity == 1

// internal commands

void mydir(char* cmds[], int cmdc);  // prints contents of specified directory or current directory if no argument

void myclr(char* cmds[], int cmdc); // clears the screen

void myenviron(char* cmds[], int cmdc); // prints every environment variable to screen

void myecho(char* cmds[], int cmdc); // prints arguments to stdout or newline if no argument

void myhelp(char* cmds[], int cmdc); // uses more command to display user manual

void mypause(char* cmds[], int cmdc); // pauses shell and waits for enter to be pressed

void mycd(char* cmds[], int cmdc); // changes working directory to specified directory, prints current directory if no argument 
