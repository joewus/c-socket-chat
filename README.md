# 💬 C Terminal Chat Application

This is a simple terminal chat app written in C using TCP sockets.

## Features

- Multiple clients can connect to a server (group-chat)
- Real-time messaging in the terminal
- Uses threads for handling multiple users

## How to Use

1. Compile the files using `make`:
   - Run `make all` to compile both the server and client.
   - Run `make clean` to remove the compiled executables.

2. Run the server:
   ```bash
   ./server
   ```
3. Open multiple terminals and run the client

## To Do

- Add private messaging
- Improve error handling
