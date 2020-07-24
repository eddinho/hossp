/*
Copyright (c) 2020 S.E. B.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
