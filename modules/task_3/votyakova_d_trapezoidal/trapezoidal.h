// Copyright 2021 Votyakova Daria
#ifndef MODULES_TASK_3_VOTYAKOVA_D_TRAPEZOIDAL_TRAPEZOIDAL_H_
#define MODULES_TASK_3_VOTYAKOVA_D_TRAPEZOIDAL_TRAPEZOIDAL_H_

#include <cmath>
#include <functional>
#include <vector>
#include <utility>

using std::vector;
using std::pair;

double getSequentialTrapezoidal(int n, vector<pair<double, double>> limits,
                                function<double(vector<double>)> f);
double getParallelTrapezoidal(int n, vector<pair<double, double>> limits,
                              const function<double(vector<double>)> f);
#endif  // MODULES_TASK_3_VOTYAKOVA_D_TRAPEZOIDAL_TRAPEZOIDAL_H_
