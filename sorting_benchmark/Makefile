# Makefile for Parallel Sorting Benchmark
# Compiles Sequential, OpenMP, and Pthreads versions of merge sort

CC = gcc
CFLAGS = -Wall -std=gnu99 -O2 -g
LDFLAGS = -pthread
OMPFLAGS = -fopenmp

# Directories
SRCDIR = src
BINDIR = bin
DATADIR = data

# Targets
all: sequential openmp pthread

# Sequential version
sequential: $(BINDIR)/seq_sort

$(BINDIR)/seq_sort: $(SRCDIR)/sequential_sort.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<

# OpenMP version
openmp: $(BINDIR)/omp_sort

$(BINDIR)/omp_sort: $(SRCDIR)/openmp_sort.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(OMPFLAGS) -o $@ $<

# Pthreads version
pthread: $(BINDIR)/pthread_sort

$(BINDIR)/pthread_sort: $(SRCDIR)/pthread_sort.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# Generate test data
data: $(DATADIR)/numbers_10k.txt $(DATADIR)/numbers_100k.txt \
      $(DATADIR)/numbers_1m.txt $(DATADIR)/numbers_10m.txt

$(DATADIR)/numbers_10k.txt:
	@mkdir -p $(DATADIR)
	python3 generate_random.py 10000 > $@

$(DATADIR)/numbers_100k.txt:
	@mkdir -p $(DATADIR)
	python3 generate_random.py 100000 > $@

$(DATADIR)/numbers_1m.txt:
	@mkdir -p $(DATADIR)
	python3 generate_random.py 1000000 > $@

$(DATADIR)/numbers_10m.txt:
	@mkdir -p $(DATADIR)
	python3 generate_random.py 10000000 > $@

# Clean build artifacts
clean:
	rm -rf $(BINDIR)/*.o $(BINDIR)/seq_sort $(BINDIR)/omp_sort $(BINDIR)/pthread_sort

# Clean everything including data
cleanall: clean
	rm -rf $(DATADIR)/*

# Phony targets
.PHONY: all sequential openmp pthread data clean cleanall

# Help
help:
	@echo "Available targets:"
	@echo "  all        - Build all implementations (sequential, openmp, pthread)"
	@echo "  sequential - Build sequential merge sort"
	@echo "  openmp     - Build OpenMP parallel merge sort"
	@echo "  pthread    - Build Pthreads parallel merge sort"
	@echo "  data       - Generate test data files"
	@echo "  clean      - Remove compiled binaries"
	@echo "  cleanall   - Remove binaries and data"
	@echo "  help       - Show this help message"
