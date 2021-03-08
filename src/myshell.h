#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 1024		// maximum length of each line of input
#define MAX_CMDS 64		// maximun number of tokens taken from input 
#define DELIM " \n\t"		// token deliminators

extern char** environ;

typedef void (*func_ptr)(char*[], int);

void set_environment_variables(char* path);

void get_cmds(char* buffer, char* cmds[], int* cmdc);

void free_array(char* array[], int start, int end);

void get_function(char* cmd, char* function_names[], int* func_i);

void background_execution_check(char* cmds[], int *cmdc, int *wait);

void redirection_check(char* cmds[], int *cmdc, char* redirections[]);

void new_process(char* cmds[], int cmdc, func_ptr cur_function, char* redirections[], int wait);

void change_streams(char* redirections[]);

// internal commands
void myclr(char* cmds[], int cmdc);

void myenviron(char* cmds[], int cmdc);

void myecho(char* cmds[], int cmdc);

void myhelp(char* cmds[], int cmdc);

void mypause(char* cmds[], int cmdc);

void mycd(char* cmds[], int cmdc);

