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
    TMatrix mas2(3, vector<double>(3,0));

    double x;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
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

    TMatrix result = MatrixConvolution(mas1, mas2, maxThread, loop);

    cout << " result \n";
    for(int i = 0; i < n; ++i){
        for(int j = 0;j < m; ++j){
            cout << result[i][j] << " ";
            
        }
        cout << '\n';
    }

}