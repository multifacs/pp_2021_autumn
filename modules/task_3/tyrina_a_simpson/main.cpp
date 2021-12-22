// Copyright 2021 Tyrina Anastasia
#include <gtest/gtest.h>

#include <cmath>
#include <gtest-mpi-listener.hpp>

#include "./simpson.h"

const function<double(vector<double>)> func1 = [](vector<double> vec) {
  double x = vec[0];
  double y = vec[1];
  return x * x - 2 * y;
};

const double eps = 0.0001;

TEST(SIMPSON_METHOD_MPI, TEST_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<std::pair<double, double>> limits({{4, 10}, {1, 2}});
  int n = 100;

  double start = MPI_Wtime();
  double result = getParallelSimpson(func1, limits, n);
  double end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;

    start = MPI_Wtime();
    double reference_result = getSequentialSimpson(func1, limits, n);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "Sequential: " << stime << std::endl;
    std::cout << "Parallel: " << ptime << std::endl;
    std::cout << "Speedup: " << stime / ptime << std::endl;
    
    ASSERT_NEAR(result, reference_result, eps);
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
