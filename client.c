#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COMMAND 256

char validFiles[6][6] = {".txt", ".c", ".cpp", ".py", ".tex", ".java"};

int main(int argc, char *argv[]){
  char command[30];
  FILE *file; 
  bool selectedFile = false;

  printf("Client running!\n");
  printf("Endereço IP: %s\n", argv[1]);
  printf("Porta de conexão: %s\n", argv[2]);

  do{
    fgets(command, MAX_COMMAND, stdin);
    //Limpa o '\n' da string
    command[strcspn(command, "\n")] = '\0'; 

    if(strstr(command, "select file") != NULL){
      char *fileExtension = strrchr(command, '.');
      char *fileName = strrchr(command, ' ');
      bool validFile = false;

      //Não faz nada caso o comando não possua um arquivo como parâmetro
      if(fileName != NULL && fileExtension != NULL){
        //retira o espaço em branco do nome do arquivo
        fileName++;
        
        //Check if file extension is valid
        for(int i=0; i < 6; i++){
          if(strcmp(validFiles[i], fileExtension) == 0){
            validFile = true;
          }
        }

        if(!validFile){
          printf("%s not valid!\n", fileName);
          continue;
        }

        //Tenta selecionar o arquivo
        file = fopen(fileName, "rb");
        if(file == NULL){
          printf("%s do not exist\n", fileName);
          continue;
        }


        printf("%s selected!\n", fileName);
        selectedFile = true;
      }
    }else if(strcmp(command, "send file") == 0){
      
      if(!selectedFile){
        printf("no file selected!\n");
        continue;
      }

      printf("Send file implementation!\n");
      //send file     
    }

  }while(strcmp(command, "exit") != 0);
  
  return 0;
}