[![main](https://github.com/eddinho/hossp/actions/workflows/main.yml/badge.svg)](https://github.com/eddinho/hossp/actions/workflows/main.yml)

# A Hard Open Shop Scheduling Problems Generator
A random generator of hard open shop scheduling problems based on Guéret and Prins (1999).

[Read more](./doc/hossp.pdf).

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
  -n, --jobs arg      number of jobs (default: 4)
  -m, --machines arg  number of machines (default: 4)
  -k arg              the k value, =rand(n*m,n*m*100) if 0 (default: 0)
  -f, --fix arg       fixed percentage, =rand(0,1) if 0 (default: 0)
  -p, --pert arg      number of perturbations, =rand(n*m,n*m^2) if 0 (default: 0)
  -g, --generate arg  number of instances to generate (default: 1)
  -o, --out           Enable stdout
  -d, --dir arg       Output directory (default: "")
```

The following command generates 5 random instances, of 5 jobs and 5 machines and outputs the instances to stdout (-o).

```bash
./build/bin/hossp.exe -n 5 -m 5 -g 2 -o

// k = 491, pert = 83, fix = 0.627064, seed = 634927729, lb = 491
5 5
11 249 153 3 75
222 13 212 35 9
2 14 118 231 126
1 173 5 187 125
255 42 3 35 156

// k = 644, pert = 82, fix = 0.0872524, seed = 931827985, lb = 644
5 5
151 86 368 12 27
65 176 267 39 97
266 1 5 251 121
91 1 2 341 209
71 380 2 1 190
```

## Reference

Guéret, C., & Prins, C. (1999). A new lower bound for the open-shop problem. Annals of Operations Research, 92, 165–183. https://doi.org/10.1023/A:1018930613891
