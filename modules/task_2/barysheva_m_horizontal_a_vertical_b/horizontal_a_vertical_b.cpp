// Copyright 2021 Barysheva Maria
#include "../../../modules/task_2/barysheva_m_horizontal_a_vertical_b/horizontal_a_vertical_b.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#define A_TAG 1
#define B_TAG 2

vector<int> getRandomMatrix(int n, int m) {
  std::random_device dev;
  std::mt19937 gen(dev());
  vector<int> matrix(n * m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      matrix[i * m + j] = gen() % 100;
    }
  }
  return matrix;
}

vector<int> SequentialMatrixMultiplication(vector<int> A, vector<int> B,
                                           int A_n, int A_m, int B_m) {
  int B_n = A_m;

  int matrix_size = A_n * B_m;
  vector<int> matrix(matrix_size, 0);

  for (int i = 0; i < A_n; i++) {
    for (int j = 0; j < B_m; j++) {
      for (int k = 0; k < A_m; k++) {
        matrix[i * B_m + j] += A[i * B_n + k] * B[k * B_m + j];
      }
    }
  }
  return matrix;
}

vector<int> ParallelMatrixMultiplication(vector<int> A, vector<int> B, int A_n,
                                         int A_m) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (A_n < size || size == 1) {
    if (rank == 0) {
      return SequentialMatrixMultiplication(A, B, A_n, A_m, A_n);
    } else {
      return vector<int>{};
    }
  }

  int B_n = A_m;
  int B_m = A_n;
  int delta = A_n / size;
  int rem = A_n % size;

  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
      int index_A = proc * delta * A_m;
      int index_B = proc * delta;

      if (rem != 0) {
        index_A += rem * A_m;
        index_B += rem;
      }

      vector<int> b_columns(delta * B_n, 0);
      for (int i = 0; i < b_columns.size(); i++) {
        int index_n = i % delta;
        int index_m = (i / delta) * B_m;

        b_columns[i] = B[index_B + index_n + index_m];
      }

      MPI_Send(A.data() + index_A, delta * A_m, MPI_INT, proc, A_TAG,
               MPI_COMM_WORLD);
      MPI_Send(b_columns.data(), b_columns.size(), MPI_INT, proc, B_TAG,
               MPI_COMM_WORLD);
    }
  }
  int local_x = (delta + rem) * A_m;

  vector<int> local_a(local_x, 0);
  vector<int> local_b(local_x + 1, 0);

  if (rank == 0) {
    for (int i = 0; i < local_x; i++) {
      int index_n = i % (delta + rem);
      int index_m = (i / (delta + rem)) * B_m;

      local_b[i] = B[index_n + index_m];
      local_a[i] = A[i];
    }
  } else {
    MPI_Recv(local_a.data(), local_x - rem * A_m, MPI_INT, 0, A_TAG,
             MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    MPI_Recv(local_b.data(), local_x - rem * A_m, MPI_INT, 0, B_TAG,
             MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
  }

  if (rank == 0) {
    local_b[local_x] = 0;
  } else {
    local_b[local_x] = rank * delta + rem;
  }

  vector<int> local_product(A_n * B_m, 0);

  int from = (rank + 1) % size;
  int to = (rank - 1) < 0 ? size - 1 : rank - 1;

  for (int i = 0; i < size; i++) {
    int local_A_n;
    if (rank == 0) {
      local_A_n = delta + rem;
    } else {
      local_A_n = delta;
    }

    int local_B_m;
    if ((rank + i) % size == 0) {
      local_B_m = delta + rem;
    } else {
      local_B_m = delta;
    }

    vector<int> temp = SequentialMatrixMultiplication(
        local_a, local_b, local_A_n, A_m, local_B_m);

    int index_matrix;
    if (rank == 0) {
      index_matrix = 0;
    } else {
      index_matrix = (rank * delta + rem) * A_n;
    }

    for (int j = 0; j < temp.size(); j++) {
      int index_n = local_b[local_x] + j % local_B_m;
      int index_m = (j / local_B_m) * A_n;

      local_product[index_matrix + index_n + index_m] += temp[j];
    }

    MPI_Sendrecv_replace(local_b.data(), local_b.size(), MPI_INT, to, i, from,
                         i, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
  }

  vector<int> global_product(A_n * B_m, 0);
  MPI_Reduce(local_product.data(), global_product.data(), local_product.size(),
             MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return global_product;
}
