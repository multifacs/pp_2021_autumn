// Copyright 2021 Barysheva Maria
#include "../../../modules/task_3/barysheva_m_radix_batcher/radix_batcher.h"

#include <mpi.h>

#include <cmath>
#include <ctime>
#include <algorithm>
#include <random>
#include <utility>
#include <vector>

double* getRandomVector(const int size) {
  std::mt19937 gen(time(0));
  std::uniform_real_distribution<> urd(0, 300);
  double* vec = new double[size];
  for (int i = 0; i < size; i++) vec[i] = urd(gen);

  return vec;
}

double* merge(double* vec1, double* vec2, int size1, int size2) {
  int vec1_elem = 0, vec2_elem = 0, k = 0;
  double* result = new double[size1 + size2];

  while (vec1_elem < size1 && vec2_elem < size2)
    if (vec1[vec1_elem] < vec2[vec2_elem]) {
      result[k] = vec1[vec1_elem];
      vec1_elem++;
      k++;
    } else {
      result[k] = vec2[vec2_elem];
      vec2_elem++;
      k++;
    }
  if (vec1_elem == size1) {
    while (vec2_elem < size2) {
      result[k] = vec2[vec2_elem];
      vec2_elem++;
      k++;
    }
  } else {
    while (vec1_elem < size1) {
      result[k] = vec1[vec1_elem];
      vec1_elem++;
      k++;
    }
  }
  return result;
}

void RadixSortParallel(double** arr, int size_v) {
  double* chunk;
  double* other;
  int m = size_v, n = size_v;
  int rank, size;
  int delta, step = 1;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    delta = n / size;

    MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
    chunk = new double[delta];
    MPI_Scatter(*arr, delta, MPI_DOUBLE, chunk, delta, MPI_DOUBLE, 0,
                MPI_COMM_WORLD);
    floatRadixSort<double>(&chunk, delta);
  } else {
    MPI_Bcast(&delta, 1, MPI_INT, 0, MPI_COMM_WORLD);
    chunk = new double[delta];
    MPI_Scatter(*arr, delta, MPI_DOUBLE, chunk, delta, MPI_DOUBLE, 0,
                MPI_COMM_WORLD);
    floatRadixSort<double>(&chunk, delta);
  }

  while (step < size) {
    int current_rank = rank % (2 * step);
    if (current_rank == 0) {
      if (rank + step < size) {
        MPI_Recv(&m, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);
        other = new double[m];
        MPI_Recv(other, m, MPI_DOUBLE, rank + step, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);
        chunk = merge(chunk, other, delta, m);
        delta = delta + m;
        fflush(stdout);
      }
    } else {
      int near = rank - step;
      MPI_Send(&delta, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
      MPI_Send(chunk, delta, MPI_DOUBLE, near, 0, MPI_COMM_WORLD);
      break;
    }
    step = step * 2;
  }
  if (rank == 0) {
    *arr = chunk;
  }
}
