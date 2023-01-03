#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <limits>
#include "GenericVector.h"
#include "Distances.h"

using namespace std;

/**
 * It reads the files in the given path, and returns a vector of IrisVectors
 *
 * @param argc The number of arguments passed to the program.
 * @param argv An array of strings, each string is one of the parameters you passed to the program.
 */
int main(int argc, char** argv) {
    // Check if K is valid:
    bool isKValid = validateK(argv[1]);
    if(!isKValid){
        cout<< "Error: Invalid K value" << endl;
        exit(1);
    }
    int k = atoi(argv[1]);

    // Get user path of CSV file:
    string path = argv[2];
    // Get user requested distance algorithm:
    string distanceAlgorithm = argv[3];
    // Get user input:
    string inputVector;

    int vectorSize = 0;
    // Create a DB for all the vectors in the given file:
    vector<GenericVector> vectorsDB = readFromFiles(path, &vectorSize);
    // Check if the DB is empty, if so- exit the program:
    if(vectorsDB.empty()){
        exit(1);
    }

    // Get the user input in infinity a loop:
    while (true) {
        // Get user unclassified vector:
        getline(cin, inputVector);

        // Check if the input is empty:
        if (inputVector.empty()) {
            cout << "Error: empty string" << endl;
            continue;
        }

        // Check if the input vector is valid:
        bool flag = false;
        vector<double> newVector = splitAndValidate(inputVector + " ", &flag);
        if (flag) {
            flag = false;
            cout << "Error: incorrect input" << endl;
            continue;
        }

        // Check if the user inout vector size match the vector size in DB:
        if(newVector.size() != vectorSize ){
            cout << "Error: invalid amount of values for vector" << endl;
            continue;
        }

        // Calculate the distances of each vector from the given vector
        int power = 2;
        string closestVectorType;
        try {
            closestVectorType = calculateDistances(vectorsDB, newVector, distanceAlgorithm, power);
        } catch (invalid_argument &e) {
            cout << e.what() << endl;
            continue;
        }

        // Apply KNN on the irisVectorDB to find the k closest neighborhoods:
        vectorsDB = quickSelect(vectorsDB, 0, (int) vectorsDB.size(), k);
        getKNeighborhood(vectorsDB, k, closestVectorType);
    }
}