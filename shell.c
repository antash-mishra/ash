#include "vector.h"
#include <_stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_pid_t.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
  // if passed null, forces getlien to allocate memory automatically
  char *buffer = NULL;

  size_t bufsize = 0;
  ssize_t characters;

  char *pathBuffer = NULL;
  size_t pathBufsize = 0;

  while (true) {

    // get current working directory
    char *gcwd = getcwd(pathBuffer, pathBufsize);
    printf("ash:%s>", gcwd);

    characters = getline(&buffer, &bufsize, stdin);
    if (characters == -1) {
      free(buffer);
      printf("Error reading input or EOF reached");
      return 1;
    } else {
      // in getline end of the line has \n so
      // if we write a command it ends with \n and
      // it fails so to fix that we replace it with \0
      // for ex: ls -la\n -> ls -la\0
      if (characters > 0 && buffer[characters - 1] == '\n') {
        buffer[characters - 1] = '\0';
      }
    }

    Vector args;
    vector_init(&args);

    // Tokenize the buffer so that we can change it into arguments
    char *token = strtok(buffer, " ");
    while (token != NULL) {
      // printf("Token: %s\n", token);
      vector_push(&args, token);

      // we do this so that tokenization starts in the
      // same string but next word in string
      token = strtok(NULL, " ");
    }
    vector_push(&args, NULL);

    for (size_t i = 0; i < args.size; i++) {
      printf("Tokens[%zu]: %s\n", i, args.data[i]);
    }

    // exit the program
    if (strcmp(args.data[0], "exit") == 0) {
      break;
    }
    else {
      // creates duplicate process
      int status;
      pid_t pid = fork();

      // inside a child process
      if (pid == 0) {
        execvp(args.data[0], args.data);
      }

      pid_t child_pid = waitpid(pid, &status, 0);
    }
  }

  free(buffer);
  return 0;
}
