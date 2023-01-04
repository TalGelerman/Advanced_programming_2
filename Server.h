/**
 * @author Ido Ziv, 318420254
 * @author Tal Gelerman, 322280850
 * @version ass3
 * @since 04.01.23
 */

#ifndef MAIN_CPP_SERVER_H
#define MAIN_CPP_SERVER_H
#include "GenericVector.h"
#include <map>

using namespace std;

bool validateDouble (const string& str);
vector<GenericVector> readFromFiles(const string& path, int* vectorSize);
vector<string> removeSpaces (const string& str);
bool validateK(const string& k);
bool validateDistance(const string& str);
vector<double> splitAndValidate (const char* str, string* distanceAlgo, int* k, bool* flag, int vectorSize);
void swap(GenericVector* vector1, GenericVector* vector2);
int partition (vector<GenericVector> &arr, int low, int high);
vector<GenericVector> quickSelect(vector<GenericVector> &arr, int low, int high, int k);
string calculateDistances(vector<GenericVector> &vectorsArr, const vector<double>& newVector, const string& algorithm);
string classifyVector(const map<string, int>& map, string& closestVectorType);
string getKNeighborhood(vector<GenericVector> vectorsDB, int k, string& closestVectorType);
bool validatePort(int port);

#endif //MAIN_CPP_SERVER_H
