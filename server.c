#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

void reverseMessage(char* message) {
    int length = strlen(message);
    int i, j;
    char temp;

    for (i = 0, j = length - 1; i < j; ++i, --j) {
        temp = message[i];
        message[i] = message[j];
        message[j] = temp;
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Server configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind socket to address and port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    printf("Server started. Waiting for client messages...\n");

    while (1) {
        client_addr_len = sizeof(client_addr);

        // Receive data from client
        ssize_t received_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (received_bytes < 0) {
            perror("Error receiving data from client");
            exit(1);
        }

        buffer[received_bytes] = '\0';

        // Reverse the received message
        reverseMessage(buffer);

        // Print client's IP address
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        printf("Received message from %s: %s\n", client_ip, buffer);
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, client_addr_len) < 0) {
            perror("Error sending data to client");
            exit(1);
        }
    }

    close(sockfd);

    return 0;
}
