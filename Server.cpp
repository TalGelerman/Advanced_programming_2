#include "Server.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

const int MAX_SIZE_MESSAGE = 4096;

int main() {
    // Initialize a port for the server:
    const int serverPort = 5555;

    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating server socket");
        cerr << "Error creating server socket" << endl;
        return 1;
    }

    // Bind the socket to a port
    sockaddr_in addr{};   // struct for address
    addr.sin_family = AF_INET;  // reset the struct
    addr.sin_port = htons(8080);    // define the port
    addr.sin_addr.s_addr = INADDR_ANY;  // address protocol type
    // bind the socket with the bind command and check if the binding is done:
    if (bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Error binding socket to port");
        cerr << "Error binding socket to port" << endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {  // listen to up to 5 clients ar a time.
        cerr << "Error listening for incoming connections" << endl;
        return 1;
    }

    // Accept an incoming connection
    sockaddr_in client_addr{};  // create address struct for the sender information.
    socklen_t client_addr_size = sizeof(client_addr);
    // Accept command to create a new socket for the client that wants to connect the server:
    int clientSocket = accept(serverSocket, (sockaddr*)&client_addr, &client_addr_size);
    if (clientSocket < 0) {
        perror("Error accepting incoming connection");
        cerr << "Error accepting incoming connection" << endl;
        return 1;
    }

    char buffer[MAX_SIZE_MESSAGE];  // create a buffer for the client of 4096 bytes.
    int dataLength = sizeof(buffer);    // the maximum length of data to receive from the client socket.
    int readBytes = (int) recv(clientSocket, buffer, dataLength, 0);  // receive the client message from its socket to buffer.
    if (readBytes == 0) {
        // connection is closed
        perror("Error: No message received from the client");
        cerr << "Error: No message received from the client" << endl;
        return 1;
    } else if (readBytes < 0) {
        // error:
        perror("Error: Couldn't read message from the client");
        cerr << "Error: Couldn't read message from the client" << endl;
        return 1;
    } else {
        // print the buffer (message from the user):
        cout << buffer;
        int sentBytes = (int) send(clientSocket, buffer, readBytes, 0);
        if (sentBytes < 0) {
            perror("Error sending message to the client");
            cerr << "Error sending message to the client" << endl;
            return 1;
//        }
    }

    // Send a message to the client
    const char* message = "Hello, client!\n";
    send(clientSocket, message, strlen(message), 0);

    // Close the socket
    close(serverSocket);
    return 0;
}

