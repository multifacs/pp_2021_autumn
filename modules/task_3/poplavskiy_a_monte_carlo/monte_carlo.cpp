// Copyright 2021 Poplavskiy Anton
#include "../../../modules/task_3/poplavskiy_a_monte_carlo/monte_carlo.h"

double getSequentialIntegral(vector<array<double, 2>> a_b, int points,
                             function<double(vector<double>)> f) {
  int dimension = static_cast<int>(a_b.size());
  mt19937 gen(time(0));
  vector<uniform_real_distribution<double>> uniform(dimension);

  double result = 0.0;

  for (int i = 0; i < dimension; i++)
    uniform[i] = uniform_real_distribution<double>(a_b[i][0], a_b[i][1]);

  vector<double> node(dimension);

  for (int i = 0; i < points; i++) {
    for (int j = 0; j < dimension; j++) node[j] = uniform[j](gen);
    result += f(node);
  }
  for (int i = 0; i < dimension; i++) {
    result *= (a_b[i][0] - a_b[i][1]);
  }
  result /= points;
  return result;
}

double getParallelIntegral(vector<array<double, 2>> a_b, int points_num,
                           function<double(vector<double>)> f) {
  int rank;
  int size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int integral_dim = static_cast<int>(a_b.size());
  vector<double> points(points_num * integral_dim);

  if (rank == 0) {
    mt19937 gen(time(0));
    vector<uniform_real_distribution<double>> uniform(integral_dim);
    for (int i = 0; i < integral_dim; i++) {
      uniform[i] = uniform_real_distribution<double>(a_b[i][0], a_b[i][1]);
    }
    for (int i = 0; i < points_num; i++) {
      for (int j = 0; j < integral_dim; j++) {
        points[(i * integral_dim) + j] = uniform[j](gen);
      }
    }
  }

  double local_sum = 0.0;
  double global_sum = 0.0;
  int delta = points_num / size;
  int rem = points_num % size;
  vector<double> local_points(delta * integral_dim);

  MPI_Scatter(points.data(), delta * integral_dim, MPI_DOUBLE,
              local_points.data(), delta * integral_dim, MPI_DOUBLE, 0,
              MPI_COMM_WORLD);

  vector<double> func_params(integral_dim);

  for (int i = 0; i < delta; i++) {
    for (int j = 0; j < integral_dim; j++) {
      func_params[j] = local_points[(i * integral_dim) + j];
    }
    local_sum += f(func_params);
  }

  if (rank == 0) {
    for (int i = 0; i < rem; i++) {
      for (int j = 0; j < integral_dim; j++) {
        func_params[j] = points[(delta * size + i) * integral_dim + j];
      }
      local_sum += f(func_params);
    }
  }

  MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0,
             MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < integral_dim; i++) {
      global_sum *= (a_b[i][0] - a_b[i][1]);
    }
    global_sum /= points_num;
  }

  return global_sum;
}
