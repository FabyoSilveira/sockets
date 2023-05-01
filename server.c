#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
  char command[30]; 

  printf("Server running!\n");
  printf("Versão IP: %s\n", argv[1]);
  printf("Porta: %s\n", argv[2]);

  do{
    scanf("%s", command);
  }while(strcmp(command, "exit") != 0);
  
  return 0;
}