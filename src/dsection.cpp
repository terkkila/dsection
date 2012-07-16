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
  vector<num_t> dirAlpha;
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

vector<num_t> Dirichlet(Engine& eng, const vector<num_t>& alpha) {

  size_t d = alpha.size();

  vector<num_t> sample( d );

  num_t sum = 0.0;

  for ( size_t i = 0; i < d; ++i ) {
    sample[i] = Gamma(eng,alpha[i],1);
    sum += sample[i];
  }

  assert( sum > 0 );

  for ( size_t i = 0; i < d; ++i ) {
    sample[i] /= sum;
  }

  return( sample );

}

int main() {

  // Number of threads
  size_t nThreads = 2;
  size_t nSamples = 10;

  // Engines for probability distributiona
  vector<Engine> eng(nThreads);

  // Thread initializers
  vector<num_t> dirAlpha(3);
  dirAlpha[0] = 1;
  dirAlpha[1] = 5;
  dirAlpha[2] = 3;

  vector<vector<Parameters> > parameters(nThreads,vector<Parameters>(nSamples,{1,2,3,4,dirAlpha}));
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
	 << " - Gamma(" << params[i].alpha << "," << params[i].beta << ")  => " << Gamma(*eng,params[i].alpha,params[i].beta) << endl
	 << " - Dirichlet(";
    utils::print(params[i].dirAlpha.begin(),params[i].dirAlpha.end(),',');
    cout << ") => (";
    vector<num_t> sample = Dirichlet(*eng,params[i].dirAlpha);
    utils::print(sample.begin(),sample.end(),',');
    cout << ")" << endl;
  }

  mtx.unlock();

}
