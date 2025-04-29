import numpy as np
from numba import cuda
import time

# Size of vectors
N = 10_000_000

# Create random vectors on CPU first
A = np.arange(N, dtype=np.float32)
B = np.arange(N, dtype=np.float32) * 2
C = np.zeros(N, dtype=np.float32)

# Define CUDA kernel
@cuda.jit
def vector_add(A, B, C):
    idx = cuda.grid(1)  # get global thread ID
    if idx < A.size:
        C[idx] = A[idx] + B[idx]

# Allocate arrays on GPU
d_A = cuda.to_device(A)
d_B = cuda.to_device(B)
d_C = cuda.device_array_like(C)

threads_per_block = 256
blocks_per_grid = (N + threads_per_block - 1) // threads_per_block

start = time.time()

# Launch kernel
vector_add[blocks_per_grid, threads_per_block](d_A, d_B, d_C)

# Copy result back to CPU
d_C.copy_to_host(C)

end = time.time()

print(f"Vector addition completed for {N} elements.")
print(f"Time taken (GPU using Numba): {end - start:.5f} seconds")

print("First 5 results:", C[:5])
