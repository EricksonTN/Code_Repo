#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  int firstchild = fork();

  if(firstchild == 0) {
    
    // Do work here to fork to create the target pstree diagram.

    fork();
    if (fork() != 0)
      fork();
      

    // Put all calls to fork() above this line, and don't modify the code below.

    sleep(2); /* required so that execve runs before the process quits (don't do this sort of thing in real code) */
    return 0;
  }
  
  else {
    sleep(1); /* ensure child finishes forking before parent calls execve */


    // The parent process calls execve on pstree.

    char pid[20]; // location to put the pid argument to the pstree execution
    char* args[] = {"/usr/bin/pstree", "-p" , pid, NULL};
    char* environ[] = {NULL};

    sprintf(pid, "%u", firstchild);
    execve( args[0], args, environ);
  }
}
