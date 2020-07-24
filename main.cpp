/*
Copyright (c) 2020 S.E.B.

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
#include <fstream>
#include <sstream>
#include <ciso646>

#include "Randomizer.h"
#include "thirdparty/cxxopts/cxxopts.hpp"
#include "thirdparty/rv2d/rv2d.h"

// minmax
#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

std::string generateInstance(int nb_jobs, int nb_machines, int k, int nb_perturbations, double fixed_percentage)
{
  rv2d::RowVector2D<int> ptime_arr(nb_jobs, nb_machines);
  Randomizer random;

  // populate ptime_arr
  for (int i = 0; i < nb_jobs; ++i)
  {
    // sum of each row = k
    for (int j = 0; j < nb_machines; ++j)
    {
      ptime_arr(i, j) = k / nb_jobs;
    }
    // except diagonal
    if (i < nb_jobs and i < nb_machines)
      ptime_arr(i, i) += k % (nb_jobs-1);
  }

  // perturbations to to ptime_arr
  for (int i = 0; i < nb_perturbations; ++i)
  {
    // task 1
    int idx1 = 0;
    int idx2 = 0;
    // task 2
    int idx3 = 0;
    int idx4 = 0;

    // select randomly 2 taks randomely
    while (idx1 == idx3 or idx2 == idx4)
    {
      idx1 = static_cast<int>(random(0, (int)ptime_arr.rows()-1));
      idx2 = static_cast<int>(random(0, (int)ptime_arr.cols()-1));
      idx3 = static_cast<int>(random(0, (int)ptime_arr.rows()-1));
      idx4 = static_cast<int>(random(0, (int)ptime_arr.cols()-1));
    }
    // the maximum removable work is the minimum of the two durations
    // minus 1 to avoid creating tasks of length 0
    const int removable = MIN(ptime_arr(idx1, idx2), ptime_arr(idx3, idx4)) - 1;
    // fixed part that must be removed
    const int must_remove = static_cast<int>(trunc(removable * fixed_percentage));
    // randomely duration to be remove
    const int removed = must_remove + static_cast<int>(random(0, removable - must_remove));
    // substract from the removed frmo the first two tasks
    ptime_arr(idx1, idx2) = ptime_arr(idx1, idx2) - removed;
    // add the amount removed from the two first tasks,
    // to keep all line sums equal to K
    ptime_arr(idx1, idx4) = ptime_arr(idx1, idx4) + removed;
  }

  std::stringstream ss;
  ss << "// k = " << k
     << ", pert = " << nb_perturbations
     << ", fix = " << fixed_percentage
     << "%\n";

  ss << nb_jobs << " " << nb_machines << "\n";
  for (int i = 0; i < nb_jobs; ++i)
  {
    for (int j = 0; j < nb_machines; ++j)
    {
      ss << ptime_arr(i, j) << " ";
    }
    ss << "\n";
  }
  ss << "\n";

  return ss.str();
}

int main(int argc, char **argv)
{
  try
  {
    cxxopts::Options option("hossp", "hossp: Generate hard random instances of the open shop problem");
    option.add_options()("h,help", "This help message and exit")
                        ("k", "the k value", cxxopts::value<int>()->default_value("1000"))
                        ("o,out", "Enable stdout", cxxopts::value<bool>()->default_value("false"))
                        ("d,dir", "Output directory", cxxopts::value<std::string>()->default_value(""))
                        ("n,jobs", "number of jobs", cxxopts::value<int>()->default_value("4"))
                        ("m,machines", "number of machines", cxxopts::value<int>()->default_value("4"))
                        ("f,fix", "fixed percentage", cxxopts::value<double>()->default_value("0.95"))
                        ("p,pert", "number of perturbations", cxxopts::value<int>()->default_value("100"))
                        ("s,seed", "random seed", cxxopts::value<long>()->default_value("4294967"))
                        ("g,generate", "number of instances to generate", cxxopts::value<int>()->default_value("1"));
    
    if (argc == 1)
    {
      std::cout << option.help();
      exit(1);
    }
    auto option_parse = option.parse(argc, argv);
    if (option_parse.count("help"))
    {
      std::cout << option.help();
      exit(1);
    }
    if (option_parse["jobs"].as<int>() <= 0)
    {
      std::cerr << "Error: option jobs must be > 0" << std::endl;
      exit(1);
    }
    if (option_parse["machines"].as<int>() <= 0)
    {
      std::cerr << "Error: option machines must be > 0" << std::endl;
      exit(1);
    }
    if (option_parse["pert"].as<int>() < 0)
    {
      std::cerr << "Error: option pert must be > 0" << std::endl;
      exit(1);
    }
    if (option_parse["fix"].as<double>() < 0 or option_parse["fix"].as<double>() >= 1)
    {
      std::cerr << "Error: option fix must be > 0.0 and < 1.0" << std::endl;
      exit(1);
    }
    if (option_parse["k"].as<int>() < 0)
    {
      std::cerr << "Error: option k must be > 0" << std::endl;
      exit(1);
    }
    if (option_parse["g"].as<int>() <= 0)
    {
      std::cerr << "Error: option generate must be > 0" << std::endl;
      exit(1);
    }

    bool is_stdout = option_parse["out"].as<bool>();
    std::string out_dir = option_parse["dir"].as<std::string>();
    int nb_jobs = option_parse["jobs"].as<int>();
    int nb_machines = option_parse["machines"].as<int>();
    int nb_perturbations = option_parse["pert"].as<int>();
    double fixed_percentage = option_parse["fix"].as<double>();
    int k = option_parse["k"].as<int>();
    int nb_instances = option_parse["g"].as<int>();

    for (int i = 0; i < nb_instances; ++i)
    {
      // use random parameters if parameters are 0 or option not used =
      Randomizer random;
      if (option_parse["k"].count() == 0 or k == 0)
        k = static_cast<int>(random(nb_jobs * nb_machines, nb_jobs * nb_machines * 100));
      if (option_parse["pert"].count() == 0 or nb_perturbations == 0)
        nb_perturbations = static_cast<int>(random(nb_jobs * nb_jobs, nb_jobs * nb_jobs * nb_machines));
      if (option_parse["fix"].count() == 0 or fixed_percentage == 0)
        fixed_percentage = MAX(0.25, (double)random(RAND_MAX) / (RAND_MAX));

      const std::string instance = generateInstance(nb_jobs, nb_machines, k, nb_perturbations, fixed_percentage);

      if (is_stdout)
        std::cout << instance;
      if (not(is_stdout and out_dir.empty()))
      {
        std::string file_name = out_dir + "rand" + std::to_string(nb_jobs) + "_" + std::to_string(nb_machines) + "_" + std::to_string(i) + ".txt";
        std::ofstream trc_file;
        trc_file.open(file_name, std::ofstream::out);
        if (trc_file.is_open())
        {
          trc_file << instance;
          trc_file.close();
        }
        else
        {
          std::cerr << "Error: cannot create file " << file_name << std::endl;
          ;
          exit(1);
        }
      }
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  return 0;
}
