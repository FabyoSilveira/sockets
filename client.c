#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

char validFiles[6][6] = {".txt", ".c", ".cpp", ".py", ".tex", ".java"};

int main(int argc, char *argv[]){
  char buffer[BUFFER_SIZE];
  char *ip = argv[1];
  int port = atoi(argv[2]);

  FILE *file; 
  bool selectedFile = false;

  printf("Client running!\n");
  printf("Endereço IP: %s\n", argv[1]);
  printf("Porta de conexão: %s\n", argv[2]);

  int sock;
  struct sockaddr_in servAddr;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Criação do socket falhou!\n");
    return 0;
  }
  
  memset(&servAddr, '0', sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip, &servAddr.sin_addr) <= 0) {
    printf("Endereço não suportado\n");
    return 0;
  }

  printf("Endereço válido mapeado para servAddr\n");

  if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    printf("Connection Failed\n");
    return 0;
  }

  printf("Conectado com sucesso ao server %s porta %d\n", ip, port);

  do{
    fgets(buffer, sizeof(buffer), stdin);
    //Limpa o '\n' da string
    buffer[strcspn(buffer, "\n")] = '\0'; 

    if(strstr(buffer, "select file") != NULL){
      char *fileExtension = strrchr(buffer, '.');
      char *fileName = strrchr(buffer, ' ');
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
    }else if(strcmp(buffer, "send file") == 0){
      
      if(!selectedFile){
        printf("no file selected!\n");
        continue;
      }

      printf("Send file implementation!\n");
      //send file     
    }

  }while(strcmp(buffer, "exit") != 0);
  
  return 0;
}