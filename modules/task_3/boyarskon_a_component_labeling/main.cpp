// Copyright 2021 Boyarskov Anton
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./component_labeling.h"

TEST(Component_Labeling_MPI, Test_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<int> image;
  int width = 500, height = 400;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  pair<vector<int>, int> parallel_labels =
      getComponentLabelingParallel(image, width, height);

  if (rank == 0) {
    pair<vector<int>, int> reference_labels =
        getComponentLabelingSequential(image, width, height);

  ASSERT_EQ(parallel_labels.second, reference_labels.second);
  }
}

TEST(Component_Labeling_MPI, Test_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<int> image;
  int width = 200, height = 400;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  pair<vector<int>, int> parallel_labels =
      getComponentLabelingParallel(image, width, height);

  if (rank == 0) {
    pair<vector<int>, int> reference_labels =
        getComponentLabelingSequential(image, width, height);

    ASSERT_EQ(parallel_labels.second, reference_labels.second);
  }
}

TEST(Component_Labeling_MPI, Test_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<int> image;
  int width = 123, height = 207;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  pair<vector<int>, int> parallel_labels =
      getComponentLabelingParallel(image, width, height);

  if (rank == 0) {
    pair<vector<int>, int> reference_labels =
        getComponentLabelingSequential(image, width, height);

    ASSERT_EQ(parallel_labels.second, reference_labels.second);
  }
}

TEST(Component_Labeling_MPI, Test_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<int> image;
  int width = 503, height = 1007;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  pair<vector<int>, int> parallel_labels =
      getComponentLabelingParallel(image, width, height);

  if (rank == 0) {
    pair<vector<int>, int> reference_labels =
        getComponentLabelingSequential(image, width, height);

    ASSERT_EQ(parallel_labels.second, reference_labels.second);
  }
}

TEST(Component_Labeling_MPI, Test_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  vector<int> image;
  int width = 1001, height = 403;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }

  pair<vector<int>, int> parallel_labels =
      getComponentLabelingParallel(image, width, height);

  if (rank == 0) {
    pair<vector<int>, int> reference_labels =
        getComponentLabelingSequential(image, width, height);

    ASSERT_EQ(parallel_labels.second, reference_labels.second);
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
