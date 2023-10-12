#include <iostream>
#include <vector>
#include <math.h>
#include <unordered_map>
#include <bitset>
#include <cmath>
#include <chrono>

using namespace std;

/*
    This function takes the exponent part of each number and returns
    the number that is 1/2 the exponent.

    Ex. if number = 64, 64 is 2 ^ 6.

    So the guess would be 2 ^ 3 (because 6/2 = 3)
*/

double initialGuess(double x)
{

    // 1) Convert the double into a 64 bit datatype

    uint64_t x_bits = *reinterpret_cast<uint64_t *>(&x);

    // 2) Bit shifting and masking so that we get bits 2-11 (the exponent).

    int exponent = (x_bits >> 52) & 0x7FF;
    exponent -= 1023; // Remove bias

    // 3) Divide unbiased exponent by 2 and do 2^ to get a close guess
    exponent /= 2;

    double result = pow(2, exponent);

    return result;
}

double squareroot(double x)
{
    double guess = initialGuess(x);

    double x_n = guess;

    int counter = 0;
    while (counter < 5)
    {
        x_n = x_n - (((x_n * x_n) - x) / (2 * x_n));

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

    //  auto start = std::chrono::high_resolution_clock::now();

    std::cout << squareroots(inputs, expected) << endl;

    //  auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> duration = end - start;

    // std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

    return 0;
}