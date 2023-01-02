/**
 * @author Ido Ziv, 318420254
 * @author Tal Gelerman, 322280850
 * @version ass3
 * @since 02.01.23
 */

#ifndef MAIN_CPP_SERVER_H
#define MAIN_CPP_SERVER_H
#include "GenericVector.h"

using namespace std;

class Server {
private:

public:
    static int partition (vector<GenericVector> &arr, int low, int high);
};

#endif //MAIN_CPP_SERVER_H
