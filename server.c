#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  char buf[256];

  // Create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("Failed to create a socket\n");
    return 1;
  }

  // Bind the socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(12345);
  server_address.sin_addr.s_addr = INADDR_ANY;
  if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) <
      0) {
    printf("Failed to bind socket\n");
    return 1;
  }

  // Listen
  if (listen(sockfd, 5) < 0) {
    printf("Failed to listen\n");
    return 1;
  }

  // Accept
  int cfd = accept(sockfd, 0, 0);
  if (cfd < 0) {
    printf("Failed to accept connection\n");
    close(sockfd);
    return 1;
  }

  // Chat loop
  while (1) {
    int bytes_rec = recv(cfd, buf, sizeof(buf) - 1, 0);
    if (bytes_rec <= 0) {
      printf("Client disconnected\n");
      break;
    }
    buf[bytes_rec] = '\0';
    printf("Client: %s\n", buf);

    printf("Server: ");
    fgets(buf, sizeof(buf), stdin);
    send(cfd, buf, strlen(buf), 0);
  }

  close(cfd);
  close(sockfd);

  return 0;
}
