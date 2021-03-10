/*
*  Name: David McGuigan
*  Student ID: 19425942
*
*  I hereby acknowledge the DCU Academic Integrity Policy
*/

#include "myshell.h"


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
  char path[BUF_SIZE];
  chdir(cmds[1]);		// change directory
  getcwd(path, BUF_SIZE);	// get path of current directory
  setenv("PWD", path, 1);	// overwrite previous PWD environment variable
}
