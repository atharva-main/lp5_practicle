#include <iostream>
#include <omp.h>
#include <climits>
#include <cstdlib>
#include <ctime>


using namespace std;


int main() {
    const int SIZE = 1000;
    int arr[SIZE];

    srand(time(0));

    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 1000;
    }

    int min_val = INT_MAX;
    int max_val = INT_MIN;
    long long sum = 0;

    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
    for (int i = 0; i < SIZE; i++) {
        if (arr[i] < min_val)
            min_val = arr[i];
        if (arr[i] > max_val)
            max_val = arr[i];
        sum += arr[i];
    }

    double average = (double)sum / SIZE;

    // Output the results
    cout << "Min: " << min_val << endl;
    cout << "Max: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << average << endl;

    return 0;
}