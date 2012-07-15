#include <cstdlib>
#include <string>
#include <thread>
#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include <functional>
#include <cmath>

#include "utils.hpp"

using namespace std;

typedef double num_t;

typedef ranlux_base_01 Engine;

static mutex mtx;

struct Parameters {
  num_t mu;
  num_t s2;
  num_t alpha; 
  num_t beta;
};

void dsection_mcmc_samples(Engine* eng, const vector<Parameters>& params);

num_t Normal(Engine& eng, const num_t mu, const num_t s2) {
  normal_distribution<num_t> normrand(mu,sqrt(s2));
  return( normrand(eng) );
}

num_t Gamma(Engine& eng, const num_t alpha, const num_t beta) {

  // A random number generator Gamma(alpha,1)
  gamma_distribution<num_t> gamrand(alpha);
  
  // Get a random number from Gamma(alpha,1) 
  // and scale it to Gamma(alpha,beta)
  return( gamrand(eng) / beta );
}

int main() {

  // Number of threads
  size_t nThreads = 2;
  size_t nSamples = 10;

  // Engines for probability distributiona
  vector<Engine> eng(nThreads);

  // Thread initializers
  vector<vector<Parameters> > parameters(nThreads,vector<Parameters>(nSamples,{1,2,3,4}));
  for ( unsigned int threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    eng[threadIdx].seed(threadIdx);
  }
  
  // Generate threads
  thread thr[nThreads];
  for ( unsigned int threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    thr[threadIdx] = thread(dsection_mcmc_samples,&eng[threadIdx],parameters[threadIdx]);
  }  
    
  // Join results
  for ( size_t threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    thr[threadIdx].join();
  }


  return( EXIT_SUCCESS );
}

void dsection_mcmc_samples(Engine* eng, const vector<Parameters>& params) {
  
  mtx.lock();

  for ( size_t i = 0; i < params.size(); ++i ) {
    
    cout << "ENGINE " << eng << ", SEED " << 1 << ", SAMPLE " << i << endl
	 << " - Normal(" << params[i].mu << "," << params[i].s2 << ") => " << Normal(*eng,params[i].mu,params[i].s2) << endl
	 << " - Gamma(" << params[i].alpha << "," << params[i].beta << ")  => " << Gamma(*eng,params[i].alpha,params[i].beta) << endl;
    
  }

  mtx.unlock();

}
