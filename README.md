# Parallel Programming

Benchmarking sequential vs parallel implementations (OpenMP, Pthreads, MPI) for matrix multiplication and sorting.

---

## Structure
```
parallel-matrix-sorting/
├── matrix_multiplication/
│   ├── src/               # Sequential, OpenMP, Pthreads, MPI implementations
│   ├── Makefile          
│   ├── random_float_matrix.py
│   └── Test-Script.sh
│
└── sorting_benchmark/
    ├── src/               # Sequential, OpenMP, Pthreads merge sort
    ├── Makefile
    ├── generate_random.py
    └── run_benchmark.sh
```

---

## What I Did

### 1. Makefile
Learned and created Makefiles for automated compilation:
- Compiles all 4 versions (seq, omp, pthread, mpi) with one `make all` command
- Used `-O2` optimization flag
- Separate binaries for each implementation

### 2. Matrix Multiplication
Tested sizes: 10×10, 500×500, 2000×2000, 5000×5000

**Best Results (2000×2000):**
- Sequential: 77.41s
- OpenMP: 20.71s → **3.73x faster**
- Pthreads: 18.94s → **4.08x faster**
- MPI (4 processes): 23.58s → 3.28x faster

### 3. Sorting Algorithm
Chose **merge sort** (O(n log n), naturally parallelizable)

Tested sizes: 10k, 100k, 1M, 10M elements

**Best Results (10M elements):**
- Sequential: 1.89s
- OpenMP: 0.56s → **3.38x faster**
- Pthreads: 0.95s → 1.99x faster

---

## How to Run

### Matrix Multiplication
```bash
cd matrix_multiplication
make all

# Generate test data
python3 random_float_matrix.py 500 500 > data/mat_a.txt
python3 random_float_matrix.py 500 500 > data/mat_b.txt

# Run
bin/seq data/mat_a.txt data/mat_b.txt
bin/omp data/mat_a.txt data/mat_b.txt
bin/thread2 data/mat_a.txt data/mat_b.txt
mpirun -np 4 bin/mpi data/mat_a.txt data/mat_b.txt
```

### Sorting
```bash
cd sorting_benchmark
make all

# Generate test data
python3 generate_random.py 1000000 > data/numbers.txt

# Run
bin/seq_sort data/numbers.txt
bin/omp_sort data/numbers.txt
bin/pthread_sort data/numbers.txt
```

---

## Key Findings

1. **Small datasets**: Parallel overhead hurts performance (10×10 matrices slower)
2. **Large datasets**: Huge speedups (up to 4x)
3. **OpenMP**: Most consistent, easiest to implement
4. **Pthreads**: Best performance when optimized (2000×2000 matrix)
5. **MPI**: Higher overhead, better for distributed systems

---

## System Specs

- Ubuntu 24.04
- 8 cores
- GCC 13.3.0
- Compiled with `-O2`

---

## Credits

Matrix multiplication code based on [mperlet's implementation](https://github.com/mperlet/matrix_multiplication) (MIT License).

