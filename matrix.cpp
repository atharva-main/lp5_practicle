#include <iostream>
#include <omp.h>
#include <cstdlib> // for rand()

using namespace std;

const int N = 500; // matrix size

void multiplyMatrices(int A[N][N], int B[N][N], int C[N][N]) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    static int A[N][N], B[N][N], C[N][N];

    // Initialize matrices with random numbers
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    double start = omp_get_wtime();
    multiplyMatrices(A, B, C);
    double end = omp_get_wtime();

    cout << "Matrix Multiplication Completed." << endl;
    cout << "Time taken: " << (end - start) << " seconds." << endl;

    return 0;
}
