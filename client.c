#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024
#define FILE_BUFFER 495

char validFiles[6][6] = {".txt", ".c", ".cpp", ".py", ".tex", ".java"};

int createAndConnectSockToServerIPV4orIPV6(char* ip, int port){
  int sock;
  struct sockaddr_in servAddrV4;
  struct sockaddr_in6 servAddrV6;

  //IPV4
  if(inet_pton(AF_INET, ip, &servAddrV4.sin_addr) == 1){

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

    if (connect(sock, (struct sockaddr *)&servAddrV4, sizeof(servAddrV4)) < 0) {
      printf("Connection Failed\n");
      return -1;
    }

    printf("Conectado com sucesso ao server %s porta %d\n", ip, port);

  //IPV6
  }else if(inet_pton(AF_INET6, ip, &servAddrV6.sin6_addr) == 1){

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

    if (connect(sock, (struct sockaddr *)&servAddrV6, sizeof(servAddrV6)) < 0) {
      printf("Connection Failed\n");
      return -1;
    }

    printf("Conectado com sucesso ao server %s porta %d\n", ip, port);
  }

  return sock;
}

int sendMessage(int cliSock, FILE *file, char *fileName){
  char fileBuffer[FILE_BUFFER+1];
  char *finalMessage = (char*)malloc(strlen(fileName) * sizeof(char));
  size_t bytesRead;

  //Clear memory of fileBuffer to avoid missreading
  memset(fileBuffer, 0, sizeof(fileBuffer));

  while ((bytesRead = fread(fileBuffer, 1, FILE_BUFFER, file)) > 0) {
    //Build the message format to be sent to server   
    memcpy(finalMessage, fileName, strlen(fileName));
    strcat(finalMessage, " ");
    strcat(finalMessage, fileBuffer);
    
    if (send(cliSock, finalMessage, (int)strlen(finalMessage), 0) == -1) {
      printf("Failed to send data\n");
      return 0;
    }
  }

  //Reset file pointer to the beggining, to allow just resent the file as in TP description
  //without having to select it again
  fseek(file, 0, SEEK_SET);

  //Free dynamic allocated memory for the finalMessage
  free(finalMessage);
  
  return 1;
}

void closeSocket(int sock){
  if(close(sock) == -1){
    printf("Erro ao fechar o socket\n");
  }
}

int main(int argc, char *argv[]){
  char buffer[BUFFER_SIZE];
  char serverResponseBuffer[BUFFER_SIZE];
  char *ip = argv[1];
  int port = atoi(argv[2]);

  FILE *file = NULL; 
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
    //Clear string \n
    buffer[strcspn(buffer, "\n")] = '\0'; 

    if(strstr(buffer, "select file") != NULL){
      //reset pointer to null to avoid reference bugs
      fileName = NULL;

      char *fileExtension = strrchr(buffer, '.');
      fileName = strrchr(buffer, ' ');
      bool validFile = false;
      
      //Do nothing if a file is not given as parameter
      if(fileName != NULL && fileExtension != NULL){
        //Remove white space
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
        file = NULL;
        //Tenta selecionar o arquivo
        file = fopen(fileName, "rb");
        if(file == NULL){
          printf("%s do not exist\n", fileName);
          selectedFile = false;
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

      int sendMessageFlag;
      //send file 
      if((sendMessageFlag = sendMessage(sock, file, fileName)) == 0){
        return 0;
      }

      //Receive response from server
      memset(serverResponseBuffer, 0, sizeof(serverResponseBuffer));
      recv(sock, serverResponseBuffer, sizeof(serverResponseBuffer), 0);

      printf("%s\n", serverResponseBuffer);
    }else if(strcmp(buffer, "exit") == 0){
           
      if (send(sock, buffer, sizeof(buffer), 0) == -1){
        printf("Send message failed\n");
        return 0;
      }

      //Receive response from server
      memset(serverResponseBuffer, 0, sizeof(serverResponseBuffer));
      recv(sock, serverResponseBuffer, sizeof(serverResponseBuffer), 0);

      printf("%s\n", serverResponseBuffer);

      closeSocket(sock);
      break;

    }else{ 
      closeSocket(sock);
      break;
    } 
  }
  return 0;
}