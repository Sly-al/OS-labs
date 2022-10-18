#include "lab3.h"
#include "utils.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>

using namespace std;

struct TData {
    
    vector<vector<double> > &mas1;
    vector<vector<double> > &mas2;
    vector<vector<double> > &res1;

    int threadNum;
    int threadCount;
};

void *MatrixMult(void *args){

    vector<vector<double>>&mas1 = ((TData *)args)->mas1;
    vector<vector<double>>&mas2 = ((TData *)args)->mas2;
    vector<vector<double>>&res1 = ((TData *)args)->res1;

    int threadNum = ((TData *)args)->threadNum;  // номер потока
    int thCount = ((TData *)args)->threadCount;  // количество потоков
    int cols = isize(mas1[0]);
    int rows = isize(mas1);
    int dir[3] = {-1, 0, 1};

     for (int thRow = threadNum; thRow < rows; thRow += thCount) {
        
        for (int thCol = 0; thCol < cols; thCol++) {

            double div = 0.0;
            double newVal = 0.0;

            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    div += mas2[i][j];
                    if ( (thRow + dir[i] >= 0) && (thRow + dir[i] < rows) &&
                    (thCol + dir[j] >= 0) && (thCol + dir[j] < cols) ) {
                        newVal += mas1[thRow + dir[i]][thCol + dir[j]] * mas2[j][i];
                    }
                }
            }
            res1[thRow][thCol] = newVal/div;
        }

    }

    return NULL;
}


TMatrix MatrixConvolution(TMatrix& mas1, TMatrix& mas2, int maxThread, int loop){

    int n = isize(mas1);
    int m = isize(mas1[0]);

    TMatrix res1(n, vector<double>(m,0));

    vector<TData> data = vector<TData>(maxThread,{mas1,mas2,res1,0,0});
    
    vector<pthread_t> threads(maxThread);

    for(int k = 0; k < loop; ++k){
        
        for(int p = 0; p < maxThread; p++){
            data[p].threadCount = maxThread;
            data[p].threadNum = p;
            if(p >= n*m){
                break;
            }

            if(int err = pthread_create(&threads[p], NULL, MatrixMult, (void *)&data[p])){
                cout << "Thread create error: " << err << '\n';
                exit(EXIT_FAILURE);
            }
        }

        //join
        for(int i = 0; i < maxThread; i++) {
            if (pthread_join(threads[i],NULL) != 0) {
                cout << "Can't wait for thread\n";
            }
        }

        swap(data[0].mas1,data[0].res1);

    }
    swap(data[0].mas1,data[0].res1);

    return res1;
}