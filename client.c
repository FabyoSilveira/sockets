#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

char validFiles[6][6] = {".txt", ".c", ".cpp", ".py", ".tex", ".java"};

int createAndConnectSockToServerIPV4orIPV6(char* ip, int port){
  int sock;
  struct sockaddr_in servAddrV4;
  struct sockaddr_in6 servAddrV6;

  //IPV4
  if(inet_pton(AF_INET, ip, &servAddrV4.sin_addr) == 1){
    printf("Versão IP: IPV4\n");

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("Criação do socket falhou!\n");
      return -1;
    }
    
    memset(&servAddrV4, '0', sizeof(servAddrV4));
    servAddrV4.sin_family = AF_INET;
    servAddrV4.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &servAddrV4.sin_addr) <= 0) {
      printf("Endereço v4 não suportado\n");
      return -1;
    }

    printf("Endereço válido mapeado para servAddr4\n");

    if (connect(sock, (struct sockaddr *)&servAddrV4, sizeof(servAddrV4)) < 0) {
      printf("Connection Failed\n");
      return -1;
    }

    printf("Conectado com sucesso ao server %s porta %d\n", ip, port);

  //IPV6
  }else if(inet_pton(AF_INET6, ip, &servAddrV6.sin6_addr) == 1){
    printf("Versão IP: IPV6\n");

    if ((sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
      printf("Criação do socket falhou!\n");
      return -1;
    }
    
    memset(&servAddrV6, '0', sizeof(servAddrV6));
    servAddrV6.sin6_family = AF_INET6;
    servAddrV6.sin6_port = htons(port);

    if (inet_pton(AF_INET6, ip, &servAddrV6.sin6_addr) <= 0) {
      printf("Endereço v6 não suportado\n");
      return -1;
    }

    printf("Endereço válido mapeado para servAddr6\n");

    if (connect(sock, (struct sockaddr *)&servAddrV6, sizeof(servAddrV6)) < 0) {
      printf("Connection Failed\n");
      return -1;
    }

    printf("Conectado com sucesso ao server %s porta %d\n", ip, port);
  }

  return sock;
}

int main(int argc, char *argv[]){
  char buffer[BUFFER_SIZE];
  char *ip = argv[1];
  int port = atoi(argv[2]);

  FILE *file; 
  bool selectedFile = false;
  char *fileName;

  printf("Client running!\n");
  printf("Endereço IP: %s\n", argv[1]);
  printf("Porta de conexão: %s\n", argv[2]);

  int sock; 
  struct sockaddr_in servAddrV4;

  //Create and connect sock to server
  if((sock = createAndConnectSockToServerIPV4orIPV6(ip, port)) < 0){
    return 0;
  }

  while(1){
    fgets(buffer, sizeof(buffer), stdin);
    //Limpa o '\n' da string
    buffer[strcspn(buffer, "\n")] = '\0'; 

    if(strstr(buffer, "select file") != NULL){
      char *fileExtension = strrchr(buffer, '.');
      fileName = strrchr(buffer, ' ');
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

      printf("Send message file: %s\n", fileName);
      //send file  
      if (send(sock, fileName, sizeof(fileName), 0) == -1){
        printf("Send message failed\n");
        return 0;
      }   

      printf("Send succesfully!\n");
    }else if(strcmp(buffer, "exit") == 0){
           
      if (send(sock, buffer, sizeof(buffer), 0) == -1){
        printf("Send message failed\n");
        return 0;
      } 

      if(close(sock) == -1){
        printf("Erro ao fechar o socket\n");
      }

      break;
    }

  } 

  return 0;
}