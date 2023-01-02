/**
 * @author Ido Ziv, 318420254
 * @author Tal Gelerman, 322280850
 * @version ass1
 * @since 22.11.22
 */

#include <vector>
#include <cmath>
#include "Distances.h"

using namespace std;

/**
 * a function that calculates vectors distance according to
 * euclidean distance, taxicab geometry and minkowski distance.
 * @param v1 first vector
 * @param v2 second vector
 * @param p power
 * @return the distance
 */
double Distances::calculateDistance(vector<double> v1, vector<double> v2, double p) {
    double sum = 0;

    for (int i = 0; i < v1.size(); i++) {
        sum += pow(abs(v1.at(i) - v2.at(i)), p);
    }

    return pow(sum, 1 / p);
}

/**
 * A function that calculates vectors distance according to chebyshev distance.
 * @param v1 first vector
 * @param v2 second vector
 * @return the distance
 */
double Distances::chebyshevDistance (vector<double> v1, vector<double> v2) {
    double differences[v1.size()];

    for (int i = 0; i < v1.size(); i++) {
        differences[i] = abs(v1.at(i) - v2.at(i));
    }

    double max = differences[0];
    for (int j = 1; j < v1.size(); j++) {
        if (differences[j] > max) {
            max = differences[j];
        }
    }

    return max;
}

/**
* A function that calculates vectors distance according to canberra distance.
* @param v1 first vector
* @param v2 second vector
* @return the distance
*/
double Distances::canberraDistance(vector<double> v1, vector<double> v2) {
    double sum = 0;

    for (int i = 0; i < v1.size(); i++) {
        double mone = abs(v1.at(i) - v2.at(i));
        double mechane = (abs(v1.at(i)) + abs(v2.at(i)));
        sum += (mone / mechane);
    }
    return sum;
}
