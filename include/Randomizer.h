#pragma once

#include <random>

class Randomizer
{
public:
  Randomizer()
  {
    (*this)(0, RAND_MAX);
  }

  double operator()(int min, int max, long seed = std::random_device{}())
  {
    setSeed(seed);
    distrib_ = std::uniform_int_distribution<int>(min, max);
    return (*this)();
  }

  double operator()(int max)
  {
    return (*this)(0, max);
  }

  double operator()()
  {
    return distrib_(gen_);
  }

  void setSeed(long seed)
  {
    gen_.seed(seed);
  }
private:
  std::mt19937 gen_;
  std::uniform_int_distribution<int> distrib_ ;
};