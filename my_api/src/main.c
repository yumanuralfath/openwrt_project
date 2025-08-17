/* Fixed HTTP Server for OpenWrt - removes SO_REUSEPORT */
/* Place in: package/my_package/my_api/src/main.c */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 2007
#define BUFFER_SIZE 1024

static int server_fd = -1;

const char *http_response = "HTTP/1.1 200 OK\r\n"
                            "Content-Type: application/json\r\n"
                            "Access-Control-Allow-Origin: *\r\n"
                            "Connection: close\r\n"
                            "Content-Length: 50\r\n"
                            "\r\n"
                            "{\"message\": \"Hello from OPENWRT MR3020 V3\"}";

void signal_handler(int sig) {
  (void)sig;
  printf("\nShutting down server...\n");
  if (server_fd >= 0) {
    close(server_fd);
  }
  exit(0);
}

int main() {
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  printf("Starting API server...\n");

  // Create socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Only use SO_REUSEADDR (SO_REUSEPORT not supported on older kernels)
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt SO_REUSEADDR failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Set up address structure
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind socket
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    fprintf(stderr, "Error code: %d\n", errno);
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Listen for connections
  if (listen(server_fd, 5) < 0) {
    perror("listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("API Server successfully started on port %d\n", PORT);
  printf("Server ready to accept connections...\n");

  while (1) {
    int new_socket;

    printf("Waiting for connection...\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
      perror("accept failed");
      continue;
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr),
           ntohs(address.sin_port));

    // Read request
    ssize_t bytes_read = read(new_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read > 0) {
      buffer[bytes_read] = '\0';
      printf("Received request: %.100s...\n", buffer);

      // Send response
      ssize_t bytes_sent =
          send(new_socket, http_response, strlen(http_response), 0);
      if (bytes_sent < 0) {
        perror("send failed");
      } else {
        printf("Response sent (%zd bytes)\n", bytes_sent);
      }
    }

    // Close connection
    close(new_socket);
    memset(buffer, 0, BUFFER_SIZE);

    printf("Connection closed\n\n");
  }

  return 0;
}
