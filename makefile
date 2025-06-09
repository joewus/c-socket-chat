# Compiler
CC = gcc

# Compiler flags
CFLAGS = -pthread

# Source files
CLIENT_SRC = client.c
SERVER_SRC = server.c

# Output executables
CLIENT_OUT = client
SERVER_OUT = server

# Default target
all: $(CLIENT_OUT) $(SERVER_OUT)

# Compile the client
$(CLIENT_OUT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_OUT) $(CLIENT_SRC)

# Compile the server
$(SERVER_OUT): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_OUT) $(SERVER_SRC)

# Clean up generated files
clean:
	rm -f $(CLIENT_OUT) $(SERVER_OUT)