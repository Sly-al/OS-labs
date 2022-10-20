#ifndef LAB3_H
#define LAB3_H
#include <vector>

using namespace std;

constexpr int KERNEL_MATRIX_SIZE = 3;

using TMatrix = vector<vector<double>>;

TMatrix MatrixConvolution(TMatrix mas1, TMatrix mas2, int maxThread, int loop);

#endif