/*
*  Name: David McGuigan
*  Student ID: 19425942
*
*  I hereby acknowledge the DCU Academic Integrity Policy
*
*  Each function in this file is tied to a specific command line function that the user can call.
*  For general descriptions of each function see their prototypes in myshell.h
*/

#include "myshell.h"


void myrename(char* cmds[], int cmdc)
{
  char response;  // char to hold users response to overwriting file

  // check if sufficent arguments 
  if (cmdc < 3) {
    report_error("Not enough arguments", 1);
  }

  // check if file to move exists  
  if (access(cmds[1], F_OK) != 0) {
    report_error("File does not exist", 1);
  }

  // check if desination name is taken, ask to overwrite
  if (access(cmds[2], F_OK) == 0) {
    printf("The file %s already exists. Do you wish to overwrite it?[y/n]: ", cmds[2]);
    
    scanf(" %c", &response);
    if (response == 'y') {
      execlp("mv", "mv", cmds[1], cmds[2], NULL);
    }
  }
}


void mydir(char* cmds[], int cmdc)
{
  // replace process with ls, use current directory if none supplied
  execlp("ls", "ls", "-al", (cmdc == 1 ? "." : cmds[1]), NULL);
}


void myclr(char* cmds[], int cmdc)
{
  system("clear");
}


void myenviron(char* cmds[], int cmdc)
{
  for (int i = 0; environ[i] != NULL; i++) {
    printf("%s\n", environ[i]);
  }
}


void myecho(char* cmds[], int cmdc)
{
  int i;
  // print all but first and last strings in cmds array
  for (i = 1; i < cmdc - 1; i++) {
    printf("%s ", cmds[i]);
  }
  // print last string in cmds array with newline
  printf("%s\n", (cmdc == 1 ? "" : cmds[i]));
}


void myhelp(char* cmds[], int cmdc)
{
  execlp("more", "more", getenv("MAN_PATH"), NULL);
}


void mypause(char* cmds[], int cmdc)
{
  char buffer;
  printf("Shell paused...Enter to continue");
  while (scanf("%c", &buffer) && buffer != '\n');
}

void mycd(char* cmds[], int cmdc)
{
  if (cmdc == 1) {		// if no arguments just print current directory
    printf("%s\n", getenv("PWD"));
    return;
  }
  // check if directory exists by trying to access it
  DIR* dir_ptr = opendir(cmds[1]);
  if (dir_ptr) {		// if dir_ptr is not NULL the directory exists
    closedir(dir_ptr);		// close directory
    char path[BUF_SIZE];
    chdir(cmds[1]);		// change directory
    getcwd(path, BUF_SIZE);	// get path of current directory
    setenv("PWD", path, 1);	// overwrite previous PWD environment variable
  }
  else {
    report_error("invalid directory", 0);
  }
}
