# ass03
In this repository, you can find the most updated version of Ido Ziv an Tal Gelerman's project in "Advanced Programming" course.
First we describe the algorithm and the optimizations, then how compile and run the program and then the inputs expectations.

### Algorithm description
In this project we get from the client enters an input - an unclassified vector, an algorithm method and a K, and the server classifies the vector and sent the client the classification result.
The program finishes when the client's input is "-1"

In this project you will be able to find the following documents:
- GenericVector.cpp - a generic class for differnt types of vectors. This class contains a vector tyoe name, values and a function the calculate ditstance from another vector.
- Distances.cpp - the distance class that contains the calculating distance different functions.
-Client.cpp - the client side of the program including basic validation of the user input.
- Server.cpp - the server side of the program including an additional input validation, the read from file, partition and calculating processes.

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
git clone https://github.com/TalGelerman/Advanced_programming_2.git
```
2. A folder named "Advanced_programming_2" will be created. Access the cloned directory by using the cd command:
```
cd Advanced_programming_2
```
3. Enter to command:
```
make
```
4. A file named "client.out" and "Server.out" should be created on this folder. You are ready to run the project with the command:
```
./server.out file port 
```
While file is a file path, and port

5. use the command for client too:
```
./client.out ip port 
```
While file is a valid ip and port (pay attention you need the same port number for both server and client)

6.Now, the program expect the user to enter an input - vector distance k , While vector is the unclassified vector, distance is the algorithm method and k.

The program expect:
- Same port number for both client and server, and to in range of 1024 to 65535
- A file path to a valid file
- A valid ip number
- K to be a non-negative number and an integer
- A file path to be a valid path to an existing and readable file. Each line should be vector with double values, seperated by ',', and a string which represents the vector type. Each line should have the same amount of values.
- Distance matric that is one of : AUC, MAN, CHB, CAN, MIN.

In case that the input is invalid, an "invalid input" message will be shown up.

After you run the program, the program expect the user to enter a line, representing the vector, inserted with valid numbers, seperated by " "(space). 

A valid input is a number that consists of:
- Digits (0-9)
- Only one dot if needed , not as the first or last chararcter.
- Minus ('-') as the first characters as a representation of a negative number.
-empty input is invalid


