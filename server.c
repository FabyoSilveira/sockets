#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
  char buffer[BUFFER_SIZE]; 
  char *ipVersion = argv[1];
  int port = atoi(argv[2]);

  printf("Server running!\n");
  printf("Versão IP: %s\n", argv[1]);
  printf("Porta: %s\n", argv[2]);

  int sockfd, newSock;
  struct sockaddr_in servAddr, cliAddr;
  socklen_t cliLen = sizeof(cliAddr);

  // Cria o socket
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Criação do socket falhou!\n");
    return 0;
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
    return 0;
  }

  printf("Socket associado a porta e endereço com sucesso!\n");

  // Aguarda a conexão de um cliente
  if (listen(sockfd, 1) < 0) {
    printf("Listen do socket falhou!\n");
    return 0;
  }

  printf("Socket server ouvindo a porta: %d\n", port);

  do{
    scanf("%s", buffer);
  }while(strcmp(buffer, "exit") != 0);
  
  return 0;
}