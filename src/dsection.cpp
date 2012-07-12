#include <cstdlib>
#include <string>
#include <thread>
#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include <functional>

#include "utils.hpp"

using namespace std;

void my_thread_func() {

}

int main() {

  size_t nThreads = 10;

  // Generate threads
  thread t[nThreads];
  for ( size_t threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    t[threadIdx] = thread(my_thread_func);
  }  

  // Join results
  for ( size_t threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    t[threadIdx].join();
  }

  // ... which we split evenly across the threads
  vector<vector<size_t> > icsSets = utils::splitRange(7,nThreads);

  // Print the batches
  for ( size_t i = 0; i < icsSets.size(); ++i ) {
    cout << "Set " << i << " has " << icsSets[i].size() << " indices: ";
    for ( size_t j = 0; j < icsSets[i].size(); ++j ) {
      cout << " " << icsSets[i][j];
    }
    cout << endl;
  }

  // Define distribution and engine for Normal(0,1) distribution
  normal_distribution<double> normdist(0,1);
  ranlux64_base_01 eng;

  auto normRNG = bind(normdist,eng);

  for ( size_t i = 0; i < 100; ++i ) {
    cout << " " << normRNG();
  }
  cout << endl;

  return( EXIT_SUCCESS );
}
