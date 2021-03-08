#include "myshell.h"

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
  for (int i = 1; i < cmdc - 1; i++) {
    printf("%s ", cmds[i]);
  }
  printf("%s\n", (cmdc == 1 ? "" : cmds[cmdc - 1]));
}


void myhelp(char* cmds[], int cmdc)
{
  execlp("more", "more", "../readme.md", NULL);
}


void mypause(char* cmds[], int cmdc)
{
  char buffer;
  int enter_pressed = 0;
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
