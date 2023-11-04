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
// I've set this just to keep the numbers in a reasonable range when
// we print them out... for your run, please set it to 2 billion or so
constexpr int MAXELEMENT = 1000;
class Process {
  std::vector<int> sorted_contents_;
public:
  // Add new commands here... Think of the process as a thing that receives
  // a broadcast command (one of these) and an argument and creates a response
  // One process is designated as the leader.  It will receive all responses
  // (including its own as the first element)
  typedef enum {
    PRINT = 0,
    VOTE_FOR_LEADER = 1
  } commands;
  // Just fill with some random data... 
  Process(int m) {
    for(int i=0;i<m;++i) {
      sorted_contents_.push_back( std::rand() % MAXELEMENT );
    }
    std::sort(sorted_contents_.begin(), sorted_contents_.end());
  }
  // This is cheating
  void cheat(std::vector<int>& cheat_values) const {
    cheat_values.insert(cheat_values.end(),
			sorted_contents_.begin(),
			sorted_contents_.end());
  }
  // Only the leader will call broadcast... it will receive all the
  // responses and reduce it.
  int broadcast(std::vector<Process>& everyone, int c, int argument) {
    std::vector<int> responses;
    responses.push_back( command(c,argument) );
    for(auto& p:everyone) {
      if (&p != this) {
	responses.push_back(p.command(c,argument));
      }
    }
    return reduce(c, argument, responses);
  }
  // Each process will execute this when it hears a broadcast
  int command(int c, int argument) {
    switch(c) {
    case PRINT:
      for(const auto& x:sorted_contents_) {
	std::cout << x << ' ';
      }
      std::cout << std::endl;
      return 0;
    case VOTE_FOR_LEADER:
      // I will vote for being the leader by sending my biggest value
      // plus a random offset
      return sorted_contents_.back() + std::rand() % argument;
    }
    return -1;
  }
  // The leader can see all the responses
  int reduce(int c, int argument, const std::vector<int>& responses) {
    switch(c) {
    case PRINT:
      return -1;
    }
    return -1;
  }
};



class Distributed {
public:
  Distributed(int M, int from, int to) {
    for (int i=0; i < M; ++i) {
      int m = from + std::rand()%(to-from+1);
      processes.emplace_back( m );
    }
  }
  void print_all() {
    auto* leader = pick_a_leader();
    leader->broadcast(processes, Process::PRINT,0);
  }
  int max() {
    auto* leader = pick_a_leader();
    // return leader->broadcast(processes, Process::CHOOSE_MAX,0);
    return 0;
  }
  int median() {
    auto* leader = pick_a_leader();
    // return leader->broadcast(processes, Process::CHOOSE_MEDIAN,0);
    return 0;
  }
  std::vector<int> cheat() const {
    std::vector<int> all_values;
    for(auto& p:processes) {
      p.cheat(all_values);
    }
    std::sort(all_values.begin(), all_values.end());
    return all_values;
  }
private:
  std::vector<Process> processes;
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
  std::srand((unsigned) time(NULL));
  Distributed D(3 /* processes */, 10,15 /* with between 10 and 15 numbers */);
  D.print_all();
  for(const auto& x:D.cheat()) {
    std::cout << x << ' ';
  }
  std::cout << std::endl;
  return 0;
}