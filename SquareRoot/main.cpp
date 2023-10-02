#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_map>

using namespace std;

unordered_map<int, double> sqrtMap = {
    {1, 1.0},
    {2, 1.41421},
    {3, 1.73205},
    {4, 2.0},
    {5, 2.23607},
    {6, 2.44949},
    {7, 2.64576},
    {8, 2.82843},
    {9, 3.0}};

double initialGuess(double x)
{

    if (x < 10)
    {
        int nearest = static_cast<int>(round(x));
        return sqrtMap[nearest];
    }

    double sqrt_2 = 1.414213562;
    double sqrt_10 = 3.16227766;

    double divideBy10 = x / 10;

    double guess = sqrt_10 * sqrt_2 * (divideBy10 / 2);

    return guess;
}

double calculateDerivative(double x) {
    return 2 * x;
}

double calculateFunction(double prevX, double xToSolveFor) {
    return prevX * prevX - xToSolveFor;
}

double squareroot(double x)
{
    double guess = initialGuess(x);

    double x_n = guess;

    bool changeIsAlot = true;

    int counter = 0;
    while (changeIsAlot || counter < 5)
    {

        double temp = x_n;
        x_n = 0.5 * (x_n + (x / x_n));

        double change = abs(temp - x_n);

        if (change <= 10)
        {
            changeIsAlot = false;
        }
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
    double x = 75;
  
        inputs.push_back(x);
        expected.push_back(::sqrt(x));
    

    std::cout << squareroots(inputs, expected);

    return 0;
}