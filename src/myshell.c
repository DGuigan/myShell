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

  char* redirections[3]; 	// string array of redirections of form {in, out, out_type}

  int wait = 1;			// boolean to control background execution

  int running = 1;		// boolean to control main loop

  set_environment_variables(argv[0]);	// sets the SHELL environment variable and creates MAN_PATH, a path to the manual

  while(running) {

    // reset contents of redirections array
    for (int i = 0; i < 3; i++) {
      redirections[i] = NULL;
    }

    printf("%s %s ", getenv("PWD"), prompt); 	// prints prompt to screen

    if (fgets(buffer, BUF_SIZE, stdin)) {

      get_cmds(buffer, cmds, &cmdc);	// fill cmds array with tokenised input

      if (cmdc > 0) {		// check for command

        if (strncmp("quit", cmds[0], BUF_SIZE) == 0) {
          running = 0;
        }
        else if (strncmp("cd", cmds[0], BUF_SIZE) == 0) {
          mycd(cmds, cmdc);
        }
        else {
          get_function(cmds[0], function_names, &func_i);	// match first input to internal commands

          background_execution_check(cmds, &cmdc, &wait);	// check if background execution requested

          redirection_check(cmds, &cmdc, redirections);		// check from io redirection

          new_process(cmds, cmdc, functions[func_i], redirections, wait);	// spawns new process with desired functionality

          free_array(redirections, 0, 3);			// free contents of redirections array
        }
      }
      free_array(cmds, 0, cmdc);
    }
  }
}
