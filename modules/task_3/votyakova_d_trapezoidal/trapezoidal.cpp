// Copyright 2021 Votyakova Daria
#include "../../../modules/task_3/votyakova_d_trapezoidal/trapezoidal.h"

#include <mpi.h>

double getSequentialTrapezoidal(int n, vector<pair<double, double>> limits,
                                function<double(vector<double>)> f) {
  int integral_dim = static_cast<int>(limits.size());
  int it = 1;
  vector<double> h(integral_dim);
  for (int i = 0; i < integral_dim; i++) {
    h[i] = (limits[i].second - limits[i].first) / n;
    it = it * n;
  }
  double result = 0.0;
  vector<double> x(integral_dim);
  for (int i = 0; i < it; i++) {
    for (int j = 0; j < integral_dim; j++) {
      x[j] = limits[j].first + (i % n) * h[j] + h[j] * 0.5;
    }
    result += f(x);
  }

  for (int i = 0; i < integral_dim; i++) {
    result *= h[i];
  }
  return result;
}

double getParallelTrapezoidal(int n, vector<pair<double, double>> limits,
                              const function<double(vector<double>)> f) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int integral_dim = static_cast<int>(limits.size());
  vector<double> h(n);
  int it_all = 1;

  for (int i = 0; i < integral_dim; i++) {
    h[i] = (limits[i].second - limits[i].first) / n;
    it_all *= n;
  }

  int delta = it_all / size;

  int start = 0;
  if (rank != 0) start = rank * delta + it_all % size;
  int end = (rank + 1) * delta + it_all % size;

  vector<double> x(integral_dim);
  double local_result = 0.0;
  for (int j = start; j < end; j++) {
    for (int i = 0; i < integral_dim; i++) {
      x[i] = limits[i].first + (j % n) * h[i] + h[i] * 0.5;
    }
    local_result += f(x);
  }

  double global_result = 0.0;
  MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < integral_dim; i++) {
      global_result *= h[i];
    }
  }

  return global_result;
}
