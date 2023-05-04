#include <unistd.h> //for sleep
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
  if(argc > 1){
    sleep(atoi(argv[1])); //crash slowly with extra params
  }
  fprintf(stdout, "about to crash\n");
  fflush(stdout);
  volatile int* crasher = NULL;
  *crasher = 5;
  return 0;
}
