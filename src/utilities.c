#include "myshell.h"

void get_cmds(char* buffer, char* cmds[], int* cmdc)
{
  *cmdc = 0;		// reset command count to zero

  char* tmp;		// temporary pointer to hold next token
  tmp = strtok(buffer, DELIM);

  // dynamically allocate tokens into cmds array until array is full or no more tokens
  while (*cmdc < MAX_CMDS && tmp != NULL) {
    cmds[*cmdc] = (char*) malloc(sizeof(char) * (strlen(tmp) + 1));
    strcpy(cmds[*cmdc], tmp);
    tmp = strtok(NULL, DELIM);
    (*cmdc)++;
  }

  // add NULL pointer to cmds array for ease of use with exec function
  cmds[*cmdc] = (char*) malloc(sizeof(NULL));
  cmds[*cmdc] = NULL;
}

void free_array(char* array[], int start, int end)
{
  for (int i = start; i < end; i++) {
    free(array[i]);
  }
}

void get_function(char* cmd, char* function_names[], int* func_i)
{
  *func_i = 0;

  while (function_names[*func_i] != NULL && strncmp(cmd, function_names[*func_i], BUF_SIZE)) {
    (*func_i)++;
  }
}
