#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>

#include "lab3.h"

using namespace std;

int main(){
    int N, M, max_thread, loop;
    
    cin >> N >> M >> max_thread >> loop;
    
    TMatrix mas1(N, vector<double>(M,0));
    TMatrix mas2(3, vector<double>(3,0));

    double x;
    for(int i = 0; i < N; ++i){
        for(int j = 0; j < M; ++j){
            cin >> x;
            mas1[i][j] = x;
        }
    }

    for(int i = 0; i < 3; ++i){
        for(int j = 0;j < 3;++j){
            cin >> x;
            mas2[i][j] = x;
        }
    }

    TMatrix result = MatrixConvolution(mas1, mas2, max_thread, loop);

    cout << " result \n";
    for(int i = 0; i < N; ++i){
        for(int j = 0;j < M; ++j){
            cout << result[i][j] << " ";
            
        }
        cout << '\n';
    }

}