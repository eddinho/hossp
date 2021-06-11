
# A Hard Open Shop Scheduling Problems Generator

A random generator of hard open shop scheduling problems based on Guéret and Prins (1999).

[Read more here](./doc/hossp.pdf).

[![Build Status](https://github.com/eddinho/hossp/actions/workflows/build.yml/badge.svg)](https://github.com/eddinho/hossp/actions?query=workflow%3ABuild)
## Installation

```bash
git clone https://github.com/eddinho/hossp.git
cd hossp
mkdir build && cd build && cmake ..
cmake --build .
```
hossp will be located in the folder ./build/bin

## Usage

Open shop instance problem format:

```c++
// a comment line
# also a comment line

// jobs machines
3 3
// processing times
3 8 49
52 12 5
20 25 13
```

Generating problem instances

```bash
hossp: Generate hard random instances of the open shop problem
Usage:
  hossp [OPTION...]

  -h, --help          This help message and exit
  -n, --jobs arg      number of jobs (default: 0)
  -m, --machines arg  number of machines (default: 0)
  -k arg              the k value, =rand(n*m,n*m*100) if 0 (default: 0)
  -f, --fix arg       fixed percentage, =rand(0,1) if 0 (default: 0)
  -p, --pert arg      number of perturbations, =rand(n*m,n*m^2) if 0
                      (default: 0)
  -g, --generate arg  number of instances to generate (default: 1)
  -o, --out           Enable stdout
  -d, --dir arg       Output directory (default: "")
  -s, --seed arg      random seed. (default: 0)
```

The following command generates 5 random instances, of 5 jobs and 5 machines and outputs the instances to stdout (-o).

```bash
./build/bin/hossp.exe -n 5 -m 5 -g 2 -o

// k = 1412, pert = 121, fix = 0.80282, seed = 1993166717, lb = 1412, workload = 1
5 5
146 684 452 103 27
1 6 304 55 1046
420 648 23 1 320
832 4 161 414 1
13 70 472 839 18

// k = 1412, pert = 121, fix = 0.80282, seed = 1280406316, lb = 1412, workload = 1
5 5
1051 22 13 168 158
146 126 1051 87 2
17 1043 19 332 1
17 6 163 9 1217
181 215 166 816 34
```

## Reference

Guéret, C., & Prins, C. (1999). A new lower bound for the open-shop problem. Annals of Operations Research, 92, 165–183. https://doi.org/10.1023/A:1018930613891
