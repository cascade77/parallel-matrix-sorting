#!/bin/bash

# Complete Sorting Benchmark Script
# Tests Sequential, OpenMP, and Pthreads merge sort implementations

echo "=========================================="
echo "Parallel Sorting Benchmark"
echo "Algorithm: Merge Sort"
echo "Date: $(date)"
echo "=========================================="
echo ""

# Create results directory
mkdir -p results

# Results file
RESULTS_FILE="results/sorting_benchmark_$(date +%Y%m%d_%H%M%S).txt"

# Header for results file
{
    echo "Parallel Sorting Benchmark Results"
    echo "Algorithm: Merge Sort"
    echo "Date: $(date)"
    echo "System: $(uname -a)"
    echo "CPU cores: $(nproc)"
    echo "========================================"
    echo ""
} > "$RESULTS_FILE"

# Dataset sizes
SIZES=("10k" "100k" "1m" "10m")

# Function to run and time a command
run_benchmark() {
    local name=$1
    local cmd=$2
    local datafile=$3
    
    echo -n "  $name: "
    
    # Run and capture timing
    local output=$($cmd $datafile 2>&1)
    local time_taken=$(echo "$output" | grep "Time taken:" | awk '{print $3}')
    
    if [ -z "$time_taken" ]; then
        echo "ERROR"
        echo "$name: ERROR" >> "$RESULTS_FILE"
        return 1
    fi
    
    echo "$time_taken seconds"
    echo "$name: $time_taken seconds" >> "$RESULTS_FILE"
    
    # Verify sorting
    local sorted=$(echo "$output" | grep "sorted correctly")
    if [ -z "$sorted" ]; then
        echo "  WARNING: Sorting verification failed!"
    fi
}

# Run benchmarks for each size
for size in "${SIZES[@]}"; do
    datafile="data/numbers_${size}.txt"
    
    if [ ! -f "$datafile" ]; then
        echo "ERROR: $datafile not found. Run 'make data' first."
        exit 1
    fi
    
    # Get element count
    count=$(wc -l < "$datafile")
    
    echo "=========================================="
    echo "Dataset: $size ($count elements)"
    echo "=========================================="
    
    {
        echo ""
        echo "Dataset: $size ($count elements)"
        echo "----------------------------------------"
    } >> "$RESULTS_FILE"
    
    # Run benchmarks
    run_benchmark "Sequential" "bin/seq_sort" "$datafile"
    seq_time=$(tail -1 "$RESULTS_FILE" | awk '{print $2}')
    
    run_benchmark "OpenMP    " "bin/omp_sort" "$datafile"
    omp_time=$(tail -1 "$RESULTS_FILE" | awk '{print $2}')
    
    run_benchmark "Pthreads  " "bin/pthread_sort" "$datafile"
    pthread_time=$(tail -1 "$RESULTS_FILE" | awk '{print $2}')
    
    # Calculate speedups
    if command -v bc &> /dev/null; then
        echo "" | tee -a "$RESULTS_FILE"
        echo "Speedup vs Sequential:" | tee -a "$RESULTS_FILE"
        
        if [[ "$omp_time" != "ERROR" ]] && [[ "$seq_time" != "ERROR" ]]; then
            speedup=$(echo "scale=2; $seq_time / $omp_time" | bc)
            echo "  OpenMP:   ${speedup}x" | tee -a "$RESULTS_FILE"
        fi
        
        if [[ "$pthread_time" != "ERROR" ]] && [[ "$seq_time" != "ERROR" ]]; then
            speedup=$(echo "scale=2; $seq_time / $pthread_time" | bc)
            echo "  Pthreads: ${speedup}x" | tee -a "$RESULTS_FILE"
        fi
    fi
    
    echo ""
done

echo "=========================================="
echo "Benchmark Complete!"
echo "=========================================="
echo ""
echo "Results saved to: $RESULTS_FILE"
echo ""

# Generate summary table
echo "Summary Table:" | tee -a "$RESULTS_FILE"
echo "----------------------------------------" | tee -a "$RESULTS_FILE"
printf "%-10s %-12s %-12s %-12s\n" "Dataset" "Sequential" "OpenMP" "Pthreads" | tee -a "$RESULTS_FILE"
echo "----------------------------------------" | tee -a "$RESULTS_FILE"

for size in "${SIZES[@]}"; do
    seq=$(grep -A 10 "Dataset: $size" "$RESULTS_FILE" | grep "Sequential:" | awk '{print $2}')
    omp=$(grep -A 10 "Dataset: $size" "$RESULTS_FILE" | grep "OpenMP:" | awk '{print $2}')
    pth=$(grep -A 10 "Dataset: $size" "$RESULTS_FILE" | grep "Pthreads:" | awk '{print $2}')
    
    printf "%-10s %-12s %-12s %-12s\n" "$size" "$seq" "$omp" "$pth" | tee -a "$RESULTS_FILE"
done

echo "----------------------------------------" | tee -a "$RESULTS_FILE"
