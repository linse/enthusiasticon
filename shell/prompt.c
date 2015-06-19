#include <stdio.h>  // printf
#include <string.h> // strlen
#include <stdlib.h> // exit
#include <unistd.h> // syscalls

int MAXLINE = 1024;
char prompt[] = "lnsh> "; // command line prompt

void eval(char *cmdline) {
  printf("Future me will evaluate '%s'\n", cmdline);
}

void error(char *msg) {
  printf("Error: %s", msg);
  exit(0);
}

int main(int argc, char **argv) {
  char cmdline[MAXLINE]; // buffer for fgets

  while (1) {

    printf("%s", prompt);

    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
      error("fgets error");

    if (feof(stdin)) { 
      printf("\n");
      exit(0);
    }
    
    // remove trailing newline
    cmdline[strlen(cmdline)-1] = '\0';
    
    // evaluate command line
    eval(cmdline);
  }
}
