#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char const *argv[]) {
  printf("Hi there\n");

  // executing a ls command
  char* argv2[] = {"ls", "-la", NULL};
  execvp(argv2[0], argv2);

  printf(".. and done!");
}
