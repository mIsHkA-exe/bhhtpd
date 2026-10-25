#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int opt = 0;
  char* logFile = NULL;
  int port = 26216;  // default port
  int debugMode = 0;
  char* configFile = NULL;

  // options handle code
  while ((opt = getopt(argc, argv, "l:dhc:p:")) != -1) {
    switch (opt) {
      case 'd':
        debugMode = 1;
        break;
      case 'p':
        port = strtol(optarg, NULL, 10);  // Secure conversion
        break;
      case 'l':
        logFile = optarg;
        break;
      case 'h':
        printf(
            "Avaiable command : \n -l : log file \n -p : input port \n -d : "
            "debug mode \n -h : help command \n -c : config file\n");
        exit(0);  // Only options that does'nt launch the socket server excep
                  // the case where the options doesn't exist
      case 'c':
        configFile = optarg;
        break;
      default:
        printf(
            "Invalid command : please check avaiable commands with -h "
            "options\n");
        exit(0);
    }
  }

  // socket creation
  int socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd == -1) {
    printf("Unable to create the socket!\n");
    exit(1);
  }

  // Bind socket to network config

  // Network config for IPV4
  struct sockaddr_in socketaddr;
  memset(&socketaddr, 0,
         sizeof(socketaddr));  // filling the 16 bytes with 0 for clean memory
  socketaddr.sin_family = AF_INET;
  socketaddr.sin_port = htons(port);               // s = 16 bits
  socketaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // l = 32 bits

  // bind socket + network config
  int bindState =
      bind(socketFd, (struct sockaddr*)&socketaddr, sizeof(struct sockaddr_in));
  if (bindState == -1) {
    printf(
        "Unable de bind the socket with the network config ! please check the "
        "state of configs in the sockaddr_in !\n");
    exit(1);
  }

  // Switch the socket instance to listen modea
  int listenState = listen(socketFd, 10);
  if (listenState == -1) {
    printf("Unable to switch the socket to listen mode\n");
    exit(1);
  }
  printf("Server is listening !\n");

  // Connection with the client socket
  struct sockaddr_in clientaddr;
  int clientFd;
  socklen_t clientaddr_size = sizeof(clientaddr);

  // Create a client socket to established the communication and keep the main
  // socket in listen mode
  while (1) {
    clientFd =
        accept(socketFd, (struct sockaddr*)&clientaddr, &clientaddr_size);
    if (clientFd == -1) {
      printf("Failed to initiate the communcation with the client !\n");
      continue;
    }
    printf("Connection established !\n");  // test connection with client

    // close(clientFd); Use to test the connection with client
    char request[1024];  // request to store the client dumbass request
    int requestByte = recv(clientFd, request, sizeof(request)-1, 0);
    if (requestByte == -1) {
      printf("Bad request ! \n");
      continue ;
    }
    request[requestByte] = '\0';
    printf("Request : %s\n Request-size : %d\n", request, requestByte);
    int valid = strncmp(request, "GET ", 4);
    if (valid == 0) {
      char response[1024] = "HTTP/1.1 200 OK\r\n\r\nHello from my C server!\n";
      int responseByte = send(clientFd, response, strlen(response), 0);
      if (responseByte == -1) {
        printf("response failed !\n");
        continue ;
      }
      printf("Response : %s\n Size : %d\n", response, responseByte);
      close(clientFd);
    } else {
      printf("Invalid requests\n");
      close(clientFd);
    }
  }
}
