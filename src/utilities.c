#include "myshell.h"


void set_environment_variables(char* path)
{
  char bin_path[BUF_SIZE];
  char man_path[BUF_SIZE];

  realpath(path, bin_path);		// get the full path of the myshell execuatable
  setenv("SHELL", bin_path, 1);		// overwrites the previous SHELL environment variable

  bin_path[strlen(bin_path) - 12] = '\0';		// remove "/bin/myshell" from path
  sprintf(man_path, "%s/manual/readme", bin_path);	// append the remaining path to the manual
  setenv("MAN_PATH", man_path, 1);			// create new environment variable with full path to manual
}


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


void background_execution_check(char* cmds[], int *cmdc, int *wait)
{
  if (strcmp("&", cmds[*cmdc - 1]) == 0) {
    *wait = 0;
    free_array(cmds, *cmdc - 1, *cmdc);
    (*cmdc)--;
  }
}


void new_process(char* cmds[], int cmdc, func_ptr cur_function, int wait)
{
  pid_t pid;
  int status;

  switch (pid = fork()) {
    case -1:
      printf("failed fork\n");
      return;
    case 0:
      if (cur_function == NULL) {
        cmds[cmdc] = NULL;	// append NULL to cmds for use with exec function
        execvp(cmds[0], cmds);
      }
      cur_function(cmds, cmdc);
      exit(0);
    default:
      if (wait) {
        waitpid(pid, &status, WUNTRACED);
      }
  }
}
