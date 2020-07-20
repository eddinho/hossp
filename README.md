# Hard Open Shop Scheduling Problems Generator
A random generator of hard open shop scheduling problems based on Guéret and Prins (1999).

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
  ossp.ai_rand [OPTION...]

  -h, --help          This help message and exit
  -k, arg             the k value (default: 1000)
  -o, --out           Enable stdout
  -d, --dir arg       Output directory (default: "")
  -n, --jobs arg      number of jobs (default: 4)
  -m, --machines arg  number of machines (default: 4)
  -f, --fix arg       fixed percentage (default: 0.95)
  -p, --pert arg      number of perturbations (default: 100)
  -s, --seed arg      random seed (default: 4294967)
  -g, --generate arg  number of instances to generate (default: 1)
```

The following command generates 5 random instances, of 5 jobs and 5 machines and outputs the instances to stdout (-o).

To create the instances files rand5_5_1.txt to rand5_5_5.txt in current folder, use option (-d ./) or remove option -o

```bash
./build/bin/hossp.exe -n 5 -m 5 -g 5 -o

// k = 1088, pert = 51, fix = 0.25%
5 5
18 371 405 264 30
164 265 116 518 25
331 261 138 277 81
263 388 281 104 52
40 426 393 151 78

// k = 1218, pert = 67, fix = 0.143376%
5 5
32 280 229 628 49
226 206 324 221 241
164 498 167 228 161
407 90 102 601 18
284 7 312 581 34

// k = 2145, pert = 48, fix = 0.25%
5 5
301 184 29 311 1320
237 28 118 813 949
127 381 411 1022 204
140 150 727 219 909
136 309 365 551 784

// k = 186, pert = 48, fix = 0.15714%
5 5
99 2 6 55 24
89 50 32 12 3
47 1 55 42 41
65 28 6 46 41
29 27 34 40 56

// k = 1975, pert = 59, fix = 0.309732%
5 5
872 225 652 210 16
277 120 1483 45 50
632 235 531 11 566
843 32 1086 3 11
914 2 737 170 152
```

## Reference

Guéret, C., & Prins, C. (1999). A new lower bound for the open-shop problem. Annals of Operations Research, 92, 165–183. https://doi.org/10.1023/A:1018930613891
