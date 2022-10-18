#ifndef LAB3_H
#define LAB3_H
#include <vector>

using namespace std;

using TMatrix = vector<vector<double>>;

TMatrix MatrixConvolution( TMatrix& mas1, TMatrix& mas2, int max_thread, int loop);

#endif