#include <iostream>
#include <omp.h>
#include <cstdlib> // for rand()
#include <chrono>
using namespace std;

// Merge two halves
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// Sequential Merge Sort
void mergeSortSequential(int arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void mergeSortParallel(int arr[], int left, int right, int depth = 0) {
    if (left < right) {
        int mid = (left + right) / 2;

        if (depth <= 3) {
            #pragma omp parallel sections
            {
                #pragma omp section
                mergeSortParallel(arr, left, mid, depth + 1);

                #pragma omp section
                mergeSortParallel(arr, mid + 1, right, depth + 1);
            }
        } else {
            mergeSortSequential(arr, left, mid);
            mergeSortSequential(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

int main() {
    const int SIZE = 100000;
    int* arr = new int[SIZE];
    int* arr_copy = new int[SIZE];

    // Initialize array with random values
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = rand();
        arr_copy[i] = arr[i];
    }

    // Sequential Merge Sort Timing
    double start = omp_get_wtime();
    mergeSortSequential(arr_copy, 0, SIZE - 1);
    double end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << (end - start) << " seconds\n";

    // Parallel Merge Sort Timing
    start = omp_get_wtime();
    mergeSortParallel(arr, 0, SIZE - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << (end - start) << " seconds\n";

    delete[] arr;
    delete[] arr_copy;

    return 0;
}
