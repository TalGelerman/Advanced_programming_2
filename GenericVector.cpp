#include <utility>
#include "GenericVector.h"

double GenericVector::getDistance() const {
    return distance;
}

vector<double> GenericVector::getValues() {
    return values;
}

string GenericVector::getType() {
    return type;
}

void GenericVector::setType(string t) {
    type = t;
}

void GenericVector::setValues(vector<double> valsVector) {
    values = valsVector;
}

void GenericVector::setDistance(double dist) {
    distance = dist;
}
