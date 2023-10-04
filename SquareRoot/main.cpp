#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_map>
#include <bitset>
#include <cmath>
#include <chrono>


using namespace std;


double initialGuess(double x)
{

    uint64_t x_bits = *reinterpret_cast<uint64_t*>(&x);

    if(x < 1) {
        return 1;
    }


    int exponent = (x_bits >> 52) & 0x7FF;
    exponent -= 1023; // Remove bias
    

    exponent = exponent / 2;

    int result = pow(2,exponent);

 

    return result;

}

double calculateDerivative(double x)
{
    return 2 * x;
}

double calculateFunction(double prevX, double xToSolveFor)
{
    return prevX * prevX - xToSolveFor;
}

double squareroot(double x)
{
    double guess = initialGuess(x);

    double x_n = guess;



    int counter = 0;
    while (counter < 3)
    {

        double temp = x_n;

        x_n = x_n - (calculateFunction(x_n, x) / calculateDerivative(x_n));

        double change = abs(temp - x_n);

   
        counter++;
    }

    return x_n;
}

double squareroots(const std::vector<double> &x,
                   const std::vector<double> &expected)
{
    double sum_of_squares_of_error = 0.0;
    for (int i = 0; i < x.size(); ++i)
    {
        double mine = squareroot(x[i]);
        double error = mine - expected.at(i);
        sum_of_squares_of_error += error * error;
    }

    return sum_of_squares_of_error;
}

int main()
{
    std::vector<double> inputs;
    std::vector<double> expected;
    double x;


    while (std::cin >> x)
    {
        inputs.push_back(x);
        expected.push_back(::sqrt(x));
    }

     auto start = std::chrono::high_resolution_clock::now();
     
    std::cout << squareroots(inputs, expected) << endl;

     auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    return 0;
}