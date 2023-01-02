# ass02
In this repository, you can find the most updated version of Ido Ziv an Tal Gelerman's first project in "Advanced Programming" course.
First we describe the algorithm and the optimizations, then how compile and run the program and then the inputs expectations.

### Algorithm description
In this project we get from the console 3 arguments - k, a file path and a distance matric. In addition we get unclassified vector from the user, and by using KNN algorithm, we classifies the vector.

In this project you will be able to find the following documents:
- GenericVector.cpp - a generic class for differnt types of vectors. This class contains a vector tyoe name, values and a function the calculate ditstance from another vector.
- Distances.cpp - the distance class that contains the calculating distance different functions.
- Main.cpp - conatins the main function and also the reading from files process,  different functions of validation.

In order to optimize the project we defined a generic class for a generic vector that is readen from the file.
This class contains the vector's type name, the vector's values and a distance function, which is also generic, that calculates the distance from an additional vector according to a calculation method.
These generic class and function give the program the functionality to get any  type of vector, with differnt distance calculating methods (that we implemented in ass01) and to calssify thr vector's type.

An additional optimization we applied in this code was added after we noticed that the Euclidean, Manhattan and Minkowski algorithms all have the same form – and the only difference is the power.
Therefore, we defined the function “calculateDistance” which gets the two vectors and and an int p as the power for the formula.
- For Euclidean distance - sqrt(calculateDistance(v1,v2,2))
- For Taxicab distance - calculateDistance(v1,v2,1)
- For Minkowski distance - (calculateDistance(v1,v2,p))^(1/p) ("p" is a const)


### How to compile and run the program?
1. Clone the repository using the command:
```
git clone https://github.com/idoZiv15/ass02.git
```
2. A folder named "ass02" will be created. Access the cloned directory by using the cd command:
```
cd ass02
```
3. Enter to command:
```
make
```
4. A file named "ex2" should be created on this folder. You are ready to run the project with the command:
```
./ex2 K filePath distanceMetric
```
While K, filepath and distanceMetric are the arguments for the algorithm.

The program expect:
- K to be a non-negative number and integer
- A file path to be a valid path to an existing and readable file. Each line should be vector with double values, seperated by ',', and a string which represents the vector type. Each line should have the same amount of values.
- Distance matric that is one of : AUC, MAN, CHB, CAN, MIN.

After you run the program, the program expect the user to enter a line, representing the vector, inserted with valid numbers, seperated by " "(space). 

A valid input is a number that consists of:
- Digits (0-9)
- Only one dot if needed , not as the first or last chararcter.
- Minus ('-') as the first characters as a representation of a negative number.
-empty input is invalid


