// Copyright 2021 Poplavskiy Anton
#include <gtest/gtest.h>

#include <cmath>
#include <gtest-mpi-listener.hpp>

#include "./monte_carlo.h"

double f1(vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  return 7 * x * x + 6 * y * y * y;
}

double f5(vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  double z = vec[2];
  return 5 * x * x * x - 2 * y + sin(z);
}

TEST(Monte_Carlo_MPI, Test_Function_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<array<double, 2>> a_b = {{0.0, 1.0}, {1.0, 2.0}};
  int points = 100000;

  double start = MPI_Wtime();
  double parallel_result =
      getParallelIntegral(a_b, points, f1);
  double end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;

    double start = MPI_Wtime();
    double reference_result = getSequentialIntegral(a_b, points, f1);
    double end = MPI_Wtime();
    double stime = end - start;

    std::cout << "Speedup: " << stime / ptime
              << std::endl;
    std::cout << "Error: " << abs(reference_result - parallel_result)
              << std::endl;
  }
}

TEST(Monte_Carlo_MPI, Test_Function_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<array<double, 2>> a_b = {{0.0, 4.0}, {1.5, 2.5}, {1.0, 5.0}};
  int points = 10000;

  double start = MPI_Wtime();
  double parallel_result = getParallelIntegral(a_b, points, f5);
  double end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;

    double start = MPI_Wtime();
    double reference_result = getSequentialIntegral(a_b, points, f5);
    double end = MPI_Wtime();
    double stime = end - start;

    std::cout << "Speedup: " << stime / ptime
              << std::endl;
    std::cout << "Error: " << abs(reference_result - parallel_result)
              << std::endl;
  }
}

TEST(Monte_Carlo_MPI, Test_Function_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<array<double, 2>> a_b = {{0.0, 4.0}, {1.5, 2.5}, {1.0, 5.0}};
  int points = 10000;

  double start = MPI_Wtime();
  double parallel_result = getParallelIntegral(a_b, points, f5);
  double end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;

    double start = MPI_Wtime();
    double reference_result = getSequentialIntegral(a_b, points, f5);
    double end = MPI_Wtime();
    double stime = end - start;

    std::cout << "Speedup: " << stime / ptime
              << std::endl;
    std::cout << "Error: " << abs(reference_result - parallel_result)
              << std::endl;
  }
}

TEST(Monte_Carlo_MPI, Test_Function_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<array<double, 2>> a_b = {{0.0, 4.0}, {1.5, 2.5}, {1.0, 5.0}};
  int points = 10000;

  double start = MPI_Wtime();
  double parallel_result = getParallelIntegral(a_b, points, f5);
  double end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;

    double start = MPI_Wtime();
    double reference_result = getSequentialIntegral(a_b, points, f5);
    double end = MPI_Wtime();
    double stime = end - start;

    std::cout << "Speedup: " << stime / ptime
              << std::endl;
    std::cout << "Error: " << abs(reference_result - parallel_result)
              << std::endl;
  }
}

TEST(Monte_Carlo_MPI, Test_Function_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<array<double, 2>> a_b = {{0.0, 4.0}, {1.5, 2.5}, {1.0, 5.0}};
  int points = 10000;

  double start = MPI_Wtime();
  double parallel_result = getParallelIntegral(a_b, points, f5);
  double end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;

    double start = MPI_Wtime();
    double reference_result = getSequentialIntegral(a_b, points, f5);
    double end = MPI_Wtime();
    double stime = end - start;

    std::cout << "Speedup: " << stime / ptime
              << std::endl;
    std::cout << "Error: " << abs(reference_result - parallel_result)
              << std::endl;
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
