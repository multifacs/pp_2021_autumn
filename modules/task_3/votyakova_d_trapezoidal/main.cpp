// Copyright 2021 Votyakova Daria
#include <gtest/gtest.h>

#include <cmath>
#include <gtest-mpi-listener.hpp>

#include "./trapezoidal.h"

const double error = 0.01;

const function<double(vector<double>)> f1 = [](vector<double> vec) {
  double x = vec[0];
  return x * x + 10 * x - 5;
};

const function<double(vector<double>)> f2 = [](vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  return x - sin(y) + x * x;
};

const function<double(vector<double>)> f3 = [](vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  return y * y * y - 10 * cos(x);
};

const function<double(vector<double>)> f4 = [](vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  double z = vec[2];
  return x * x * x + sin(y) * 2 - 10 * z;
};

const function<double(vector<double>)> f5 = [](vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  double z = vec[2];
  return (x + y + z) * 10 * log10(z);
};

TEST(Trapezoidal_method_MPI, TEST_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<pair<double, double>> limits = {{0, 2}};
  int n = 100;

  double start, end;
  if (rank == 0) start = MPI_Wtime();
  double parallel_result = getParallelTrapezoidal(n, limits, f1);
  if (rank == 0) end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    cout << "Parallel: " << ptime << endl;

    start = MPI_Wtime();
    double reference_result = getSequentialTrapezoidal(n, limits, f1);
    end = MPI_Wtime();

    double stime = end - start;
    cout << "Sequential: " << stime << endl;
    cout << "Speedup: " << stime / ptime << endl;

    ASSERT_NEAR(reference_result, parallel_result, error);
  }
}

TEST(Trapezoidal_method_MPI, TEST_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<pair<double, double>> limits = {{0, 2}, {-1, 5}};
  int n = 50;

  double start, end;
  if (rank == 0) start = MPI_Wtime();
  double parallel_result = getParallelTrapezoidal(n, limits, f2);
  if (rank == 0) end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    cout << "Parallel: " << ptime << endl;

    start = MPI_Wtime();
    double reference_result = getSequentialTrapezoidal(n, limits, f2);
    end = MPI_Wtime();

    double stime = end - start;
    cout << "Sequential: " << stime << endl;
    cout << "Speedup: " << stime / ptime << endl;

    ASSERT_NEAR(reference_result, parallel_result, error);
  }
}

TEST(Trapezoidal_method_MPI, TEST_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<pair<double, double>> limits = {{0, 2}, {1, 3}};
  int n = 75;

  double start, end;
  if (rank == 0) start = MPI_Wtime();
  double parallel_result = getParallelTrapezoidal(n, limits, f3);
  if (rank == 0) end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    cout << "Parallel: " << ptime << endl;

    start = MPI_Wtime();
    double reference_result = getSequentialTrapezoidal(n, limits, f3);
    end = MPI_Wtime();

    double stime = end - start;
    cout << "Sequential: " << stime << endl;
    cout << "Speedup: " << stime / ptime << endl;

    ASSERT_NEAR(reference_result, parallel_result, error);
  }
}

TEST(Trapezoidal_method_MPI, TEST_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<pair<double, double>> limits = {{0, 2}, {1, 3}, {-2, 3}};
  int n = 10;

  double start, end;
  if (rank == 0) start = MPI_Wtime();
  double parallel_result = getParallelTrapezoidal(n, limits, f4);
  if (rank == 0) end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    cout << "Parallel: " << ptime << endl;

    start = MPI_Wtime();
    double reference_result = getSequentialTrapezoidal(n, limits, f4);
    end = MPI_Wtime();

    double stime = end - start;
    cout << "Sequential: " << stime << endl;
    cout << "Speedup: " << stime / ptime << endl;

    ASSERT_NEAR(reference_result, parallel_result, error);
  }
}

TEST(Trapezoidal_method_MPI, TEST_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<pair<double, double>> limits = {{0, 2}, {6, 7}, {-5, 1}};
  int n = 100;

  double start, end;
  if (rank == 0) start = MPI_Wtime();
  double parallel_result = getParallelTrapezoidal(n, limits, f1);
  if (rank == 0) end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    cout << "Parallel: " << ptime << endl;

    start = MPI_Wtime();
    double reference_result = getSequentialTrapezoidal(n, limits, f1);
    end = MPI_Wtime();

    double stime = end - start;
    cout << "Sequential: " << stime << endl;
    cout << "Speedup: " << stime / ptime << endl;

    ASSERT_NEAR(reference_result, parallel_result, error);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
