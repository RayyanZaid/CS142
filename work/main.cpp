#include <iostream>
#include <vector>

const int N = 1000000; // Adjust the number of iterations as needed

int main()
{
    std::vector<int> data(N);

    for (int i = 0; i < N; i++)
    {
        data[i] = i;
    }

    // Print a message so you can attach 'perf' to your program
    std::cout << "Press Enter to continue and start 'perf'..." << std::endl;
    std::cin.get();

    // Access the data in a way that might stress the cache
    long long sum = 0;
    for (int i = 0; i < N; i++)
    {
        sum += data[i];
    }

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
