#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_map>

using namespace std;

double initialGuess(double x)
{

    double doubleValue = x; // Replace with your double value

    // Use type punning to interpret the double as a long long
    long long longLongValue;
    std::memcpy(&longLongValue, &doubleValue, sizeof(double));

    // Extract the individual bits
    std::bitset<64> bits(longLongValue); // Assuming a 64-bit double

    // Print the bits
    std::cout << "Double Value: " << doubleValue << std::endl;
    std::cout << "Binary Representation: " << bits << std::endl;

    return 1;
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

    bool changeIsAlot = true;

    int counter = 0;
    while (changeIsAlot || counter < 5)
    {

        double temp = x_n;

        x_n = x_n - (calculateFunction(x_n, x) / calculateDerivative(x_n));

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
    double x;

    while (std::cin >> x)
    {
        inputs.push_back(x);
        expected.push_back(::sqrt(x));
    }

    std::cout << squareroots(inputs, expected);

    return 0;
}