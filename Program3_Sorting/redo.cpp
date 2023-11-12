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

using namespace std;

// I've set this just to keep the numbers in a reasonable range when
// we print them out... for your run, please set it to 2 billion or so
constexpr int MAXELEMENT = 10;
class Process {
  vector<int> sorted_contents_;
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
    FIND_SIZE = 4
    
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

  int broadcast(vector<Process>& everyone, int c, int argument) {
    vector<int> responses;

    // The next line looks at the leader process and calls the command function
    // For the max function, this should push back the maximum element from each process
    responses.push_back( command(c,argument) );

    // The for loop does the same for the rest of the processes
    for(auto& p:everyone) {
      if (&p != this) {
        responses.push_back(p.command(c,argument));
      }
    }

    return reduce(c, argument, responses);
  }


  // Each process will execute this when it hears a broadcast
  int command(int c, int argument) {

    // for FIND_SIZE
    int size;

    // for CHOOSE_MAX
    int lastElement = sorted_contents_.back();

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
        
        return lastElement;


    // Goal 1: Print total size of the current process' list
    case CHOOSE_MEDIAN:

        cout << "Working on it" << endl;
        
        

    
    case FIND_SIZE:

        size = sorted_contents_.size();
        return size;

    }
    

    

    
    


    return -1;
  }
  // The leader can see all the responses
  int reduce(int c, int argument, const vector<int>& responses) {

    int responsesSize = responses.size();
    int totalSize = 0;
    

    int overallMax = 0;

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
        cout << "Reduce - Still working on it" << endl;



    case FIND_SIZE:

        for(int i = 0; i < responsesSize; i++) {
            int sizeOfEachProcess = responses[i];
            totalSize += sizeOfEachProcess;
        }

        return totalSize;

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
    leader->broadcast(processes, Process::PRINT,0);
  }


  int max() {
    auto* leader = pick_a_leader();
    return leader->broadcast(processes, Process::CHOOSE_MAX,0);

  }


  int median() {
    auto* leader = pick_a_leader();

    // Goal: Find out the size of the the whole dataset (how many numbers are there in all processes combined)
    int size = leader->broadcast(processes, Process::FIND_SIZE,0);

    // Goal: Pass in the total size. Now we know we are looking for the size / 2 element 
    return leader->broadcast(processes, Process::CHOOSE_MEDIAN,size);
  
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
    int maxvalue = leader->command(Process::VOTE_FOR_LEADER,processes.size());
    for(auto& p:processes) {
      int vote = p.command(Process::VOTE_FOR_LEADER,processes.size());
      if ( vote > maxvalue) {
	leader = &p;
	maxvalue = vote;
      }
    }
    return leader;
  }
};
  
int main() {
  // Providing a seed value.  For debug, just pick a constant integer here
  // srand((unsigned) time(NULL));
  
  srand((unsigned) time(NULL));
  Distributed D(3 /* processes */, 2,2 /* with between 10 and 15 numbers */);

  cout << endl;
  D.print_all();
  cout << endl;

  cout << "All 3 processes sorted: "; 
  for(const auto& x:D.cheat()) {
    cout << x << ' ';
  }
  cout << endl;
  

  int calculatedMaxValue = D.max();

  cout << "Overall Max: " << calculatedMaxValue << endl;
  
  cout << endl;

  int calculatedMedianValue = D.median();

  cout << "Overall Median " << calculatedMedianValue << endl;

  return 0;
}