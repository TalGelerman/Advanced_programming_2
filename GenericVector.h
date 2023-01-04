#ifndef MAIN_CPP_IRISVECTOR_H
#define MAIN_CPP_IRISVECTOR_H

/**
 * @author Ido Ziv, 318420254
 * @author Tal Gelerman, 322280850
 * @version ass3
 * @since 04.01.23
 */

#include <string>
#include <vector>

using namespace std;

// This is a class definition. It defines a class called GenericVector. It has three private variables: type, values,
// and distance.
class GenericVector {
private:
    string type;
    vector<double> values;
    double distance;
public:
    string getType();
    double getDistance() const;
    vector<double> getValues();
    void setType(string type);
    void setValues(vector<double> values);
    void setDistance(double dist);
};


#endif //MAIN_CPP_IRISVECTOR_H
