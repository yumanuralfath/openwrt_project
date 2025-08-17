/* Simple HTTP server using standard sockets - no external dependencies */
/* Place in: package/my_package/my_api/src/main.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 2007
#define BUFFER_SIZE 1024

static int server_fd = -1;

const char* http_response = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: application/json\r\n"
    "Access-Control-Allow-Origin: *\r\n"
    "Connection: close\r\n"
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
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("API Server running on port %d\n", PORT);
    printf("Press Ctrl+C to stop the server...\n");
    
    while (1) {
        int new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }
        
        // Read request (we don't actually process it, just respond)
        read(new_socket, buffer, BUFFER_SIZE);
        
        // Send response
        send(new_socket, http_response, strlen(http_response), 0);
        
        // Close connection
        close(new_socket);
    }
    
    return 0;
}