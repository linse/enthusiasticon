#include <stdio.h>  // printf
#include <string.h> // strlen
#include <stdlib.h> // exit
#include <unistd.h> // syscalls

#define MAXLINE 1024
// maxargs used in struct, can't be variable so we have to use define
#define MAXARGS 128

char prompt[] = "linsh> "; // command line prompt

struct command {
  int argc;              // number of args
  char *argv[MAXARGS];   // arguments list
  enum builtin_t {       // is argv[0] a builtin command?
    NONE, QUIT, JOBS, BG, FG } builtin;
};

enum builtin_t parseBuiltin(struct command *cmd) {
    if (!strcmp(cmd->argv[0], "quit")) { // quit command
        return QUIT;
    } else if (!strcmp(cmd->argv[0], "jobs")) { // jobs command
        return JOBS;
    } else if (!strcmp(cmd->argv[0], "bg")) { // bg command
        return BG;
    } else if (!strcmp(cmd->argv[0], "fg")) { // fg command
        return FG;
    } else {
        return NONE;
    }
}

void error(char *msg) {
  printf("Error: %s", msg);
  exit(0);
}

int parse(const char *cmdline, struct command *cmd) {
    static char array[MAXLINE];          // local copy of command line
    const char delims[10] = " \t\r\n";   // argument delimiters
    char *line = array;                   // ptr that traverses command line
    char *token;                          // ptr to the end of the current arg
    char *endline;                        // ptr to the end of the cmdline string
    int is_bg;                           // background job?

    if (cmdline == NULL) 
        error("command line is NULL\n");

    (void) strncpy(line, cmdline, MAXLINE);
    endline = line + strlen(line);

    // build argv list
    cmd->argc = 0;

    while (line < endline) {
        // skip delimiters
        line += strspn (line, delims);
        if (line >= endline) break;

        // Find token delimiter
        token = line + strcspn (line, delims);

        // terminate the token
        *token = '\0';

        // Record token as the token argument
        cmd->argv[cmd->argc++] = line;

        // Check if argv is full
        if (cmd->argc >= MAXARGS-1) break;

        line = token + 1;
    }

    // argument list must end with a NULL pointer
    cmd->argv[cmd->argc] = NULL;

    if (cmd->argc == 0)  // ignore blank line
        return 1;

    cmd->builtin = parseBuiltin(cmd);

    // should job run in background?
    if ((is_bg = (*cmd->argv[cmd->argc-1] == '&')) != 0)
        cmd->argv[--cmd->argc] = NULL;

    return is_bg;
}

void runSystemCommand(struct command *cmd, int bg) {
    pid_t childPid;
    // FORK !!!
    if ((childPid = fork()) < 0)
        error("fork() error");
    else if (childPid == 0) { // I'm the child and could run a command
        // EXECVP !!!!
        if (execvp(cmd->argv[0], cmd->argv) < 0) {
            printf("%s: Command not found\n", cmd->argv[0]);
            exit(0);
        }
    }
    else { // I'm the parent. Shell continues here.
       if (bg) 
          printf("Child in background [%d]\n",childPid);
       else
          wait(&childPid);
    }
}


void runBuiltinCommand(struct command *cmd, int bg) {
    switch (cmd->builtin) {
        case QUIT:
            printf("TODO: quit\n"); break;
        case JOBS:
            printf("TODO: jobs\n"); break;
        case BG:
            printf("TODO: background\n"); break;
        case FG:
            printf("TODO: foreground\n"); break;
        default:
            error("Unknown builtin command");
    }
}


// We changed this
void eval(char *cmdline) {
  int bg;
  struct command cmd;

  printf("Evaluating '%s'\n", cmdline);
  // parse line into command struct
  bg = parse(cmdline, &cmd); 

  // parse error
  if (bg == -1) return;
  // empty line - ignore
  if (cmd.argv[0] == NULL) return;

  if (cmd.builtin == NONE) 
    runSystemCommand(&cmd, bg);
  else 
    runBuiltinCommand(&cmd, bg);
}

// We already know this ..
int main(int argc, char **argv) {
  char cmdline[MAXLINE]; // buffer for fgets
  char c;

  while (1) {

    printf("%s", prompt);

    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
      error("fgets error");

    if (feof(stdin)) { 
      printf("\n");
      exit(0);
    }
    
    // Remove trailing newline
    cmdline[strlen(cmdline)-1] = '\0';
    
    // Evaluate command line
    eval(cmdline);
  }
}
