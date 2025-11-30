#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
void merge_sort(int *arr, int left, int right, int *temp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        merge_sort(arr, left, mid, temp);
        merge_sort(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

// Read array from file
int* read_array(const char *filename, int *size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }
    
    // Count numbers
    int count = 0;
    int num;
    while (fscanf(file, "%d", &num) == 1) {
        count++;
    }
    
    // Allocate and read
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
    
    printf("Loaded %d numbers\n", size);
    
    // Allocate temporary array for merging
    int *temp = (int*)malloc(size * sizeof(int));
    if (!temp) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(arr);
        return 1;
    }
    
    // Sort and measure time
    clock_t start = clock();
    merge_sort(arr, 0, size - 1, temp);
    clock_t end = clock();
    
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
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
