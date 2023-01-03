#include "Client.h"
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <vector>

using namespace std;

const int MAX_SIZE_MESSAGE = 4096;
//
int ipAndPortValidation (const char *ipAddress , const int port){
    struct in_addr ipAdd{};
    int ipValid = 0 , portValid = 0;
    //check if ip and port are valid, whoever is valid, it changes it to 1
    if(inet_pton(AF_INET, ipAddress, &ipAdd) == 1){
        ipValid = 1;
    }
    if((port>=0) && (port <= 65535)){
        portValid = 1;
    }
    //check different cases for different error messages
    if ((ipValid == 0) && (portValid == 0)){
        cout <<"Ip and port are not valid"<< endl;
        return 0;
    }
    if (ipValid == 0){
        cout<<"IP is not valid"<<endl;
        return 0;
    }
    if (portValid == 0){
        cout<<"Port is not valid"<<endl;
        return 0;
    }
    return 1;
}


int stringValidation (string input){
    double doubleNum;
    string str;
    int num = -1;
    vector<double> vector;

    //go over the vector values
    stringstream ss(input);
    while (ss >> doubleNum ){
        vector.push_back(doubleNum);
    }
    if (vector.empty()){
        cout<< "invalid input"<< endl;
        return 0;
    }

    ss.clear();
    if (ss >> str){
        ss << str;
    }

    if(str.empty()){
        cout<< "invalid input"<< endl;
        return 0;
    }

    ss.clear();
    if(ss >> num){
        ss << num;
    }

    if(num < 0){
        cout<< "invalid input"<< endl;
        return 0;
    }

    //checks if the end of the stream has been reached
    if(!ss.eof()){
        return 0;
    }
    return 1;
}

int main(int argc, char** argv) {
    // Get ip address for the client:
    const char *ipAddress = argv[1];
    // Get a port for the client:
    const int clientPort = atoi(argv[2]);

    //ip and port validation
    int areIPAndPortValid = ipAndPortValidation(ipAddress, clientPort);
    if (areIPAndPortValid == 0){
        return 0;
    }

    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error creating client socket");
        cerr << "Error creating client socket" << endl;
        return 0;
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
        close(clientSocket);
        return 0;
    }

    // For user input:
    string input;

    // Send input from a user to the server:
    while (true) {
        char buffer[MAX_SIZE_MESSAGE];
        bool flag = true;

        // Get user input:
        getline(cin, input);
        //checks if the user input is -1
        if(input == "-1"){
            break;
        }

        int inputValid = stringValidation(input);
        if (inputValid == 0){

        }
        //convert

        // Check if the input is empty:
        if (input.empty()) {
            cout << "Error: empty string" << endl;
            continue;
        }

        //cleans the buffer
        memset(&buffer, 0, sizeof(buffer));
        //takes the input and change it to array of chars and adds /0 to the end of the string
        char inputArray[input.size() +1];
        strncpy(inputArray, input.c_str(), input.size());
        inputArray[input.size()] = '\0';

        int dataLength = strlen(inputArray);
        int sentBytes = (int) send(clientSocket, inputArray, dataLength, 0);
        if (sentBytes < 0) {
            perror("Error: message was not sent to the server");
            cerr << "Error: message was not sent to the server" << endl;
            break;
        }

        // Receive a response from the server
        int exceptedDataLen = sizeof(buffer);
        // TODO: is casting necessary?
        int bytesReceived = (int) recv(clientSocket, buffer, exceptedDataLen, 0);
        if (bytesReceived < 0) {
            perror("Error receiving data from server");
            cerr << "Error receiving data from server" << endl;
            break;
        } else if (bytesReceived == 0) {
            cout << "connection is closed" << endl;
            break;
        } else {
            cout << "Server response: " << string(buffer, bytesReceived) << endl;
        }
    }

    // Close the socket
    close(clientSocket);
    return 0;
}
