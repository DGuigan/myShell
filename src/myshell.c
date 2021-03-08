#include "myshell.h"

int main(int argc, char* argv[])
{
  char* prompt = "==>";		// shell prompt

  char buffer[BUF_SIZE];	// input line buffer
  
  char* cmds[MAX_CMDS];		// string array to hold tokenised input
  int cmdc = 0;			// length of cmds array

  int running = 1;		// boolean to control main loop

  while(running) {

    printf("%s ", prompt); 	// prints prompt to screen

    if (fgets(buffer, BUF_SIZE, stdin)) {

      get_cmds(buffer, cmds, &cmdc);

      for (int i = 0; i < cmdc; i++){
        printf("cmds %d: %s\n", i, cmds[i]);
      }
    }
  }
}
