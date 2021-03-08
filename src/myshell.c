#include "myshell.h"

int main(int argc, char* argv[])
{
  char* prompt = ">";		// shell prompt

  char buffer[BUF_SIZE];	// input line buffer
  
  char* cmds[MAX_CMDS];		// string array to hold tokenised input
  int cmdc = 0;			// length of cmds array

  func_ptr functions[] = {&myclr, &myenviron, &myecho, &myhelp, &mypause, NULL};	// NULL terminated list of internal commands
  char* function_names[] = {"clr", "environ", "echo", "help", "pause", NULL};			// string array of function aliases, order must match functions array
  func_ptr cur_function;								// pointer to desired function
  int func_i = 0;

  int running = 1;		// boolean to control main loop

  while(running) {

    printf("%s %s ", getenv("PWD"), prompt); 	// prints prompt to screen

    if (fgets(buffer, BUF_SIZE, stdin)) {

      get_cmds(buffer, cmds, &cmdc);	// fill cmds array with tokenised input

      if (cmds[0] != NULL) {		// check for command

        if (strncmp("quit", cmds[0], BUF_SIZE) == 0) {
          running = 0;
        }
        else if (strncmp("cd", cmds[0], BUF_SIZE) == 0) {
          mycd(cmds, cmdc);
        }
        else {
          get_function(cmds[0], function_names, &func_i);	// match first input to internal commands

          if (functions[func_i] != NULL) {
            functions[func_i](cmds, cmdc);
          }
        }
      }
      free_array(cmds, 0, cmdc);
    }
  }
}
