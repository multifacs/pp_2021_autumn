// Copyright 2021 Boyarskov Anton
#ifndef MODULES_TASK_3_BOYARSKON_A_COMPONENT_LABELING_COMPONENT_LABELING_H_
#define MODULES_TASK_3_BOYARSKON_A_COMPONENT_LABELING_COMPONENT_LABELING_H_

#include <utility>
#include <vector>

using std::pair;
using std::vector;
using std::max;
using std::min;

pair<vector<int>, int> getComponentLabelingSequential(vector<int> image,
                                                      int width, int height);

pair<vector<int>, pair<vector<int>, int> > firstIter(vector<int> image,
                                                     int width, int height,
                                                     int first_label = 0);

vector<int> secondIter(vector<int> map, vector<int> disjoint_sets, int width,
                       int height);

pair<vector<int>, int> getComponentLabelingParallel(vector<int> image,
                                                    int width, int height);

vector<int> getRandomImage(int width, int height);

vector<int> remark(vector<int> image, int width, int height);

#endif  // MODULES_TASK_3_BOYARSKON_A_COMPONENT_LABELING_COMPONENT_LABELING_H_
