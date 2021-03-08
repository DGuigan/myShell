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

