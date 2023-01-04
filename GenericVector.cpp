#include "GenericVector.h"
/**
 * @author Ido Ziv, 318420254
 * @author Tal Gelerman, 322280850
 * @version ass3
 * @since 04.01.23
 */

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
