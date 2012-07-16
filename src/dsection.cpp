#include <cstdlib>
#include <string>
#include <thread>
#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include <functional>
#include <cmath>
#include <ctime>

#include "utils.hpp"
#include "distributions.hpp"
#include "datadefs.hpp"

using namespace std;
using datadefs::num_t;

static mutex mtx;

static bool DEBUG = false;

struct Parameters {
  num_t mu;
  num_t s2;
  num_t alpha; 
  num_t beta;
  vector<num_t> dirAlpha;
};

void dsection_mcmc_samples(distributions::Engine* eng, const Parameters& params, const size_t nSamples);

int main(const int argc, char* const argv[]) {

  assert( argc == 3 );

  // Store the start time (in clock cycles) just before the analysis
  //clock_t clockStart( clock() );

  clock_t timeStart = time(0);

  // Number of threads
  size_t nThreads = static_cast<size_t>(atoi(argv[1]));
  size_t nSamples = static_cast<size_t>(atoi(argv[2]));

  cout << endl
       << "MCMC set-up:" << endl
       << " - " << nThreads << " threads" << endl
       << " - " << nSamples << " samples" << endl << endl;

  // Engines for probability distributiona
  vector<distributions::Engine> eng(nThreads);

  // Thread initializers
  vector<num_t> dirAlpha(3);
  dirAlpha[0] = 1;
  dirAlpha[1] = 5;
  dirAlpha[2] = 3;

  vector<Parameters> parameters(nThreads,{1,2,3,4,dirAlpha});
  for ( unsigned int threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    eng[threadIdx].seed(threadIdx);
  }
  
  // Generate threads
  thread thr[nThreads];
  for ( unsigned int threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    thr[threadIdx] = thread(dsection_mcmc_samples,&eng[threadIdx],parameters[threadIdx],nSamples);
  }  
    
  // Join results
  for ( size_t threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    thr[threadIdx].join();
  }

  cout << endl
       << time(0) - timeStart << " seconds elapsed" << endl;

  return( EXIT_SUCCESS );
}

void dsection_mcmc_samples(distributions::Engine* eng, const Parameters& params, const size_t nSamples) {

  if ( DEBUG ) {    
    mtx.lock();
    for ( size_t i = 0; i < nSamples; ++i ) {
      cout << "ENGINE " << eng << ", SEED " << 1 << ", SAMPLE " << i << endl
	   << " - Normal(" << params.mu << "," << params.s2 << ") => " << distributions::normal(*eng,params.mu,params.s2) << endl
	   << " - Gamma(" << params.alpha << "," << params.beta << ")  => " << distributions::gamma(*eng,params.alpha,params.beta) << endl
	   << " - Dirichlet(";
      utils::print(params.dirAlpha.begin(),params.dirAlpha.end(),',');
      cout << ") => (";
      vector<num_t> sample = distributions::dirichlet(*eng,params.dirAlpha);
      utils::print(sample.begin(),sample.end(),',');
      cout << ")" << endl;
    }
    mtx.unlock();
  } else {
    for ( size_t i = 0; i < nSamples; ++i ) {
      num_t foo1 = distributions::normal(*eng,params.mu,params.s2);
      num_t foo2 = distributions::gamma(*eng,params.alpha,params.beta);
      vector<num_t> foo3 = distributions::dirichlet(*eng,params.dirAlpha);
    }
  }
  
  
}
