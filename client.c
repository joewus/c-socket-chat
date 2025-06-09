// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

// Define the size of the buffer used for messages
#define BUFFER_SIZE 1024

// Global variable for the socket file descriptor
int sockfd;

// Function to handle receiving messages from the server
void *receive_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        // Clear the buffer before receiving data
        memset(buffer, 0, BUFFER_SIZE);

        // Receive data from the server
        int bytes = recv(sockfd, buffer, BUFFER_SIZE, 0);

        // If no data is received or the connection is closed, exit
        if (bytes <= 0) {
            printf("Disconnected from server.\n");
            exit(1);
        }

        // Print the received message to the terminal
        printf("%s", buffer);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr; // Structure to store server address
    char message[BUFFER_SIZE];      // Buffer for sending messages
    char nickname[32];              // Buffer for storing the user's nickname

    // Ask the user to enter their nickname
    printf("Enter your nickname: ");
    fgets(nickname, 32, stdin); // Read the nickname from the terminal
    nickname[strcspn(nickname, "\n")] = 0; // Remove the newline character

    // Create a socket for communication
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set up the server address
    server_addr.sin_family = AF_INET;              // Use IPv4
    server_addr.sin_port = htons(5000);            // Set the port number
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set the server IP address

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed"); // Print an error if the connection fails
        return 1; // Exit the program
    }

    // Create a thread to handle receiving messages
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_handler, NULL);

    // Main loop for sending messages
    while (1) {
        // Read a message from the terminal
        fgets(message, BUFFER_SIZE, stdin);

        // Combine the nickname and message into one string
        char full_message[BUFFER_SIZE + 32];
        snprintf(full_message, sizeof(full_message), "%s: %s", nickname, message);

        // Send the message to the server
        send(sockfd, full_message, strlen(full_message), 0);
    }

    return 0; // End the program
}
