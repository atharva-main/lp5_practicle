#include <omp.h>
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;

void sequentialBubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;    
            }
        }
    }
}


void parallelBubbleSort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        // even indexed passes
        #pragma omp parallel for
        for (int j = 0; j < size - i - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
        // odd indexed passes
        #pragma omp parallel for
        for (int j = 1; j < size - i - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


int main() {
    const int SIZE = 100000;
    int arr[SIZE], arr_copy[SIZE];
    srand(time(0));

    // Fill the array with random numbers
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 100000;
        arr_copy[i] = arr[i]; // Copy for sequential sort
    }

    cout << "Original array size: " << endl;
    cout << SIZE << endl;

    // Sequential Bubble Sort
    double start_sequential = omp_get_wtime();
    sequentialBubbleSort(arr, SIZE);
    double end_sequential = omp_get_wtime();
    double time_sequential = end_sequential - start_sequential;
    cout << "Time taken (Sequential): " << time_sequential << " seconds" << endl;

    // Parallel Bubble Sort
    double start_parallel = omp_get_wtime();
    parallelBubbleSort(arr_copy, SIZE);
    double end_parallel = omp_get_wtime();
    double time_parallel = end_parallel - start_parallel;
    cout << "Time taken (Parallel): " << time_parallel << " seconds" << endl;

    return 0;
}