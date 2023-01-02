#include "Client.h"
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

using namespace std;

const int MAX_SIZE_MESSAGE = 4096;

int main(int argc, char** argv) {
    // Get ip address for the client:
    const char* ipAddress = argv[1];
    // Get a port for the client:
    const int clientPort = atoi(argv[2]);

    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error creating client socket");
        cerr << "Error creating client socket" << endl;
        return 1;
    }

    // Connect to the server
    struct sockaddr_in addr{};
    memset(&addr, 0, sizeof(addr));     // reset the struct
    addr.sin_family = AF_INET;
    addr.sin_port = htons(clientPort);
    addr.sin_addr.s_addr = inet_addr(ipAddress);
    if (connect(clientSocket, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("Error connecting to server");
        cerr << "Error connecting to server" << endl;
        return 1;
    }

    // For user input:
    string input;

    // Send input from a user to the server:
    while (true) {
        // Get user input:
        getline(cin, input);
        // Check if the input is empty:
        if (input.empty()) {
            cout << "Error: empty string" << endl;
            continue;
        }

        int dataLength = (int) input.size();
        int sentBytes = (int) send(clientSocket, input.c_str(), dataLength, 0);
        if (sentBytes < 0) {
            perror("Error: message was not sent to the server");
            cerr << "Error: message was not sent to the server" << endl;
            return 1;
        }

        // Receive a response from the server
        char buffer[MAX_SIZE_MESSAGE];
        int exceptedDataLen = sizeof(buffer);
        // TODO: is casting necessary?
        int bytesReceived = (int) recv(clientSocket, buffer, exceptedDataLen, 0);
        if (bytesReceived < 0) {
            perror("Error receiving data from server");
            cerr << "Error receiving data from server" << endl;
            // todo: send -1 to the server, so it will close the client socket there.
            break;
        } else if (bytesReceived == 0) {
            cout << "Server response: something went wrong" << endl;
        } else {
            cout << "Server response: " << string(buffer, bytesReceived) << endl;
        }
    }

    // Close the socket
    close(clientSocket);
    return 0;
}
