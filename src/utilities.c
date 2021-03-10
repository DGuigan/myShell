/*
*  Name: David McGuigan
*  Student ID: 19425942
*
*  I hereby acknowledge the DCU Academic Integrity Policy
*/

#include "myshell.h"


void set_environment_variables(char* path)
{
  char bin_path[BUF_SIZE];// string to hold full path to myshell executable
  char man_path[BUF_SIZE];// string to hold full path to manual

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
  *func_i = 0;// reset function index to 0

  // loop through array of function names until cmd matches a name or end of array
  while (function_names[*func_i] != NULL && strncmp(cmd, function_names[*func_i], BUF_SIZE)) {
    (*func_i)++;
  }
}


void background_execution_check(char* cmds[], int *cmdc, int *wait)
{
  if (strcmp("&", cmds[*cmdc - 1]) == 0) {// check if final cmd is &
    *wait = 0;// set wait variable to 0
    // remove unneeded item from cmds array
    free_array(cmds, *cmdc - 1, *cmdc);
    (*cmdc)--;
  }
}


void redirection_check(char* cmds[], int *cmdc, char* redirections[])
{
  char* in = "<";	// symbol for input redirection
  char* out = ">";	// symbol for output redirection with truncate
  char* outA = ">>";	// symbol for output redirection with append

  // check cmds for redirection symbols and record files in redirections array
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
  pid_t pid;// ID of child process
  int status;// status of child process

  switch (pid = fork()) {	// fork new process and record process ID
    case -1:		 //stop if error
      report_error("failed to fork", 0);
      return;
    case 0:
      setenv("PARENT", getenv("SHELL"), 1);  // creates required evironment variable for child process
      change_streams(redirections);// change io streams
      if (cur_function == NULL) {	// if not internal function then exec
        valid_cmd(cmds[0]);		// check if command is valid
        cmds[cmdc] = NULL;		// append NULL to cmds for use with exec function
        execvp(cmds[0], cmds);
      }
      cur_function(cmds, cmdc); // if internal command then call function
      exit(0);// exit since above function will return unlike exec
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
    else { // file either exists and can be written to or does not and will be created
      freopen(redirections[1], redirections[2], stdout);
    }
  }
}

void valid_cmd(char* cmd)
{
  // copy PATH variable so it isn't destroyed by strtok
  char paths_copy[BUF_SIZE];
  strncpy(paths_copy, getenv("PATH"), BUF_SIZE);

  char path[BUF_SIZE]; // string to hold next path to be checked
  char* path_start = strtok(paths_copy, ":"); // string to hold beginning of path

  // check for command on system paths
  while (path_start != NULL) {
    sprintf(path, "%s/%s", path_start, cmd);// append desired command to path and store in path
    if (access(path, X_OK) == 0) {// return if command exists and is exectuable
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
