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
#include <iomanip>

#include "Randomizer.h"
#include "thirdparty/cxxopts/cxxopts.hpp"

#ifndef __has_include
static_assert(false, "__has_include not supported");
#else
#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#elif __has_include(<boost/filesystem.hpp>)
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif
#endif

// minmax
#ifndef MIN
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

std::pair<std::string, std::string> generateInstance(int i, int nb_jobs, int nb_machines, int k, int nb_perturbations, double fixed_percentage, long seed)
{

  // calculate the task index in ptime_arr
  auto calTaskIdx = [&](int row_idx, int col_idx) {
    return row_idx * nb_machines + col_idx;
  };

  std::string logdir = "./log/";
  fs::create_directories(logdir);

  std::vector<int> ptime_arr(nb_jobs * nb_machines);
  Randomizer random(seed);

  float workload = 0.0;
  bool is_random_hard = false;

  if (k == 0 or nb_perturbations == 0 or fixed_percentage == 0)
    is_random_hard = true;

  if (is_random_hard)
  {
    // use these random parameters if parameters are 0
    k = static_cast<int>(random(nb_jobs * nb_machines, nb_jobs * nb_machines * 100));
    nb_perturbations = static_cast<int>(random(nb_jobs * nb_jobs, nb_jobs * nb_jobs * nb_machines));
    fixed_percentage = (double)random(RAND_MAX) / (RAND_MAX);
  }


  // square problems
  if(nb_jobs == nb_machines) 
  {
    for (int i = 0; i < nb_jobs; ++i)
    {
      // sum of each row = k
      for (int j = 0; j < nb_machines; ++j)
      {
        ptime_arr[calTaskIdx(i, j)] = int(floor(k / nb_machines));
      }
      ptime_arr[calTaskIdx(i, i)] += k % (nb_machines);
    }
  }
  // for rectangle problems, make sur that sum of all row is equal to k
  // devide the rec into a square and add k mod m to a square sub matrix 
  // at a time ensuring all sum of row elem is equal to k 
  else 
  {
    int inc = 0;
    // populate ptime_arr and apply the mod on the main diagonal
    for (int i = 0; i < nb_jobs; ++i)
    {
      // sum of each row = k
      for (int j = 0; j < nb_machines; ++j)
      {
        ptime_arr[calTaskIdx(i, j)] = int(floor(k / nb_machines));
        if (inc - i == j)
          ptime_arr[calTaskIdx(i, j)] += k % (nb_machines);
      }
      if (i == inc)
        inc += MIN(nb_jobs, nb_machines);;
    }    
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
      idx1 = static_cast<int>(random(0, nb_jobs - 1));
      idx2 = static_cast<int>(random(0, nb_machines - 1));
      idx3 = static_cast<int>(random(0, nb_jobs - 1));
      idx4 = static_cast<int>(random(0, nb_machines - 1));
    }
    // the maximum removable work is the minimum of the two durations
    // minus 1 to avoid creating tasks of length 0
    const int removable = MIN(ptime_arr[calTaskIdx(idx1, idx2)], ptime_arr[calTaskIdx(idx3, idx4)]) - 1;
    // fixed part that must be removed
    const int must_remove = static_cast<int>(trunc(removable * fixed_percentage));
    // randomely duration to be remove
    const int removed = must_remove + static_cast<int>(random(0, removable - must_remove));
    // substract from the removed frmo the first two tasks
    ptime_arr[calTaskIdx(idx1, idx2)] -= removed;
    ptime_arr[calTaskIdx(idx3, idx4)] -= removed;

    // add the amount removed from the two first tasks,
    // to keep all line sums equal to K
    ptime_arr[calTaskIdx(idx1, idx4)] += removed;
    ptime_arr[calTaskIdx(idx3, idx2)] += removed;
  }

  // calculate the workload of the instance
  // classical lower bound
  std::vector<int> machine(nb_machines);
  float lower_bound = 0;
  float pttot = 0;
  for (int i = 0; i < nb_jobs; i++)
  {
    int job = 0;
    for (int j = 0; j < nb_machines; j++)
    {
      // add processing time on scheduled machine
      machine[j] += ptime_arr[calTaskIdx(i, j)];
      job += ptime_arr[calTaskIdx(i, j)];
      pttot += ptime_arr[calTaskIdx(i, j)];
    }
    lower_bound = MAX(lower_bound, job);
  }
  for (int j = 0; j < nb_machines; j++)
  {
    lower_bound = MAX(lower_bound, machine[j]);
  }
  workload = float((pttot / (nb_machines * lower_bound)));

  // create an instance name
  std::stringstream ss_name;
  ss_name << "rand_"
          << std::setfill('0') << std::setw(2) << nb_jobs
          << "_"
          << std::setfill('0') << std::setw(2) << nb_machines
          << "_"
          << std::setfill('0') << std::setw(2) << i;

  // print instance statistics
  std::ofstream out;
  std::string filename = logdir + "/stats.txt";
  out.open(filename, std::ofstream::out | std::ofstream::app);
  if (out.is_open())
  {
    out << ss_name.str()
        << ","
        << k
        << ","
        << nb_perturbations
        << ","
        << fixed_percentage
        << ","
        << random.seed()        
        << ","
        << lower_bound
        << ","
        << workload                
        << "\n";
  }
  out.close();

  std::stringstream ss_instance;
  ss_instance << "// k = " << k
              << ", pert = " << nb_perturbations
              << ", fix = " << fixed_percentage
              << ", seed = " << random.seed()
              << ", lb = " << lower_bound
              << ", workload = " << workload
              << "\n";

  ss_instance << nb_jobs << " " << nb_machines << "\n";
  for (int i = 0; i < nb_jobs; ++i)
  {
    for (int j = 0; j < nb_machines; ++j)
    {
      ss_instance << ptime_arr[calTaskIdx(i, j)] << " ";
    }
    ss_instance << "\n";
  }
  ss_instance << "\n";

  return std::make_pair(ss_name.str(), ss_instance.str());
}

int main(int argc, char **argv)
{
  try
  {
    cxxopts::Options option("hossp", "hossp: Generate hard random instances of the open shop problem");
    option.add_options()("h,help", "This help message and exit")
                        ("n,jobs", "number of jobs", cxxopts::value<int>()->default_value("4"))
                        ("m,machines", "number of machines", cxxopts::value<int>()->default_value("4"))
                        ("k", "the k value, =rand(n*m,n*m*100) if 0", cxxopts::value<int>()->default_value("0"))                     
                        ("f,fix", "fixed percentage, =rand(0,1) if 0", cxxopts::value<double>()->default_value("0"))
                        ("p,pert", "number of perturbations, =rand(n*m,n*m^2) if 0", cxxopts::value<int>()->default_value("0"))
                        ("g,generate", "number of instances to generate", cxxopts::value<int>()->default_value("1"))                        
                        ("o,out", "Enable stdout", cxxopts::value<bool>()->default_value("false"))
                        ("d,dir", "Output directory", cxxopts::value<std::string>()->default_value(""))                           
                        ("s,seed", "random seed, =rand if 0", cxxopts::value<long>()->default_value("0"))
                        ;

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
      std::cerr << "Error: option pert must be >= 0" << std::endl;
      exit(1);
    }
    if (option_parse["fix"].as<double>() < 0 or option_parse["fix"].as<double>() > 1)
    {
      std::cerr << "Error: option fix must be >= 0.0 and <= 1.0" << std::endl;
      exit(1);
    }
    if (option_parse["k"].as<int>() < 0)
    {
      std::cerr << "Error: option k must be >= 0" << std::endl;
      exit(1);
    }
    if (option_parse["g"].as<int>() <= 0)
    {
      std::cerr << "Error: option generate must be > 0" << std::endl;
      exit(1);
    }

    if (option_parse["seed"].as<long>() < 0)
    {
      std::cerr << "Error: option seed must be >= 0" << std::endl;
      exit(1);
    }

    if (option_parse["g"].as<int>() > 1 and option_parse["seed"].as<long>() > 0)
    {
      std::cerr << "Error: cannot define a random seed to generate multiple instances." << std::endl;
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
    int seed = option_parse["seed"].as<long>();

    for (int i = 0; i < nb_instances; ++i)
    {

      const std::pair<std::string, std::string> instance = generateInstance(i, nb_jobs, nb_machines, k, nb_perturbations, fixed_percentage, seed);

      if (is_stdout)
        std::cout << instance.second;
      if (not(is_stdout and out_dir.empty()))
      {
        std::string file_name = out_dir + instance.first + ".txt";
        std::ofstream trc_file;
        trc_file.open(file_name, std::ofstream::out);
        if (trc_file.is_open())
        {
          trc_file << instance.second;
          trc_file.close();
        }
        else
        {
          std::cerr << "Error: cannot create file " << file_name << std::endl;          
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
