#include <cstdlib>
#include <string>
#include <thread>
#include <iostream>
#include <vector>
#include <cassert>
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

  // x(t,i,c) is the pure expression of gene i in tissue t under experiment c
  vector<vector<vector<num_t> > > x;

  // lambda(i) is the inverse of variance of gene i
  vector<num_t> lambda;

  // These are nonsense parameters
  num_t alpha; 
  num_t beta;
  vector<num_t> dirAlpha;

};

void dsection_mcmc_samples(distributions::Engine* eng, const Parameters& initParams, const size_t nBurnIn, const size_t nSamples);

int main(const int argc, char* const argv[]) {

  assert( argc == 4 );

  // Store the start time (in clock cycles) just before the analysis
  //clock_t clockStart( clock() );

  clock_t timeStart = time(0);

  // Number of threads and samples for MCMC
  size_t nThreads = static_cast<size_t>(atoi(argv[1]));
  size_t nBurnIn = static_cast<size_t>(atoi(argv[2]));
  size_t nSamples = static_cast<size_t>(atoi(argv[3]));

  // Number of cell types and other parameters, to-be derived from data
  size_t nCellTypes = 3;
  size_t nGenes = 10000;
  size_t nExperiments = 3;

  cout << endl
       << "MCMC set-up:" << endl
       << " - " << nThreads << " threads" << endl
       << " - " << nBurnIn << " burn-in" << endl
       << " - " << nSamples << " samples" << endl << endl;

  // Engines for probability distributiona
  vector<distributions::Engine> eng(nThreads);

  // Initial parameters for Dirichlet distribution
  vector<num_t> dirAlpha(3);
  dirAlpha[0] = 1;
  dirAlpha[1] = 5;
  dirAlpha[2] = 3;

  // TEMPORARY: engine for initialization
  distributions::Engine initEngine;
  initEngine.seed(1000);

  // Initialize x(t,i,c)'s;
  vector<vector<vector<num_t> > > x(nCellTypes,vector<vector<num_t> >(nGenes,vector<num_t>(nExperiments)));
  //vector<num_t> x(nCellTypes*nGenes*nExperiments);
  for ( size_t t = 0; t < nCellTypes; ++t ) {
    for ( size_t i = 0; i < nGenes; ++i ) {
      for ( size_t c = 0; c < nExperiments; ++c ) {
	// Just random initialization for now
	x[t][i][c] = distributions::normal(initEngine,2,4);
      }
    }
  }

  // Initialize lambda(i)'s
  vector<num_t> lambda(nGenes);
  for ( size_t i = 0; i < nGenes; ++i ) {
    // Just random initialization for now
    lambda[i] = distributions::gamma(initEngine,2,2);
  }

  // Place initialization into the Parameter struct
  vector<Parameters> initParams(nThreads,{x,lambda,3,4,dirAlpha});

  // Initialize engines for random number generators
  for ( unsigned int threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    eng[threadIdx].seed(threadIdx);
  }
  
  // Generate threads
  thread thr[nThreads];
  for ( unsigned int threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    thr[threadIdx] = thread(dsection_mcmc_samples,&eng[threadIdx],initParams[threadIdx],nBurnIn,nSamples);
  }  
    
  // Join results
  for ( size_t threadIdx = 0; threadIdx < nThreads; ++threadIdx ) {
    thr[threadIdx].join();
  }

  cout << endl
       << time(0) - timeStart << " seconds elapsed" << endl;

  return( EXIT_SUCCESS );
}

num_t update_x_tic(const vector<vector<num_t> >& y_c, const num_t lambda_i, const vector<vector<num_t> >& p_c) {

  return( datadefs::NUM_NAN ); 

}


void dsection_mcmc_samples(distributions::Engine* eng, const Parameters& initParams, const size_t nBurnIn, const size_t nSamples) {

  Parameters params = initParams;

  size_t nCellTypes = initParams.x.size();
  size_t nGenes = initParams.x.begin()->size();
  size_t nExperiments = initParams.x.begin()->begin()->size();

  size_t nTotSamples = nBurnIn + nSamples;

  if ( DEBUG ) {    
    mtx.lock();
    for ( size_t s = 0; s < nTotSamples; ++s ) {
      for ( size_t t = 0; t < nCellTypes; ++t ) {
	for ( size_t i = 0; i < nGenes; ++i ) {
	  for ( size_t c = 0; c < nExperiments; ++c ) {
	    cout << "ENGINE " << eng << ", SAMPLE " << s << endl
		 << " - Normal(" << params.x[t][i][c] << "," << 1.0/params.lambda[i] << ") => " << distributions::normal(*eng,params.x[t][i][c],1.0/params.lambda[i]) << endl
		 << " - Gamma(" << params.alpha << "," << params.beta << ")  => " << distributions::gamma(*eng,params.alpha,params.beta) << endl
		 << " - Dirichlet(";
	    utils::write(cout,params.dirAlpha.begin(),params.dirAlpha.end(),',');
	    cout << ") => (";
	    vector<num_t> sample = distributions::dirichlet(*eng,params.dirAlpha);
	    utils::write(cout,sample.begin(),sample.end(),',');
	    cout << ")" << endl;
	  }
	}
      }
    }
    mtx.unlock();
  } else {
    for ( size_t s = 0; s < nTotSamples; ++s ) {
      for ( size_t t = 0; t < nCellTypes; ++t ) {
	for ( size_t i = 0; i < nGenes; ++i ) {
	  for ( size_t c = 0; c < nExperiments; ++c ) {
	    num_t foo1 = distributions::normal(*eng,params.x[t][i][c],1.0/params.lambda[i]);
	    num_t foo2 = distributions::gamma(*eng,params.alpha,params.beta);
	    vector<num_t> foo3 = distributions::dirichlet(*eng,params.dirAlpha);
	  }
	}
      }
    }
  }
  
  
}
