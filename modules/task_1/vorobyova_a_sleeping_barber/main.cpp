// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./sleeping_barber.h"
#include <gtest-mpi-listener.hpp>

TEST(Sleeping_Barber_MPI, Test1) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    ASSERT_ANY_THROW(execute(2));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Sleeping_Barber_MPI, Test2) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size > 2) {
    ASSERT_NO_THROW(execute(10));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Sleeping_Barber_MPI, Test3) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size > 2) {
    ASSERT_NO_THROW(execute(20));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Sleeping_Barber_MPI, Test4) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size > 2) {
    ASSERT_NO_THROW(execute(30));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Sleeping_Barber_MPI, Test5) {
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size > 2) {
    ASSERT_NO_THROW(execute(15));
  }
  MPI_Barrier(MPI_COMM_WORLD);
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
