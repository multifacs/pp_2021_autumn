// Copyright 2021 Votyakova Daria
#ifndef MODULES_TASK_3_BAKAEVA_M_INTEGRALS_RECTANGLES_METHOD_INTEGRALS_RECTANGLES_METHOD_H_
#define MODULES_TASK_3_BAKAEVA_M_INTEGRALS_RECTANGLES_METHOD_INTEGRALS_RECTANGLES_METHOD_H_

#include <cmath>
#include <functional>
#include <vector>

using namespace std;

double getSequentialTrapezoidal(int n, vector<pair<double, double>> limits,
                                function<double(vector<double>)> f);
double getParallelTrapezoidal(int n, vector<pair<double, double>> limits,
                              const function<double(vector<double>)> f);
#endif  // MODULES_TASK_3_BAKAEVA_M_INTEGRALS_RECTANGLES_METHOD_INTEGRALS_RECTANGLES_METHOD_H_
