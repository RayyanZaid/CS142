// Your goal is to do some searching/sorting in distributed data sets
// Easy: Find the greatest number -- start with this one.  A leader
// can do it with one broadcast and then pick the biggest number from
// everyone
//
// Hard: Pick the median
// This requires several rounds... You will need to add some commands
// and have the leader's reduce make several more broadcasts (or have
// the Distributed object pick new leaders and make new broadcasts)
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include <random>

using namespace std;

// I've set this just to keep the numbers in a reasonable range when
// we print them out... for your run, please set it to 2 billion or so
constexpr int MAXELEMENT = 2000000000;
class Process {
  vector<int> sorted_contents_;
  vector<int> largerThanK;

public:
  // Add new commands here... Think of the process as a thing that receives
  // a broadcast command (one of these) and an argument and creates a response
  // One process is designated as the leader.  It will receive all responses
  // (including its own as the first element)
  typedef enum {
    PRINT = 0,
    VOTE_FOR_LEADER = 1,
    CHOOSE_MAX = 2,
    CHOOSE_MEDIAN = 3,
    FIND_SIZE = 4,
    CHOOSE_MIN = 5
    
  } commands;
  // Just fill with some random data... 
  Process(int m) {
    for(int i=0;i<m;++i) {
      sorted_contents_.push_back( rand() % MAXELEMENT );
    }
    sort(sorted_contents_.begin(), sorted_contents_.end());
  }
  // This is cheating
  void cheat(vector<int>& cheat_values) const {
    cheat_values.insert(cheat_values.end(),
			sorted_contents_.begin(),
			sorted_contents_.end());
  }

  // Only the leader will call broadcast... it will receive all the
  // responses and reduce it.

  // 3 parameters to the broadcast function:

  // 1) all the processes   
  // 2) the command to dictate what you want the processes to do     
  // 3) an argument you can pass in

  int broadcast(vector<Process>& everyone, int c, int argument, int argument2) {
    vector<int> responses;

    // The next line looks at the leader process and calls the command function
    // For the max function, this should push back the maximum element from each process
    responses.push_back( command(c,argument,argument2) );

    // The for loop does the same for the rest of the processes
    for(auto& p:everyone) {
      if (&p != this) {
        responses.push_back(p.command(c,argument,argument2));
      }
    }

    return reduce(c, argument, responses);
  }


  // Each process will execute this when it hears a broadcast
  int command(int c, int argument, int argument2) {

    // for FIND_SIZE
    int size;

    // for CHOOSE_MAX
    int lastElement = 0;


    // for CHOOSE_MEDIAN 
    int lowerBound = argument;
    int upperBound = argument2;
    int numElementsInRange = 0;

    // for CHOOSE_MIN
    int firstElement;
    
    switch(c) {
    case PRINT:
    
    cout << "Process: ";

    
    for(const auto& x:sorted_contents_) {
	    cout << x << ' ';
    }
    
    cout << endl;


    return 0;


    case VOTE_FOR_LEADER:
      // I will vote for being the leader by sending my biggest value
      // plus a random offset
      return sorted_contents_.back() + rand() % argument;



    // Goal: return the last element of each process
    case CHOOSE_MAX:

        if(argument == 0) {
            lastElement = sorted_contents_.back();
        }

        else {
          for(const auto& x:sorted_contents_) {
	          if(x <= upperBound) {
              lastElement = x;
            }
            else {
              return lastElement;
            }
          }
        }
        
        return lastElement;


    // Goal: return number of elements within the range in the arguments
    case CHOOSE_MEDIAN:

        // cout << "Working on it" << endl;
        

        for(int i = 0; i < sorted_contents_.size(); i++) {
            if (sorted_contents_[i] <= upperBound) {
              numElementsInRange++;
            }

            else {
              return numElementsInRange;
            }
        }

        return numElementsInRange;
        

    // Goal: Return the size of each process
    case FIND_SIZE:

        size = sorted_contents_.size();
        return size;


    case CHOOSE_MIN:

      firstElement = sorted_contents_.front();
      return firstElement;
    }
    

    

    
    


    return -1;
  }
  // The leader can see all the responses
  int reduce(int c, int argument, const vector<int>& responses) {

    int responsesSize = responses.size();
    int totalSize = 0;
    

    int overallMax = 0;
    int overallMin = MAXELEMENT;


    int numElementsInRange = 0;

    switch(c) {



    case PRINT:
      return -1;

    case CHOOSE_MAX:

        

        for(int i = 0; i < responsesSize; i++) {
            int maxOfEachProcess = responses[i];
            if (maxOfEachProcess > overallMax) {
                overallMax = maxOfEachProcess;
            }
        }

        return overallMax;

    
    case CHOOSE_MEDIAN:
        for(int i = 0; i < responsesSize; i++) {
            int numElementsInRangeOfEachProcess = responses[i];
            numElementsInRange += numElementsInRangeOfEachProcess;
        }

        return numElementsInRange;



    case FIND_SIZE:

        for(int i = 0; i < responsesSize; i++) {
            int sizeOfEachProcess = responses[i];
            totalSize += sizeOfEachProcess;
        }

        return totalSize;


    case CHOOSE_MIN:

      for(int i = 0; i < responsesSize; i++) {
            int minOfEachProcess = responses[i];
            if (minOfEachProcess < overallMin) {
                overallMin = minOfEachProcess;
            }
        }

      return overallMin;

    }

    


    




    return -1;
  }
};






class Distributed {

public:
  Distributed(int M, int from, int to) {
    for (int i=0; i < M; ++i) {
      int m = from + rand()%(to-from+1);
      processes.emplace_back( m );
    }
  }


  void print_all() {
    auto* leader = pick_a_leader();
    leader->broadcast(processes, Process::PRINT,0,0);
  }


  int max() {
    auto* leader = pick_a_leader();
    return leader->broadcast(processes, Process::CHOOSE_MAX,0,0);

  }

int median() {
  auto* leader = pick_a_leader();

  // Find the global min, max, and size.
  int globalMin = leader->broadcast(processes, Process::CHOOSE_MIN, 0, 0);
  int globalMax = leader->broadcast(processes, Process::CHOOSE_MAX, 0, 0);
  int totalSize = leader->broadcast(processes, Process::FIND_SIZE, 0, 0);
  
  // Calculate the median position(s).
  // For even sizes, I take the average of the kth and (k+1)th elements.
  int kth = (totalSize - 1) / 2;
  int kthPlusOne = totalSize / 2; 
  
  // Initialize bounds for  search.
  int currentMin = globalMin;
  int currentMax = globalMax;
  int kthValue = -1;
  int kthPlusOneValue = -1;

  // Find kth smallest value by partitioning.
  while (currentMin <= currentMax) {

    // get the middle element we want to create the partition at
    int midValue = currentMin + (currentMax - currentMin) / 2;

    // get the number of elements from ALL processes that are <= midValue
    int count = leader->broadcast(processes, Process::CHOOSE_MEDIAN, currentMin, midValue);
    
    // This sees if the total number <= mid element is less than the median index.

    // If so, this means that the median is somewhere in the upper partition, so we can disregard all those lower numbers.
    if (count < kth + 1) { // Plus one because count starts from 1.
      currentMin = midValue + 1;
    } 
    
    // Else, the opposite is true and we can disregard the upper partition.
    else {
      kthValue = midValue;
      currentMax = midValue - 1;
    }

  }
  
  // Reset bounds for finding (k+1)th smallest value.
  currentMin = kthValue;
  currentMax = globalMax;

  // Find (k+1)th smallest value when size is even.

  if (totalSize % 2 == 0) {
    while (currentMin <= currentMax) {
      int midValue = currentMin + (currentMax - currentMin) / 2;
      int count = leader->broadcast(processes, Process::CHOOSE_MEDIAN, currentMin, midValue);
      
      if (count < kthPlusOne + 1) { // Adjust the count similarly as above.
        currentMin = midValue + 1;
      } else {
        kthPlusOneValue = midValue;
        currentMax = midValue - 1;
      }
    }
  } else {
    kthPlusOneValue = kthValue; // For odd sizes, kth and (k+1)th are the same.
  }

  if (kthValue != -1 && kthPlusOneValue != -1) {
    // For odd sizes, this just returns kthValue.
    return (kthValue + kthPlusOneValue) / 2;
  }

  // If we can't find the median, return an error code (this should not happen).
  return -1;
}

// function I implemented for testing. used to get the median and mean to compare.
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

  vector<int> cheat() const {
    vector<int> all_values;
    for(auto& p:processes) {
      p.cheat(all_values);
    }
    sort(all_values.begin(), all_values.end());
    return all_values;
  }
private:
  vector<Process> processes;
  Process* pick_a_leader() {
    if (processes.size() == 0) return nullptr;
    
    // everyone votes, pick the biggest vote
    Process* leader = &processes.front();
    int maxvalue = leader->command(Process::VOTE_FOR_LEADER,processes.size(),0);
    for(auto& p:processes) {
      int vote = p.command(Process::VOTE_FOR_LEADER,processes.size(),0);
      if ( vote > maxvalue) {
	leader = &p;
	maxvalue = vote;
      }
    }
    return leader;
  }

  
};

void accuracy(int numTests) {

    int medianNumRight = 0;
    int meanNumRight = 0;
    int i = 1;


    std::random_device rd; 
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    while(i <= numTests) {
        
        i++;

        srand((unsigned) time(NULL));


        std::uniform_int_distribution<> distrib(1, 20); 

        // Generate and print out a random number
        int numProcesses = distrib(gen);

        std::uniform_int_distribution<> distrib1(1, 300); 

        int numLower = distrib1(gen);

        std::uniform_int_distribution<> distrib2(numLower, 500); 

        int numHigher = distrib2(gen);
        Distributed D(numProcesses /* processes */, numLower, numHigher /* with between 10 and 15 numbers */);

        cout << endl << endl << endl;
        // D.print_all();
        

        // cout << "All 3 processes sorted: "; 
        // for(const auto& x:D.cheat()) {
        //     cout << x << ' ';
        // }
        // cout << endl;
        cout << "Test " << i << " / " << numTests << endl;
        cout << "Processes: " << numProcesses << "  " << "Lowest Num: " << numLower << "  " << "Highest Num: " << numHigher << endl << endl;

        std::vector<int> sortedValues = D.getSortedValues();


        int calculatedMaxValue = D.max();

        cout << "Overall Max: " << calculatedMaxValue << endl;
        
        cout << "Actual Max: " << sortedValues.back() << endl;
        cout << endl;

        if(calculatedMaxValue == sortedValues.back()) {
            meanNumRight++;
        }

        int calculatedMedianValue = D.median();

        cout << "Overall Median: " << calculatedMedianValue << endl;

        

        int totalSize = sortedValues.size();
        if(totalSize % 2 == 0) {
                cout << "Actual Median: " << "(" << sortedValues[sortedValues.size()/2 - 1]  << " + " << sortedValues[sortedValues.size()/2 ] << ") / 2" << " = " << (sortedValues[sortedValues.size()/2] + sortedValues[sortedValues.size()/2 - 1]) / 2  << endl;

                if(calculatedMedianValue == (sortedValues[sortedValues.size()/2] + sortedValues[sortedValues.size()/2 - 1]) / 2  ) {
                    medianNumRight++;
                }
        }

        else {
            cout << "Actual Median: " << sortedValues[sortedValues.size()/2] << endl;

            if(calculatedMedianValue == sortedValues[sortedValues.size()/2] ) {
                medianNumRight++;
            }
        }

        
        

    }

    cout << endl;
    cout << "Mean accuracy: " << meanNumRight << " / " << numTests << " = " << ((1.0 * meanNumRight) / (1.0 * numTests)) * 100 << "%" << endl;
    cout << "Median accuracy: " << medianNumRight << " / " << numTests << " = " << ((1.0 * medianNumRight) / (1.0 * numTests)) * 100 << "%" << endl;

}




void speed() {

}
int main() {
  
  int numTests = 100;
  accuracy(numTests);
  
  
  
  
  
  
  
//   Providing a seed value.  For debug, just pick a constant integer here
//   srand((unsigned) time(NULL));
  




//   srand((unsigned) time(NULL));
//   Distributed D(20/* processes */, 10,100 /* with between 10 and 15 numbers */);

//   cout << endl;
//   D.print_all();
//   cout << endl;

//   cout << "All 3 processes sorted: "; 
//   for(const auto& x:D.cheat()) {
//     cout << x << ' ';
//   }
//   cout << endl;
  

//   int calculatedMaxValue = D.max();

//   cout << "Overall Max: " << calculatedMaxValue << endl;
  
//   cout << endl;

//   int calculatedMedianValue = D.median();

//   cout << "Overall Median " << calculatedMedianValue << endl;

//   std::vector<int> sortedValues = D.getSortedValues();

//   int totalSize = sortedValues.size();
//   if(totalSize % 2 == 0) {
//         cout << "Actual Median is " << "(" << sortedValues[sortedValues.size()/2 - 1]  << " + " << sortedValues[sortedValues.size()/2 ] << ") / 2" << " = " << (sortedValues[sortedValues.size()/2] + sortedValues[sortedValues.size()/2 - 1]) / 2  << endl;
//   }

//   else {
//        cout << "Actual Median " << sortedValues[sortedValues.size()/2] << endl;
//   }
   
  return 0;
}