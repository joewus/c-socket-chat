// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

// Define constants
#define MAX_CLIENTS 10       // Maximum number of clients that can connect
#define BUFFER_SIZE 1024     // Size of the buffer for messages

// Array to store client socket file descriptors
int client_sockets[MAX_CLIENTS];

// Mutex to synchronize access to the client_sockets array
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to broadcast a message to all connected clients except the sender
void broadcast(char *message, int sender_fd) {
    // Lock the mutex to ensure thread-safe access to client_sockets
    pthread_mutex_lock(&clients_mutex);

    // Iterate through all client sockets
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        // If the socket is valid and not the sender, send the message
        if (client_sockets[i] && client_sockets[i] != sender_fd) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }

    // Unlock the mutex after broadcasting the message
    pthread_mutex_unlock(&clients_mutex);
}

// Function to handle communication with a single client
void *handle_client(void *arg) {
    int client_fd = *(int *)arg; // Get the client socket file descriptor
    char buffer[BUFFER_SIZE];   // Buffer to store received messages

    while (1) {
        // Clear the buffer before receiving data
        memset(buffer, 0, BUFFER_SIZE);

        // Receive data from the client
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

        // If no data is received or the client disconnects, exit the loop
        if (bytes_received <= 0) {
            break;
        }

        // Print the received message to the server terminal
        printf("Message received: %s\n", buffer);

        // Broadcast the message to other clients
        broadcast(buffer, client_fd);
    }

    // Close the client socket after the client disconnects
    close(client_fd);

    // Remove the client from the client_sockets array
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (client_sockets[i] == client_fd) {
            client_sockets[i] = 0; // Mark the slot as empty
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    return NULL; // Exit the thread
}

int main() {
    int server_fd, client_fd; // File descriptors for the server and client sockets
    struct sockaddr_in server_addr, client_addr; // Structures to store server and client addresses
    socklen_t addr_size = sizeof(struct sockaddr_in); // Size of the client address structure

    // Create a socket for the server
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure the server address
    server_addr.sin_family = AF_INET;          // Use IPv4
    server_addr.sin_port = htons(5000);        // Set the port to 5000
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all available network interfaces (No Need for Manual server IP Input)

    // Bind the server socket to the specified address and port
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Start listening for incoming connections
    listen(server_fd, MAX_CLIENTS);

    printf("Server is listening on port 5000...\n");

    while (1) {
        // Accept a new client connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);

        // Add the client to the client_sockets array
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (!client_sockets[i]) { // Find an empty slot
                client_sockets[i] = client_fd;
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        // Create a new thread to handle the client
        pthread_t tid;
        pthread_create(&tid, NULL, &handle_client, &client_fd);
    }

    return 0; // Exit the program  
}      

