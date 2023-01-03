#ifndef ASS01_DISTANCES_H
#define ASS01_DISTANCES_H
#include <cmath>
#include <vector>

/**
 * @author Ido Ziv, 318420254
 * @author Tal Gelerman, 322280850
 * @version ass1
 * @since 22.11.22
 */

using namespace std;

class Distances {
    public:
        /**
         * a function that calculates vectors distance according to
         * euclidean distance, taxicab geometry and minkowski distance.
         * @param v1 first vector
         * @param v2 second vector
         * @param p power
         * @return the distance
         */
       static double calculateDistance(vector<double> v1, vector<double> v2, double p);

        /**
         * A function that calculates vectors distance according to chebyshev distance.
         * @param v1 first vector
         * @param v2 second vector
         * @param p power
         * @return the distance
         */
        static double chebyshevDistance (vector<double> v1, vector<double> v2);

        /**
         * A function that calculates vectors distance according to canberra distance.
         * @param v1 first vector
         * @param v2 second vector
         * @param p power
         * @return the distance
         */
        static double canberraDistance(vector<double> v1, vector<double> v2);
};

#endif //ASS01_DISTANCES_H
