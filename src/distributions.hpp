#ifndef DISTRIBUTIONS_HPP
#define DISTRIBUTIONS_HPP

#include <cstdlib>
#include <vector>
#include <random>
#include <ctime>
#include "datadefs.hpp"

namespace distributions {

  //typedef std::mt19937 Engine;
  typedef std::ranlux_base_01 Engine;

  inline unsigned int getSeed() { return( clock() ); }

  datadefs::num_t normal(Engine& eng, const datadefs::num_t mu, const datadefs::num_t s2);
  
  datadefs::num_t gamma(Engine& eng, const datadefs::num_t shape, const datadefs::num_t scale);
  
  std::vector<datadefs::num_t> dirichlet(Engine& eng, const std::vector<datadefs::num_t>& alpha);

}

#endif
