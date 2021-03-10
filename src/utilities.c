/*
*  Name: David McGuigan
*  Student ID: 19425942
*
*  I hereby acknowledge the DCU Academic Integrity Policy
*/

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


void redirection_check(char* cmds[], int *cmdc, char* redirections[])
{
  char* in = "<";	// symbol for input redirection
  char* out = ">";	// symbol for output redirection with truncate
  char* outA = ">>";	// symbol for output redirection with append

  // check cmds for redirection symbols and records files in redirections array
  for (int i = 0; i < *cmdc - 1; i++) {

    if (strncmp(in, cmds[i], BUF_SIZE) == 0) {
      redirections[0] = (char*) malloc(sizeof(char) * (strlen(cmds[i + 1]) + 1));
      strcpy(redirections[0], cmds[i + 1]);
    }
    else if (strncmp(out, cmds[i], BUF_SIZE) == 0) {
      redirections[1] = (char*) malloc(sizeof(char) * (strlen(cmds[i + 1]) + 1));
      redirections[2] = (char*) malloc(sizeof(char) * 2);
      strcpy(redirections[1], cmds[i + 1]);
      strcpy(redirections[2], "w");
    }
    else if (strncmp(outA, cmds[i], BUF_SIZE) == 0) {
      redirections[1] = (char*) malloc(sizeof(char) * (strlen(cmds[i + 1]) + 1));
      redirections[2] = (char*) malloc(sizeof(char) * 2);
      strcpy(redirections[1], cmds[i + 1]);
      strcpy(redirections[2], "a");
    }
  }
 
  // remove unnecessary items from cmds array
  for (int i = 0; i < 2; i++) {
    if (redirections[i] != NULL) {
      free_array(cmds, *cmdc - 2, *cmdc);
      *cmdc -= 2;
    }
  }
}


void new_process(char* cmds[], int cmdc, func_ptr cur_function, char* redirections[], int wait)
{
  pid_t pid;
  int status;

  switch (pid = fork()) {	// fork new process
    case -1:			//stop if error
      report_error("failed to fork", 0);
      return;
    case 0:
      setenv("PARENT", getenv("SHELL"), 1);  // creates required evironment variable for child process
      change_streams(redirections);
      if (cur_function == NULL) {	// if not internal function then exec
        valid_cmd(cmds[0]);		// check if command is valid
        cmds[cmdc] = NULL;		// append NULL to cmds for use with exec function
        execvp(cmds[0], cmds);
      }
      cur_function(cmds, cmdc);
      exit(0);
    default:				// parent process either waits for child or continues
      if (wait) {
        waitpid(pid, &status, WUNTRACED);
      }
  }
}


void change_streams(char* redirections[])
{
  if (redirections[0]) {			// check for input redirection
    if (access(redirections[0], R_OK) == 0) {	// check if file exists and can be read
      freopen(redirections[0], "r", stdin);
    }
    else if (access(redirections[0], F_OK) == 0) { // check if file exists
      report_error("Denied read permissions for input file", 1);
    }
    else {
      report_error("Input file does no exist", 1);
    }
  }

  if (redirections[1]) {			// check for output redirection
    if (access(redirections[1], F_OK) == 0 && access(redirections[1], W_OK) != 0) { // check if file exists and cannot be written to
      report_error("Denied write permissions for output file", 1);
    }
    else {
      freopen(redirections[1], redirections[2], stdout);
    }
  }
}

void valid_cmd(char* cmd)
{
  char paths_copy[BUF_SIZE];;
  strncpy(paths_copy, getenv("PATH"), BUF_SIZE);  // copy PATH variable so it isn't destroyed by strtok
  char path[BUF_SIZE];
  char* path_start = strtok(paths_copy, ":");

  while (path_start != NULL) {		// check for command on system paths
    sprintf(path, "%s/%s", path_start, cmd);
    if (access(path, X_OK) == 0) {
      return;
    }
    path_start = strtok(NULL, ":");
  }
  
  if (access(cmd, X_OK) == 0) {		// check for command locally
    return;
  }
  
  report_error("invalid command", 1);	// if command not found/executable report error
}


void report_error(char* msg, int severity)
{
  fprintf(stderr, "Error: %s\n", msg);	// print error to stderr

  if (severity) {	// kill process if necessary
    exit(1);
  }
}
