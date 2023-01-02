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
 * This function validates a given string. If the string contains a valid double number it returns true, otherwise
 * returns false.
 * @param str input string
 * @return bool - true or false
 */
bool validate (const string& str) {
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
                bool isValid= validate((line.substr(start,i-start)));
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
                bool isValid= validate(line.substr(start,i-start));
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
 * This function gets a string and creates a numbers vector out of the string. If the input string is not valid it
 * throws an invalid argument exception.
 * @param str input string
 * @return a vector of number
 */
vector<double> splitAndValidate (const string& str, bool* flag){
    vector<string> strings = removeSpaces(str);
    vector<double> numbers;

    for (const auto& currStr : strings) {
        if (validate(currStr)) {
            numbers.push_back(stod(currStr));
        } else {
            *flag = true;
        }
    }

    return numbers;
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
string calculateDistances(vector<GenericVector> &vectorsArr, const vector<double>& newVector, const string& algorithm, int p) {
    double result, minimum = numeric_limits<double>::infinity();
    string closestVectorType;

    for (auto & vector : vectorsArr) {
        if (algorithm == "AUC") {
            result = Distances::calculateDistance(vector.getValues(), newVector, 2);
        } else if (algorithm == "MAN") {
            result = Distances::calculateDistance(vector.getValues(), newVector, 1);
        } else if (algorithm == "CHB") {
            result = Distances::chebyshevDistance(vector.getValues(), newVector);
        }  else if (algorithm == "CAN") {
            result = Distances::canberraDistance(vector.getValues(), newVector);
        }  else if (algorithm == "MIN") {
            result = Distances::calculateDistance(vector.getValues(), newVector, p);
        } else {
            throw invalid_argument("Error: Invalid distance algorithm");
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
void classifyVector(const map<string, int>& map, const string& closestVectorType) {
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
        cout << closestVectorType << endl;
    } else {
        cout << maxTypeCount << endl;
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
void getKNeighborhood(vector<GenericVector> vectorsDB, int k, const string& closestVectorType) {
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

    classifyVector(typeOfVectorsMap, closestVectorType);
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