#ifndef DISTRIBUTIONS_HPP
#define DISTRIBUTIONS_HPP

#include <cstdlib>
#include <vector>
#include <random>
#include "datadefs.hpp"

namespace distributions {

  typedef std::ranlux_base_01 Engine;

  datadefs::num_t normal(Engine& eng, const datadefs::num_t mu, const datadefs::num_t s2);
  
  datadefs::num_t gamma(Engine& eng, const datadefs::num_t alpha, const datadefs::num_t beta);
  
  std::vector<datadefs::num_t> dirichlet(Engine& eng, const std::vector<datadefs::num_t>& alpha);

}

#endif
