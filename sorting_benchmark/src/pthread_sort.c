#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define CUTOFF 10000  // Minimum size for parallel processing

typedef struct {
    int *arr;
    int left;
    int right;
    int *temp;
} sort_args_t;

// Merge two sorted subarrays
void merge(int *arr, int left, int mid, int right, int *temp) {
    int i = left;
    int j = mid + 1;
    int k = left;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    
    while (j <= right) {
        temp[k++] = arr[j++];
    }
    
    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

// Sequential merge sort
void merge_sort_sequential(int *arr, int left, int right, int *temp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        merge_sort_sequential(arr, left, mid, temp);
        merge_sort_sequential(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

// Thread function for parallel merge sort
void* merge_sort_thread(void *args) {
    sort_args_t *params = (sort_args_t*)args;
    merge_sort_sequential(params->arr, params->left, params->right, params->temp);
    return NULL;
}

// Parallel merge sort with pthreads
void merge_sort_parallel(int *arr, int left, int right, int *temp, int num_threads) {
    if (left >= right) {
        return;
    }
    
    if (right - left < CUTOFF || num_threads <= 1) {
        // Use sequential sort for small arrays or when no threads available
        merge_sort_sequential(arr, left, right, temp);
        return;
    }
    
    int mid = left + (right - left) / 2;
    
    // Create arguments for threads
    sort_args_t left_args = {arr, left, mid, temp};
    sort_args_t right_args = {arr, mid + 1, right, temp};
    
    pthread_t left_thread, right_thread;
    
    // Create threads for left and right halves
    pthread_create(&left_thread, NULL, merge_sort_thread, &left_args);
    pthread_create(&right_thread, NULL, merge_sort_thread, &right_args);
    
    // Wait for both threads to complete
    pthread_join(left_thread, NULL);
    pthread_join(right_thread, NULL);
    
    // Merge the sorted halves
    merge(arr, left, mid, right, temp);
}

// Read array from file
int* read_array(const char *filename, int *size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }
    
    int count = 0;
    int num;
    while (fscanf(file, "%d", &num) == 1) {
        count++;
    }
    
    int *arr = (int*)malloc(count * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
    
    rewind(file);
    for (int i = 0; i < count; i++) {
        fscanf(file, "%d", &arr[i]);
    }
    
    fclose(file);
    *size = count;
    return arr;
}

// Verify array is sorted
int is_sorted(int *arr, int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i-1]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    int size;
    int *arr = read_array(argv[1], &size);
    if (!arr) {
        return 1;
    }
    
    int num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Loaded %d numbers\n", size);
    printf("Using %d threads\n", num_threads);
    
    int *temp = (int*)malloc(size * sizeof(int));
    if (!temp) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(arr);
        return 1;
    }
    
    // Sort and measure time
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    merge_sort_parallel(arr, 0, size - 1, temp, num_threads);
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_spent = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1e9;
    
    // Verify
    if (is_sorted(arr, size)) {
        printf("✓ Array is sorted correctly\n");
    } else {
        printf("✗ Array is NOT sorted correctly!\n");
    }
    
    printf("Time taken: %.6f seconds\n", time_spent);
    
    // Print first and last 10 elements
    printf("\nFirst 10 elements: ");
    for (int i = 0; i < 10 && i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    printf("Last 10 elements:  ");
    for (int i = (size > 10 ? size - 10 : 0); i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    
    free(arr);
    free(temp);
    return 0;
}
