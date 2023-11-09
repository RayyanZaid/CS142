#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>

constexpr int MAXELEMENT = 6;

int chooseMedian(std::vector<int> &values)
{
    int n = values.size();
    int k = n / 2; // Index of the median element

    int left = 0;
    int right = n - 1;
    while (left < right)
    {
        int pivotIndex = left + std::rand() % (right - left + 1);
        int pivotValue = values[pivotIndex];

        // Partition the array
        std::swap(values[pivotIndex], values[right]);
        int i = left;
        for (int j = left; j < right; ++j)
        {
            if (values[j] < pivotValue)
            {
                std::swap(values[i], values[j]);
                i++;
            }
        }
        std::swap(values[i], values[right]);

        if (i == k)
        {
            // Found the median
            return values[i];
        }
        else if (i < k)
        {
            left = i + 1;
        }
        else
        {
            right = i - 1;
        }
    }

    // If left == right, that element is the median
    return values[left];
}

class Process
{
    std::vector<int> sorted_contents_;

public:
    typedef enum
    {
        PRINT = 0,
        VOTE_FOR_LEADER = 1,
        CHOOSE_MAX = 2,
        CHOOSE_MEDIAN = 3 // Added a new command for choosing the median
    } commands;

    Process(int m)
    {
        for (int i = 0; i < m; ++i)
        {
            sorted_contents_.push_back(std::rand() % MAXELEMENT);
        }
        std::sort(sorted_contents_.begin(), sorted_contents_.end());
    }

    void cheat(std::vector<int> &cheat_values) const
    {
        cheat_values.insert(cheat_values.end(),
                            sorted_contents_.begin(),
                            sorted_contents_.end());
    }

    int command(int c, int argument)
    {
        switch (c)
        {
        case PRINT:
            for (const auto &x : sorted_contents_)
            {
                std::cout << x << ' ';
            }
            std::cout << std::endl;
            return 0;
        case VOTE_FOR_LEADER:
            return sorted_contents_.back() + std::rand() % argument;
        case CHOOSE_MAX:
            return sorted_contents_.back();
        case CHOOSE_MEDIAN:
            // Choose the middle element as a candidate for the median
            return sorted_contents_[sorted_contents_.size() / 2];
        }
        return -1;
    }
};

class Distributed
{
public:
    Distributed(int M, int from, int to)
    {
        for (int i = 0; i < M; ++i)
        {
            int m = from + std::rand() % (to - from + 1);
            processes.emplace_back(m);
        }
    }

    void print_all()
    {
        auto *leader = pick_a_leader();
        leader->command(Process::PRINT, 0);
    }

    int max()
    {
        auto *leader = pick_a_leader();
        return leader->command(Process::CHOOSE_MAX, 0);
    }

    int median()
    {
        auto *leader = pick_a_leader();
        return leader->command(Process::CHOOSE_MEDIAN, 0);
    }

    std::vector<int> getSortedValues()
    {
        std::vector<int> all_values;
        for (auto &p : processes)
        {
            p.cheat(all_values);
        }
        std::sort(all_values.begin(), all_values.end());
        return all_values;
    }

private:
    std::vector<Process> processes;

    Process *pick_a_leader()
    {
        if (processes.size() == 0)
            return nullptr;

        Process *leader = &processes.front();
        int max_value = leader->command(Process::VOTE_FOR_LEADER, processes.size());
        for (auto &p : processes)
        {
            int vote = p.command(Process::VOTE_FOR_LEADER, processes.size());
            if (vote > max_value)
            {
                leader = &p;
                max_value = vote;
            }
        }
        return leader;
    }
};

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    Distributed D(3, 2, 3);

    D.print_all();
    std::cout << "Max value: " << D.max() << std::endl;
    std::cout << "Median value: " << D.median() << std::endl;

    std::vector<int> sortedValues = D.getSortedValues();
    std::cout << "Sorted values: ";
    for (const auto &x : sortedValues)
    {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    return 0;
}
