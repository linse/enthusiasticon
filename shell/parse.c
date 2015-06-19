#include <stdio.h>  // printf
#include <string.h> // strlen
#include <stdlib.h> // exit
#include <unistd.h> // syscalls

#define MAXLINE 1024
// maxargs used in struct, can't be variable so we have to use define
#define MAXARGS 128

char prompt[] = "lnsh> "; // command line prompt

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

int parse(const char *cmdline, struct command *cmd) {
    static char array[MAXLINE];          // local copy of command line
    const char delims[10] = " \t\r\n";   // argument delimiters
    char *buf = array;                   // ptr that traverses command line
    char *next;                          // ptr to the end of the current arg
    char *endbuf;                        // ptr to the end of the cmdline string
    int is_bg;                           // background job?

    if (cmdline == NULL) {
        (void) fprintf(stderr, "Error: command line is NULL\n");
        return -1;
    }

    (void) strncpy(buf, cmdline, MAXLINE);
    endbuf = buf + strlen(buf);

    // build argv list
    cmd->argc = 0;

    while (buf < endbuf) {
        // skip delimiters
        buf += strspn (buf, delims);
        if (buf >= endbuf) break;

        // Find next delimiter
        next = buf + strcspn (buf, delims);

        // terminate the token
        *next = '\0';

        // Record token as the next argument
        cmd->argv[cmd->argc++] = buf;

        // Check if argv is full
        if (cmd->argc >= MAXARGS-1) break;

        buf = next + 1;
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

void eval(char *cmdline) {
  int bg;
  struct command cmd;

  printf("Evaluating '%s'\n", cmdline);

  // parse cmdline into cmd structure
  bg = parse(cmdline, &cmd); 
  printf("Found command %s\n", cmd.argv[0]);
  printf("Builtin %d\n", cmd.builtin);

  // -1 means parse error
  if (bg == -1) return;
}

void error(char *msg) {
  printf("Error: %s", msg);
  exit(0);
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
