#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 12345
#define BUF_SIZE 256

int main() {
  char buf[BUF_SIZE];

  // Create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("Failed to create a socket\n");
    return 1;
  }

  // Connect to server socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);

  // Set server ip address
  if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr.s_addr) <= 0) {
    printf("Failed to set server ip address");
    return 1;
  }

  if (connect(sockfd, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    printf("Failed to connect to server\n");
    return 1;
  }

  // Chat loop
  while (1) {
    printf("Client: ");
    fgets(buf, sizeof(buf), stdin);
    if (strncmp(buf, "exit", 4) == 0) {
      printf("Exiting...\n");
      break;
    }
    send(sockfd, buf, strlen(buf), 0);

    int bytes_rec = recv(sockfd, buf, sizeof(buf) - 1, 0);
    if (bytes_rec <= 0) {
      printf("Server disconnected\n");
      break;
    }
    // Null-terminate the recieved message
    buf[bytes_rec] = '\0';
    printf("Server: %s", buf);
  }

  close(sockfd);

  return 0;
}
