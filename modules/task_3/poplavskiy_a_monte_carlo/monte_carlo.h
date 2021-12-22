// Copyright 2021 Poplavskiy Anton
#ifndef MODULES_TASK_3_POPLAVSKIY_A_MONTE_CARLO_MONTE_CARLO_H_
#define MODULES_TASK_3_POPLAVSKIY_A_MONTE_CARLO_MONTE_CARLO_H_
#include <mpi.h>

#include <array>
#include <ctime>
#include <functional>
#include <random>
#include <vector>

using std::array;
using std::function;
using std::mt19937;
using std::uniform_real_distribution;
using std::vector;

double getParallelIntegral(vector<array<double, 2>> a_b, int points,
                           function<double(vector<double>)> f);
double getSequentialIntegral(vector<array<double, 2>> a_b, int number_of_points,
                             function<double(vector<double>)> f);

#endif  // MODULES_TASK_3_POPLAVSKIY_A_MONTE_CARLO_MONTE_CARLO_H_
