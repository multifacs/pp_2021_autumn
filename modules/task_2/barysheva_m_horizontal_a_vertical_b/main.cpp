// Copyright 2021 Barysheva Maria
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./horizontal_a_vertical_b.h"

TEST(MATRIX_MULTIPLICATION_A_B, TEST_SQUARE_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 33, m = 33;
  vector<int> A(n * m), B(n * m);

  if (rank == 0) {
    A = getRandomMatrix(n, m);
    B = getRandomMatrix(m, n);
  }

  ASSERT_NO_THROW(ParallelMatrixMultiplication(A, B, n, m));
}

TEST(MATRIX_MULTIPLICATION_A_B, TEST_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 15, m = 30;
  vector<int> A(n * m), B(n * m);

  if (rank == 0) {
    A = getRandomMatrix(n, m);
    B = getRandomMatrix(m, n);
  }

  ASSERT_NO_THROW(ParallelMatrixMultiplication(A, B, n, m));
}

TEST(MATRIX_MULTIPLICATION_A_B, TEST_SQUARE_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 50, m = 50;
  vector<int> A(n * m), B(n * m);

  if (rank == 0) {
    A = getRandomMatrix(n, m);
    B = getRandomMatrix(m, n);
  }

  vector<int> res = ParallelMatrixMultiplication(A, B, n, m);

  if (rank == 0) {
    vector<int> expected_res = SequentialMatrixMultiplication(A, B, n, m, n);

    ASSERT_EQ(res, expected_res);
  }
}

TEST(MATRIX_MULTIPLICATION_A_B, TEST_COLS_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 50, m = 75;
  vector<int> A(n * m), B(n * m);

  if (rank == 0) {
    A = getRandomMatrix(n, m);
    B = getRandomMatrix(m, n);
  }

  vector<int> res = ParallelMatrixMultiplication(A, B, n, m);

  if (rank == 0) {
    vector<int> expected_res = SequentialMatrixMultiplication(A, B, n, m, n);

    ASSERT_EQ(res, expected_res);
  }
}

TEST(MATRIX_MULTIPLICATION_A_B, TEST_ROWS_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 75, m = 50;
  vector<int> A(n * m), B(n * m);

  if (rank == 0) {
    A = getRandomMatrix(n, m);
    B = getRandomMatrix(m, n);
  }

  vector<int> res = ParallelMatrixMultiplication(A, B, n, m);

  if (rank == 0) {
    vector<int> expected_res = SequentialMatrixMultiplication(A, B, n, m, n);

    ASSERT_EQ(res, expected_res);
  }
}

TEST(MATRIX_MULTIPLICATION_A_B, TEST_BIG_6) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int n = 567, m = 183;
  vector<int> A(n * m), B(n * m);

  if (rank == 0) {
    A = getRandomMatrix(n, m);
    B = getRandomMatrix(m, n);
  }

  double start = MPI_Wtime();
  vector<int> res = ParallelMatrixMultiplication(A, B, n, m);
  double end = MPI_Wtime();

  if (rank == 0) {
    double time1 = end - start;

    start = MPI_Wtime();
    vector<int> expected_res = SequentialMatrixMultiplication(A, B, n, m, n);
    end = MPI_Wtime();
    double time2 = end - start;

    std::cout << "Speedup: " << time2 / time1 << std::endl;
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
