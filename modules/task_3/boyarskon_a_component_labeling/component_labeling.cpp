// Copyright 2021 Boyarskov Anton
#include "../../modules/task_3/boyarskon_a_component_labeling/component_labeling.h"

#include <mpi.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

vector<int> remark(vector<int> image, int width, int height) {
  int size = width * height;
  vector<int> result(size);
  vector<int> last_labels(size / 2 + 1);
  vector<int> new_labels(size / 2 + 1);
  int max_label = 0;

  for (int i = 0; i < size; i++) {
    int pixel = image[i];
    if (pixel != 0) {
      int idx = -1;
      for (int k = 1; last_labels[k] != 0; k++)
        if (last_labels[k] == pixel) {
          idx = k;
          break;
        }
      if (idx == -1) {
        last_labels[++max_label] = pixel;
        new_labels[max_label] = max_label;
        result[i] = new_labels[max_label];
      } else {
        result[i] = new_labels[idx];
      }
    }
  }
  return result;
}

vector<int> getRandomImage(int width, int height) {
  std::mt19937 gen(time(0));
  std::uniform_int_distribution<> uid(0, 1);
  vector<int> img(width * height);
  for (int i = 0; i < width * height; i++) {
    img[i] = uid(gen);
  }
  return img;
}

pair<vector<int>, pair<vector<int>, int> > firstIter(vector<int> image,
                                                     int width, int height,
                                                     int first_label) {
  int count = 0;
  int size = width * height;
  vector<int> sets(size);  // disjoint sets of labels
  vector<int> temp(image.begin(), image.begin() + size);
  for (int x = 0; x < size; x++) sets[x] = x + first_label;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int idx = i * width + j;
      if (temp[idx] != 0) {
        // [0]      [A]
        // [B] [tmp_image[idx]]
        int A, B;
        if (idx < width) {
          A = 0;
        } else {
          A = temp[idx - width];
        }
        if (((idx < 1) || ((idx - 1) / width != i))) {
          B = 0;
        } else {
          B = temp[idx - 1];
        }

        if ((A == 0) && (B == 0)) {
          temp[idx] = idx + first_label + 1;
          count++;
        }
        if ((A == 0) && (B != 0)) temp[idx] = B;
        if ((A != 0) && (B == 0)) temp[idx] = A;
        if ((A != 0) && (B != 0)) {
          if (A == B) {
            temp[idx] = A;
          } else {
            int AB_max = max(A, B);
            while (sets[AB_max - first_label] != AB_max)
              AB_max = sets[AB_max - first_label];

            int AB_min = min(A, B);
            while (sets[AB_min - first_label] != AB_min)
              AB_min = sets[AB_min - first_label];

            if (AB_max != AB_min) {
              sets[AB_max - first_label] = AB_min;
              count--;
            }

            temp[idx] = AB_min;
          }
        }
      }
    }
  }

  // cout << "Tmp img" << endl;
  // for (int r = 0; r < height; r++) {
  //  for (int c = 0; c < width; c++) {
  //    // cout << tmp_image[c + r * width] << " ";
  //  }
  //  // cout << endl;
  //}
  // cout << "Disjoint sets" << endl;
  // for (int r = 0; r < height; r++) {
  //  for (int c = 0; c < width; c++) {
  //    // cout << disjoint_sets[c + r * width] << " ";
  //  }
  //  // cout << endl;
  //}

  return make_pair(temp, make_pair(sets, count));
}

vector<int> secondIter(vector<int> map, vector<int> sets, int width,
                       int height) {
  // std::cout << "Point 5.1" << std::endl;
  int image_size = width * height;
  vector<int> result(image_size);
  int pixels_count = image_size;

  // cout << "Point 5.2" << endl;
  // std::cout << pixels_count << " " << map.size() << " " << result.size() << "
  // "
  //       << sets.size() << std::endl;
  for (int pos = 0; pos < pixels_count - 1; pos++) {
    try {
      int pixel = map[pos];
      if (pixel != 0) {
        if (sets[pixel] == pixel) {
          result[pos] = pixel;
        } else {
          while (sets[pixel] != pixel) pixel = sets[pixel];
          result[pos] = pixel;
        }
      }
    } catch (...) {
      // cout << "Oops";
    }
  }

  /*int A = map[image_size - width];
int B = map[image_size - 2];
if ((A == 0) && (B == 0)) result[image_size - 1] = map[--pixels_count];
if ((A != 0) && (B == 0)) result[image_size - 1] = A;
if ((A == 0) && (B != 0)) result[image_size - 1] = B;
if ((A != 0) && (B != 0)) result[image_size - 1] = min(A, B);*/

  // cout << "Point 5.3" << endl;
  return result;
}

pair<vector<int>, int> getComponentLabelingSequential(vector<int> image,
                                                      int width, int height) {
  pair<vector<int>, pair<vector<int>, int> > first_pass_result =
      firstIter(image, width, height);

  vector<int> map = first_pass_result.first;
  vector<int> disjoint_sets = first_pass_result.second.first;
  int labels_count = first_pass_result.second.second;

  vector<int> result = secondIter(map, disjoint_sets, width, height);

  return make_pair(result, labels_count);
}

pair<vector<int>, int> getComponentLabelingParallel(vector<int> image,
                                                    int width, int height) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (size == 1) return getComponentLabelingSequential(image, width, height);

  // if (rank == 0) cout << "Point 1" << endl;

  int image_size = width * height;
  const int delta = height / size * width;
  const int rem = (height % size) * width;

  if (delta == 0) {
    if (rank == 0) {
      return getComponentLabelingSequential(image, width, height);
    } else {
      return make_pair(vector<int>{}, 0);
    }
  }

  if (rank == 0) {
    for (int proc = 1; proc < size; proc++)
      MPI_Send(image.data() + rem + proc * delta, delta, MPI_INT, proc, 0,
               MPI_COMM_WORLD);
  }

  // if (rank == 0) cout << "Point 2" << endl;

  vector<int> local_image(delta + rem);

  if (rank == 0) {
    local_image = vector<int>(image.cbegin(), image.cbegin() + delta + rem);
  } else {
    MPI_Recv(local_image.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUSES_IGNORE);
  }

  int temp_height = 0, temp_label = delta * rank;
  if (rank == 0) {
    temp_height = (rem + delta) / width;
  } else {
    temp_height = delta / width;
    temp_label += rem;
  }
  pair<vector<int>, pair<vector<int>, int> > firstIterResult =
      firstIter(local_image, width, temp_height, temp_label);

  // if (rank == 0) cout << "Point 3" << endl;

  vector<int> map = firstIterResult.first,
              dis_set = firstIterResult.second.first;
  int local_labels_counts = firstIterResult.second.second;

  vector<int> displs(size), counts(size);
  counts[0] = rem + delta;
  counts[1] = delta;
  displs[1] = rem + delta;
  for (int proc = 2; proc < size; proc++) {
    displs[proc] = displs[proc - 1] + delta;
    counts[proc] = delta;
  }

  // if (rank == 0) cout << "Point 4" << endl;

  vector<int> global_sets(image_size);
  int temp_sendcount = delta;
  if (rank == 0) temp_sendcount += rem;

  MPI_Gatherv(dis_set.data(), temp_sendcount, MPI_INT, global_sets.data(),
              counts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

  vector<int> global_map(image_size);
  MPI_Gatherv(map.data(), temp_sendcount, MPI_INT, global_map.data(),
              counts.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

  int global_count = 0;
  MPI_Reduce(&local_labels_counts, &global_count, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  // if (rank == 0) cout << "Point 5" << endl;

  vector<int> result(image_size);
  if (rank == 0) {
    for (int x = 1; x < size; x++) {
      int line_two = delta * x;
      if (x != 0) {
        line_two += rem;
      }
      int line_one = line_two - width;

      for (int x = 0; x < width; x++) {
        int A = global_map[line_one + x];
        int B = global_map[line_two + x];
        if ((A != 0) && (B != 0)) {
          int A_set = global_sets[A];
          int B_set = global_sets[B];
          if (A_set != B_set) {
            int AB_max = max(A, B);

            while (global_sets[AB_max] != AB_max) AB_max = global_sets[AB_max];

            int AB_min = min(A, B);

            while (global_sets[AB_min] != AB_min) AB_min = global_sets[AB_min];

            if (AB_max != AB_min) {
              global_sets[AB_max] = AB_min;
              global_count--;
            }
          }
        }
      }
    }
    result = secondIter(global_map, global_sets, width, height);
  }

  // if (rank == 0) cout << "Point 6" << endl;

  return make_pair(result, global_count);
}
