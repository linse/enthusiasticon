#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  pid_t childPid;
  printf("Before fork %d\n", getpid());

  // fork creates a child and returns it's process ID
  childPid = fork();

//  if (childPid == 0) 
//    printf("I’m child, my result of fork is %d\n", childPid);
//  else { 
//    //wait(&childPid);
//    printf("I’m parent (of child with id %d)\n", childPid);
//  }

  printf("After fork %d\n", getpid());
  exit(0);
}
