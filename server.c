#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

int createAndConfigureSocketIPV4(int port){
  int sockfd;
  struct sockaddr_in servAddr;

  // Cria o socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Criação do socket falhou!\n");
    return -1;
  }

  printf("Socket criado com sucesso!\n");

  // Configura a estrutura do endereço do servidor
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons(port);

  // Associa o socket a um endereço e porta
  if (bind(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
    printf("Bind do socket à porta e endereço falhou\n");
    return -1;
  }

  printf("Socket associado a porta e endereço com sucesso!\n");

  // Aguarda a conexão de um cliente
  if (listen(sockfd, 1) < 0) {
    printf("Listen do socket falhou!\n");
    return -1;
  }

  printf("Socket server ouvindo a porta: %d\n", port);

  return sockfd;
}

int receiveClientConnectionIPV4(int serverSock){
  int newSock;
  struct sockaddr_in cliAddr;
  socklen_t cliLen = sizeof(cliAddr);

  // Aceita a conexão do cliente
  if ((newSock = accept(serverSock, (struct sockaddr *) &cliAddr, &cliLen)) < 0) {
    printf("Accept conexão do cliente falhou!\n");
    return -1;
  }

  printf("Cliente conectado!\n");

  return newSock;
}

int createAndConfigureSocketIPV6(int port){
  int sockfd;
  struct sockaddr_in6 servAddr;

  // Cria o socket
  if ((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
    printf("Criação do socket falhou!\n");
    return -1;
  }

  printf("Socket criado com sucesso!\n");

  // Configura a estrutura do endereço do servidor
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin6_family = AF_INET6;
  servAddr.sin6_addr = in6addr_any;
  servAddr.sin6_port = htons(port);

  // Associa o socket a um endereço e porta
  if (bind(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
    printf("Bind do socket à porta e endereço falhou\n");
    return -1;
  }

  printf("Socket associado a porta e endereço com sucesso!\n");

  // Aguarda a conexão de um cliente
  if (listen(sockfd, 1) < 0) {
    printf("Listen do socket falhou!\n");
    return -1;
  }

  printf("Socket server ouvindo a porta: %d\n", port);

  return sockfd;
}

int receiveClientConnectionIPV6(int serverSock){
  int newSock;
  struct sockaddr_in6 cliAddr;
  socklen_t cliLen = sizeof(cliAddr);

  // Aceita a conexão do cliente
  if ((newSock = accept(serverSock, (struct sockaddr *) &cliAddr, &cliLen)) < 0) {
    printf("Accept conexão do cliente falhou!\n");
    return -1;
  }

  printf("Cliente conectado!\n");

  return newSock;
}

int main(int argc, char *argv[]){
  char buffer[BUFFER_SIZE]; 
  
  char *ipVersion = argv[1];
  int port = atoi(argv[2]);

  printf("Server running!\n");
  printf("Versão IP: %s\n", argv[1]);
  printf("Porta: %s\n", argv[2]);

  int sockfd, cliSock;
  
  if(strcmp(ipVersion, "v4") == 0){
    printf("Configurando conexão IPV4!\n");

    //Create sock and accept client connection
    if((sockfd = createAndConfigureSocketIPV4(port)) < 0){
      return 0;
    }
    if((cliSock = receiveClientConnectionIPV4(sockfd)) < 0){
      return 0;
    }
  }else if(strcmp(ipVersion, "v6") == 0){
    printf("Configurando conexão IPV6!\n");

    //Create sock and accept client connection
    if((sockfd = createAndConfigureSocketIPV6(port)) < 0){
      return 0;
    }
    if((cliSock = receiveClientConnectionIPV6(sockfd)) < 0){
      return 0;
    }
  }else{
    printf("Versão ip recebida inválida!\n");
  }

  int recvReturn;

  while(1){
    memset(buffer, 0, sizeof(buffer));
    recvReturn = recv(cliSock, buffer, sizeof(buffer), 0);

    if(recvReturn < 0){
      printf("Erro ao executar recv do client\n");
      return 0;
    }

    if(strcmp(buffer, "exit") == 0){
      //Fecha os sockets e printa a mensagem no servidor
      if(close(sockfd) == -1){
        printf("Erro ao fechar o socket do servidor!\n");
      }

      if(close(cliSock) == -1){
        printf("Erro ao fechar o socket do cliente!\n");
      }

      printf("connection closed\n");
      return 0;
    }
    
    printf("Buffer recebido do cliente: %s\n", buffer);
  }
}