#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>


#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIM " \t\r\n\a"

struct cmd {
  char **command;
  struct cmd *next;
};

struct cmd *parser (char **args) 
{
  struct cmd *begCmd = malloc(sizeof(struct cmd));
  struct cmd *prevCmd;
  int i = 0;
  int startofcmd = 0;
  int endofcmd = 0;
  int numPipes = 0;
  int currentInd = 0;

  for (i; args[i] != NULL; i++)
  {
    // Adds to the current command
    if (*args[i] == '|' || args[i + 1] == NULL)
    {
      numPipes++;
      int lenCmd;
      if (args[i + 1] == NULL)
      {
        lenCmd = (i - startofcmd) + 2;
      }
      else{
        lenCmd = (i - startofcmd) + 1;
      }
      
      char **currentCmd = malloc(sizeof(char **) * lenCmd);
      struct cmd *currStruct = malloc(sizeof(struct cmd));
      int tempInd = 0;

      if (args[i + 1] == NULL)
      {
        for (startofcmd; startofcmd < (i + 1); startofcmd++)
        {
          currentCmd[tempInd] = args[startofcmd];
          tempInd++;
        }
      }

      else {
        for (startofcmd; startofcmd < i; startofcmd++)
        {
          currentCmd[tempInd] = args[startofcmd];
          tempInd++; 
        }
      }
      

      currentCmd[lenCmd - 1] = NULL;
      char **parsedCmd;
      int redirCheck = 0;
      int fd;
      for (int k = 0; currentCmd[k] != NULL; k++)
      {
        if (*currentCmd[k] == '<' || *currentCmd[k] == '>')
        {
          redirCheck = 1;
          if (*currentCmd[k] == '<')
          {
            close(0);
            fd = open(currentCmd[k + 1], O_RDONLY, 0644);
          }
          else if (*currentCmd[k] == '>')
          {
            close(1);
            fd = open(currentCmd[k + 1], O_WRONLY | O_CREAT, 0644);
          }
          parsedCmd = malloc(sizeof(char **) * (k + 1));
          for (int l = 0; l < k; l++)
          {
            parsedCmd[l] = currentCmd[l];
          }
          parsedCmd[(k + 1)] = NULL;
          break;
        }
      }

      if (numPipes == 1)
      {
        if (redirCheck == 1)
        {
          begCmd->command = parsedCmd;
        }
        else{
          begCmd->command = currentCmd;
        }
 
        prevCmd = begCmd;
      }
      else {
        if (redirCheck == 1)
        {
          currStruct->command = parsedCmd;
        }
        else {
          currStruct->command = currentCmd;
        }
        prevCmd->next = currStruct;
        

        prevCmd = currStruct;
      }
      startofcmd = i + 1;
    }
  }

  return begCmd;
}

int sh_launch(char **args)
{
  int status, fd;
  int i = 0;
  int argLen = 0;
  int designator = 0;
  //printf((sizeof(args) / sizeof(char)) + "");
  for (i; args[i] != NULL; i++)
  {
    argLen = i;
    //printf(args[i]);
    //printf("\n");
    
    if (*args[i] == '>')
    {
      printf("in the writeout check\n");
      designator = 1;
    }

    else if (*args[i] == '<')
    {
      designator = 2;
    }

    else if (*args[i] == '|')
    {
      
    }
  }
  //printf(args[0]);
  if (designator == 1)
  {
    //close(1);
    //fd = open(args[argLen - 1], O_WRONLY | O_CREAT, 0644);
  }
  //printf("%d\n", fd);

  //printf("%d\n", argLen);
  pid_t pid = fork();
  if (pid == 0)
  {
    //printf("in child\n");
    if (designator == 1)
    {
      close(1);
      fd = open(args[argLen], O_WRONLY | O_CREAT, 0644);
    }

    else if (designator == 2)
    {
      close(0);
      fd = open(args[argLen], O_RDONLY, 0644);
    }
    if (strcmp(args[0], "cd") == 0)
    {
      printf("In cd loop\n");
      chdir(args[1]);
    }
    else {
      execvp(args[0], args);
    }
    
  }
  else {
    pid_t pid2;
    while (pid2 = waitpid(-1, &status, WUNTRACED) > 0)
    {
    }
  }  
  return 1;
}

int sh_pipe(struct cmd *commands)
{
  if (commands->next == NULL)
  {
    execvp(commands->command[0], commands->command);
  }
  int p[2];
  pipe(p);
  int pid;

  pid = fork();

  if (pid != 0)     // Parent
  {
    close(1);
    dup(p[1]);
    close(p[0]);
    close(p[1]);
    execvp(commands->command[0], commands->command);
  }

  else{             // Child
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    sh_pipe(commands->next);
  }

}

int sh_execute(char **args)
{
  int isPipe = 0;
  int isList = 0;
  if (args[0] == NULL) {
    return 1;  // An empty command was entered.
  }
  for (int i = 0; args[i] != NULL; i++)
  {
    if (*args[i] == '|')
    {
      isPipe = 1;
      break;
    }
  }

  if (isPipe == 1)
  {
    struct cmd *retStrct = parser(args);
    sh_pipe(retStrct);
  }

  else{
    sh_launch(args);   // launch
  }
  
}

char **sh_split_line(char *line)
{
  int bufsize = SH_TOK_BUFSIZE;
  int position = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "sh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, SH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += SH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        free(tokens_backup);
        fprintf(stderr, "sh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, SH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

char *sh_read_line(void)
{
  char *line = NULL;
  size_t bufsize = 0;  // have getline allocate a buffer for us

  if (getline(&line, &bufsize, stdin) == -1) {
    if (feof(stdin))  // EOF
    {
      fprintf(stderr, "EOF\n");
      exit(EXIT_SUCCESS);
    } else {
      fprintf(stderr, "Value of errno: %d\n", errno);
      exit(EXIT_FAILURE);
    }
  }
  return line;
}

void sh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("143A$ ");
    line = sh_read_line();
    args = sh_split_line(line);
    status = sh_execute(args);

    free(line);
    free(args);
  } while (status);
}

int main(int argc, char **argv)
{
  sh_loop();
  return EXIT_SUCCESS;
}






