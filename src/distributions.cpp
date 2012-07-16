#include "distributions.hpp"
#include <cassert>


using namespace std;
using datadefs::num_t;
using distributions::Engine;

num_t distributions::normal(Engine& eng, const num_t mu, const num_t s2) {
  normal_distribution<num_t> normrand(mu,sqrt(s2));
  return( normrand(eng) );
}

num_t distributions::gamma(Engine& eng, const num_t alpha, const num_t beta) {
  
  // A random number generator Gamma(alpha,1)
  gamma_distribution<num_t> gamrand(alpha);
  
  // Get a random number from Gamma(alpha,1)
  // and scale it to Gamma(alpha,beta)
  return( gamrand(eng) / beta );
}

vector<num_t> distributions::dirichlet(Engine& eng, const vector<num_t>& alpha) {

  size_t d = alpha.size();

  vector<num_t> sample( d );

  num_t sum = 0.0;

  for ( size_t i = 0; i < d; ++i ) {
    sample[i] = distributions::gamma(eng,alpha[i],1);
    sum += sample[i];
  }

  assert( sum > 0 );

  for ( size_t i = 0; i < d; ++i ) {
    sample[i] /= sum;
  }

  return( sample );

}





