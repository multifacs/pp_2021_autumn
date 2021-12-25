// Copyright 2021 Barysheva Maria
#include <gtest/gtest.h>

#include <cmath>
#include <vector>
#include <gtest-mpi-listener.hpp>

#include "./radix_batcher.h"

TEST(RADIX_BATCHER, TEST_1) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (log2(size) == static_cast<int>(log2(size))) {
    double *vec, *vec_p, *vec_s;
    double start, end;
    int size_v = pow(2, 4);
    if (rank == 0) {
      std::cout << "Size: " << size_v << std::endl;
      vec = getRandomVector(size_v);
      vec_p = new double[size_v];
      vec_s = new double[size_v];
      for (int i = 0; i < size_v; i++) {
        vec_p[i] = vec[i];
        vec_s[i] = vec[i];
      }
    }
    start = MPI_Wtime();
    RadixSortParallel(&vec_p, size_v);
    end = MPI_Wtime();
    if (rank == 0) {
      double ptime = end - start;
      start = MPI_Wtime();
      floatRadixSort<double>(&vec_s, size_v);
      end = MPI_Wtime();
      double stime = end - start;
      for (int i = 0; i < size_v; i++) {
        EXPECT_DOUBLE_EQ(vec_p[i], vec_s[i]);
        // std::cout << vec[i] << " " << vec_s[i] << std::endl;
      }
      std::cout << "Parallel: " << ptime << " Sequential: " << stime
                << std::endl;
      std::cout << "Speedup: " << stime / ptime << std::endl;
    }
  }
}

TEST(RADIX_BATCHER, TEST_2) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (log2(size) == static_cast<int>(log2(size))) {
    double *vec, *vec_p, *vec_s;
    double start, end;
    int size_v = pow(2, 5);
    if (rank == 0) {
      std::cout << "Size: " << size_v << std::endl;
      vec = getRandomVector(size_v);
      vec_p = new double[size_v];
      vec_s = new double[size_v];
      for (int i = 0; i < size_v; i++) {
        vec_p[i] = vec[i];
        vec_s[i] = vec[i];
      }
    }
    start = MPI_Wtime();
    RadixSortParallel(&vec_p, size_v);
    end = MPI_Wtime();
    if (rank == 0) {
      double ptime = end - start;
      start = MPI_Wtime();
      floatRadixSort<double>(&vec_s, size_v);
      end = MPI_Wtime();
      double stime = end - start;
      for (int i = 0; i < size_v; i++) {
        EXPECT_DOUBLE_EQ(vec_p[i], vec_s[i]);
        // std::cout << vec[i] << " " << vec_s[i] << std::endl;
      }
      std::cout << "Parallel: " << ptime << " Sequential: " << stime
                << std::endl;
      std::cout << "Speedup: " << stime / ptime << std::endl;
    }
  }
}

TEST(RADIX_BATCHER, TEST_3) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (log2(size) == static_cast<int>(log2(size))) {
    double *vec, *vec_p, *vec_s;
    double start, end;
    int size_v = pow(2, 10);
    if (rank == 0) {
      std::cout << "Size: " << size_v << std::endl;
      vec = getRandomVector(size_v);
      vec_p = new double[size_v];
      vec_s = new double[size_v];
      for (int i = 0; i < size_v; i++) {
        vec_p[i] = vec[i];
        vec_s[i] = vec[i];
      }
    }
    start = MPI_Wtime();
    RadixSortParallel(&vec_p, size_v);
    end = MPI_Wtime();
    if (rank == 0) {
      double ptime = end - start;
      start = MPI_Wtime();
      floatRadixSort<double>(&vec_s, size_v);
      end = MPI_Wtime();
      double stime = end - start;
      for (int i = 0; i < size_v; i++) {
        EXPECT_DOUBLE_EQ(vec_p[i], vec_s[i]);
        // std::cout << vec[i] << " " << vec_s[i] << std::endl;
      }
      std::cout << "Parallel: " << ptime << " Sequential: " << stime
                << std::endl;
      std::cout << "Speedup: " << stime / ptime << std::endl;
    }
  }
}

TEST(RADIX_BATCHER, TEST_4) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (log2(size) == static_cast<int>(log2(size))) {
    double *vec, *vec_p, *vec_s;
    double start, end;
    int size_v = pow(2, 11);
    if (rank == 0) {
      std::cout << "Size: " << size_v << std::endl;
      vec = getRandomVector(size_v);
      vec_p = new double[size_v];
      vec_s = new double[size_v];
      for (int i = 0; i < size_v; i++) {
        vec_p[i] = vec[i];
        vec_s[i] = vec[i];
      }
    }
    start = MPI_Wtime();
    RadixSortParallel(&vec_p, size_v);
    end = MPI_Wtime();
    if (rank == 0) {
      double ptime = end - start;
      start = MPI_Wtime();
      floatRadixSort<double>(&vec_s, size_v);
      end = MPI_Wtime();
      double stime = end - start;
      for (int i = 0; i < size_v; i++) {
        EXPECT_DOUBLE_EQ(vec_p[i], vec_s[i]);
      }
      std::cout << "Parallel: " << ptime << " Sequential: " << stime
                << std::endl;
      std::cout << "Speedup: " << stime / ptime << std::endl;
    }
  }
}

TEST(RADIX_BATCHER, TEST_5) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (log2(size) == static_cast<int>(log2(size))) {
    double *vec, *vec_p, *vec_s;
    double start, end;
    int size_v = pow(2, 12);
    if (rank == 0) {
      std::cout << "Size: " << size_v << std::endl;
      vec = getRandomVector(size_v);
      vec_p = new double[size_v];
      vec_s = new double[size_v];
      for (int i = 0; i < size_v; i++) {
        vec_p[i] = vec[i];
        vec_s[i] = vec[i];
      }
    }
    start = MPI_Wtime();
    RadixSortParallel(&vec_p, size_v);
    end = MPI_Wtime();
    if (rank == 0) {
      double ptime = end - start;
      start = MPI_Wtime();
      floatRadixSort<double>(&vec_s, size_v);
      end = MPI_Wtime();
      double stime = end - start;
      for (int i = 0; i < size_v; i++) {
        EXPECT_DOUBLE_EQ(vec_p[i], vec_s[i]);
        // std::cout << vec[i] << " " << vec_s[i] << std::endl;
      }
      std::cout << "Parallel: " << ptime << " Sequential: " << stime
                << std::endl;
      std::cout << "Speedup: " << stime / ptime << std::endl;
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
