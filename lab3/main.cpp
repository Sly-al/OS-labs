#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>

#include "lab3.h"

using namespace std;

int main(){
    int n, m, maxThread, loop;
    
    cin >> n >> m >> maxThread >> loop;
    
    TMatrix mas1(n, vector<double>(m,0));
    TMatrix mas2(KERNEL_MATRIX_SIZE, vector<double>(KERNEL_MATRIX_SIZE,0));

    double x;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            cin >> x;
            mas1[i][j] = x;
        }
    }

    for(int i = 0; i < KERNEL_MATRIX_SIZE; ++i){
        for(int j = 0;j < KERNEL_MATRIX_SIZE; ++j){
            cin >> x;
            mas2[i][j] = x;
        }
    }

    TMatrix result = MatrixConvolution(mas1, mas2, maxThread, loop);

    cout << " result \n";
    for(int i = 0; i < n; ++i){
        for(int j = 0;j < m; ++j){
            cout << result[i][j] << " ";
            
        }
        cout << '\n';
    }

}