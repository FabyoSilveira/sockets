#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
  char command[30]; 

  printf("Client running!\n");
  printf("Endereço IP: %s\n", argv[1]);
  printf("Porta de conexão: %s\n", argv[2]);

  do{
    scanf("%s", command);
  }while(strcmp(command, "exit") != 0);
  
  return 0;
}