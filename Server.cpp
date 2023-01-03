#include "Server.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <limits>
#include "GenericVector.h"
#include "Distances.h"

using namespace std;

/* Constants */
const int MAX_SIZE_MESSAGE = 4096;
const int AUC_POWER = 2;
const int MAN_POWER = 1;
const int POWER = 2;

/**
 * This function validates a given string. If the string contains a valid double number it returns true, otherwise
 * returns false.
 * @param str input string
 * @return bool - true or false
 */
bool validateDouble (const string& str) {
    bool dotSeen = false;
    //going over all the characters in the string
    for (int i = 0 ; i < str.length() ; i++) {
        //invalid char
        if(!isdigit(str[i])) {
            //a different char from the special cases
            if (str[i] != '-' && str[i] != '.') {
                return false;
            } else {
                //the char is minus, and it is not on the first char
                if ((str[i] == '-') && (i != 0)) {
                    return false;
                }
                //the char is not the first dot in the number
                if ((str[i] == '.') && (dotSeen)) {
                    return false;
                }
                if ((str[i] == '.') && (!dotSeen)){
                    dotSeen = true;
                }
            }
        }
    }

    return true;
}

/**
 * It reads the data from the file and stores it in a vector of GenericVector objects
 *
 * @param path the path to the file containing the vectors
 * @param vectorSize The size of the vectors in the database.
 *
 * @return A vector of GenericVector objects.
 */
vector<GenericVector> readFromFiles(const string& path, int* vectorSize) {
    vector<GenericVector> vectorsDB;
    ifstream inputFile;
    inputFile.open(path);

    string line, type;
    int amount = 0;

    //read the first line and count the amount of values
    if(inputFile.good()){
        GenericVector firstVector;
        vector<double> newVectorValues;
        //get the first line
        getline(inputFile, line);
        int start = 0;
        for (int i = 0; i < line.size(); i++) {
            //a place where we want to cut
            if (line.at(i) == ',') {
                newVectorValues.push_back(stod(line.substr(start, i-start)));
                bool isValid = validateDouble((line.substr(start, i - start)));
                start = i + 1;
                //checks the input validation
                if(!isValid){
                    cout << "Error: invalid file" << endl;
                    vector<GenericVector> emptyVector;
                    return emptyVector;
                }
                //count the number of values in vector
                amount++;
            } else if (i == line.size() - 1) {
                firstVector.setType(line.substr(start, i-start+1));
            }
        }
    }

    while (inputFile.good()) {
        GenericVector newVector;

        vector<double> newVectorValues;
        //get second line
        getline(inputFile, line);
        //counter - counts number of values, start - set as the end
        int counter = 0, start = 0;
        //i - the index of char
        for (int i = 0; i < line.size(); i++) {
            //a place where we want to cut
            if (line.at(i) == ',') {
                newVectorValues.push_back(stod(line.substr(start, i-start)));
                bool isValid= validateDouble(line.substr(start, i - start));
                if(!isValid){
                    cout << "Error: invalid file" << endl;
                    vector<GenericVector> emptyVector;
                    return emptyVector;
                }
                start = i + 1;
                //count the number of values in vector
                counter++;
            } else if (i == line.size() - 1) {
                newVector.setType(line.substr(start, i-start+1));
            }
        }
        if (counter != amount) {
            cout << "Error: invalid file" << endl;
            vector<GenericVector> emptyVector;
            return emptyVector;
        }

        *vectorSize = counter;
        newVector.setValues(newVectorValues);
        vectorsDB.push_back(newVector);
    }

    inputFile.close();
    return vectorsDB;
}

/**
 * This function creates a stings vector out from a given sting. It inserts every sub-sting in the given string into
 * the stings vector, and returns it.
 * @param str input string
 * @return strings vector
 */
vector<string> removeSpaces (const string& str) {
    vector<string> strings;
    int counter = 1, j = 0;
    for (int i = 0; i < str.length() - 1; i++) {
        if (j >= str.length()){
            break;
        }

        for (j = i + 1; j < str.length(); j++) {
            if (str[i] == ' ') {
                i++;
                continue;
            }
            if (str[j] == ' ') {
                strings.push_back(str.substr(i, counter));
                i = j + 1;
                j = i;
                counter = 1;
            } else {
                counter++;
            }
        }
    }
    return strings;
}

/**
 * It returns true if the string k is a non-zero integer, and false otherwise
 *
 * @param k the number of clusters to create
 *
 * @return a boolean value.
 */
bool validateK(const string& k) {
    if (k == "0") {
        return false;
    }
    return all_of(k.begin(), k.end(), ::isdigit);
}

/**
 * It returns true if the string is one of the five valid distances, and false otherwise
 *
 * @param str the string to be validated
 *
 * @return A boolean value.
 */
bool validateDistance(const string& str) {
    if (str == "AUC" || str == "MAN" || str == "CHB" || str == "CAN" || str == "MIN") {
        return true;
    } else return false;
}

/**
 * It splits the user input string into a vector of strings, then validates each string and returns a vector of doubles
 *
 * @param str The string that the user input.
 * @param distanceAlgo The distance algorithm to use.
 * @param k the number of nearest neighbors to find
 * @param flag a boolean that indicates if the user input is valid or not.
 * @param vectorSize The size of the vector in the DB.
 *
 * @return A vector of doubles.
 */
vector<double> splitAndValidate (const char* str, string* distanceAlgo, int* k, bool* flag, int vectorSize) {
    vector<string> strings = removeSpaces(str);
    vector<double> numbers;

    for (const auto& currStr : strings) {
        if (validateDouble(currStr)) {
            numbers.push_back(stod(currStr));
        } else if (validateDistance(currStr)) {
            *distanceAlgo = currStr;
        } else if (validateK(currStr)) {
            *k = stoi(currStr);
        } else {
            *flag = true;
        }
    }
    // Check if the user input vector size match the vector size in DB:
    if(numbers.size() != vectorSize ){
        *flag = true;
    }
    return numbers;
}

/**
 * Swap the contents of two GenericVector objects.
 *
 * @param vector1 A pointer to the first vector to swap.
 * @param vector2 The vector to swap with.
 */
void swap(GenericVector* vector1, GenericVector* vector2) {
    GenericVector temp = *vector1;
    *vector1 = *vector2;
    *vector2 = temp;
}

/**
 * It takes the last element as pivot, places the pivot element at its correct position in sorted array, and places all
 * smaller (smaller than pivot) to left of pivot and all greater elements to right of pivot
 *
 * @param arr The array to be sorted
 * @param low The lowest index of the array
 * @param high the last index of the array
 *
 * @return The index of the pivot
 */
int partition (vector<GenericVector> &arr, int low, int high) {
    double pivot = arr[high].getDistance();    // pivot
    int i = low - 1; // Index of smaller element

    for (int j = low; j <= high- 1; j++) {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j].getDistance() <= pivot) {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high - 1]);
    return i+1;
}

/**
 * If the pivot is in the right place, return the array. If the pivot is not in the right place, recursively call
 * quickSelect on the left or right side of the pivot
 *
 * @param arr the array to be sorted
 * @param low the lowest index of the array
 * @param high the last index of the array
 * @param k the kth smallest element
 *
 * @return The kth smallest element in the array.
 */
vector<GenericVector> quickSelect(vector<GenericVector> &arr, int low, int high, int k) {
    if (low >= high) {
        return arr;
    }
    // pivot is partitioning index, arr[p] is now at right place
    int pivot = partition(arr, low, high);

    if (k < pivot) {
        quickSelect(arr, low, pivot - 1, k);
    } else if (k > pivot) {
        quickSelect(arr, pivot + 1, high, k);
    }
    return arr;
}

/**
 * It calculates the distance between the new vector and all the vectors in the training set, and returns the type of the
 * closest vector
 *
 * @param vectorsArr The array of vectors that we will be comparing the new vector to.
 * @param newVector The vector we want to classify
 * @param algorithm The distance algorithm to use.
 * @param p The p value for the Minkowski distance.
 *
 * @return The type of the closest vector.
 */
string calculateDistances(vector<GenericVector> &vectorsArr, const vector<double>& newVector, const string& algorithm) {
    double result, minimum = numeric_limits<double>::infinity();
    string closestVectorType;

    for (auto & vector : vectorsArr) {
        if (algorithm == "AUC") {
            result = Distances::calculateDistance(vector.getValues(), newVector, AUC_POWER);
        } else if (algorithm == "MAN") {
            result = Distances::calculateDistance(vector.getValues(), newVector, MAN_POWER);
        } else if (algorithm == "CHB") {
            result = Distances::chebyshevDistance(vector.getValues(), newVector);
        }  else if (algorithm == "CAN") {
            result = Distances::canberraDistance(vector.getValues(), newVector);
        }  else if (algorithm == "MIN") {
            result = Distances::calculateDistance(vector.getValues(), newVector, POWER);
        }

        vector.setDistance(result);
        if (result < minimum) {
            minimum = result;
            closestVectorType = vector.getType();
        }
    }
    return closestVectorType;
}

/**
 * Find the most count of a value, and if there is more than one maxCount, print the closestVectorType
 *
 * @param map A map of the types and their counts.
 * @param closestVectorType The type of the closest vector to the vector we want to classify.
 */
string classifyVector(const map<string, int>& map, string& closestVectorType) {
    string maxTypeCount;
    int maxCount = 0;
    bool flag = false;
    // Find the most count of a value:
    for (auto const& element : map) {
        if (element.second > maxCount) {
            maxTypeCount = element.first;
            maxCount = element.second;
        }
    }
    // Find if there is more than one maxCount:
    for (auto const& element : map) {
        if (element.second == maxCount) {
            flag = true;
        }
    }
    // Print the result type:
    if(flag) {
        return closestVectorType;
    } else {
        return maxTypeCount;
    }
}

/**
 * It takes a vector of vectors, an integer k, and a string closestVectorType. It then creates a map of strings to
 * integers, and iterates through the first k vectors in the vector of vectors. For each vector, it gets the type of the
 * vector, and if the type is already in the map, it increments the count of that type by 1. If the type is not in the map,
 * it adds the type to the map with a count of 1. After iterating through the first k vectors, it calls the classifyVector
 * function, passing in the map and the closestVectorType
 *
 * @param vectorsDB The database of vectors that we have.
 * @param k The number of neighbors to consider.
 * @param closestVectorType The type of the closest vector.
 */
string getKNeighborhood(vector<GenericVector> vectorsDB, int k, string& closestVectorType) {
    map<string, int> typeOfVectorsMap;

    for (int i = 0; i < k; i++) {
        string type = vectorsDB.at(i).getType();

        if (typeOfVectorsMap.find(type) != typeOfVectorsMap.end()) {
            int count = typeOfVectorsMap.at(type);
            typeOfVectorsMap[type] = count + 1;
        } else {
            typeOfVectorsMap[type] = 1;
        }
    }

    return classifyVector(typeOfVectorsMap, closestVectorType);
}

int main(int argc, char** argv) {
    // Get the file path for the server:
    const char* filePath = argv[1];
    // Get the port for the server:
    const int serverPort = stoi(argv[2]);
    // todo: Do i need to validateDouble the port and the path?

    // Create a TCP socket:
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating server socket");
        return -1;  // todo: not exit(1)
    }

    // Bind the socket to a port
    sockaddr_in addr{};   // struct for address
    addr.sin_family = AF_INET;  // reset the struct
    addr.sin_port = htons(serverPort);    // define the port
    addr.sin_addr.s_addr = INADDR_ANY;    // address protocol type
    // bind the socket with the bind command and check if the binding is done:
    if (bind(serverSocket, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("Error binding socket to port");
        return -1;   // todo: not exit(1)
    }

    int vectorSize = 0;
    // Create a DB for all the vectors in the given file:
    vector<GenericVector> vectorsDB = readFromFiles(filePath, &vectorSize);
    // Check if the DB is empty, if so- exit the program:
    if(vectorsDB.empty()) {
        perror("Error: Vector file is empty");
        return -1; // todo: not exit(1)
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {  // todo: listen to up to 5 clients at a time.
        cerr << "Error listening for incoming connections" << endl;
        return -1;   // todo: should we return 1?
    }

    // Accept an incoming connection
    struct sockaddr_in client_addr{};  // create address struct for the sender information.
    unsigned int client_addr_size = sizeof(client_addr);
    // Accept command to create a new socket for the client that wants to connect the server:
    int clientSocket = accept(serverSocket, (struct sockaddr*)&client_addr, &client_addr_size);
    if (clientSocket < 0) {
        perror("Error accepting incoming connection");
        return -1;  // todo: should we return 1?
    }

    char buffer[MAX_SIZE_MESSAGE];      // create a buffer for the client of 4096 bytes.
    int dataLength = sizeof(buffer);    // the maximum length of data to receive from the client socket.
    int readBytes = (int) recv(clientSocket, buffer, dataLength, 0);  // receive the client message from its socket to buffer.

    if (readBytes == 0) {
        // connection is closed
        perror("Error: No message received from the client");
        return -1;  // todo: should we return 1?
    } else if (readBytes < 0) {
        // error:
        perror("Error: Couldn't read message from the client");
        return -1;  // todo: should we return 1?
    } else {
        // print the buffer (message from the user):
        cout << buffer << endl;
        string distanceAlgorithm;   // Declare user requested distance algorithm.
        int k;  // Declare K.
        // Check if the given vector is valid:
        bool flag = false;
        vector<double> newVector = splitAndValidate(buffer, &distanceAlgorithm, &k, &flag, vectorSize);
        if (flag) {
            flag = false;
            perror("Error: incorrect input");
            return -1; // todo: what to return?
        }
        // Calculate the distances of each vector from the given vector:
        string closestVectorType = calculateDistances(vectorsDB, newVector, distanceAlgorithm);
        // Apply KNN on the VectorDB to find the k closest neighborhoods:
        vectorsDB = quickSelect(vectorsDB, 0, (int) vectorsDB.size(), k);
        string vectorClass = getKNeighborhood(vectorsDB, k, closestVectorType);
        // Send the vector classification back to the client:
        const char* message = vectorClass.c_str();
        int sentBytes = (int) send(clientSocket, buffer, strlen(message), 0);
        if (sentBytes < 0) {
            perror("Error sending message to the client");
            return -1;  // todo: what to return?
        }
    }

    // Close the socket
    close(serverSocket);
    return 0;
}
