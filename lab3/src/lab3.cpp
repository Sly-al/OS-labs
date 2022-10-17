#include "lab3.h"
#include "utils.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>

using namespace std;

struct Data {
    
    vector<vector<double> > &mas1;
    vector<vector<double> > &mas2;
    vector<vector<double> > &res1;

    int thread_num;
    int thread_count;
};

void *MatrixMult(void *args){

    vector<vector<double>>&mas1 = ((Data *)args)->mas1;
    vector<vector<double>>&mas2 = ((Data *)args)->mas2;
    vector<vector<double>>&res1 = ((Data *)args)->res1;

    int thread_num = ((Data *)args)->thread_num;  // номер потока
    int th_count = ((Data *)args)->thread_count;  // количество потоков
    int cols = mas1[0].size();
    int rows = mas1.size();
    int dir[3] = {-1, 0, 1};

     for (int th_row = thread_num; th_row < rows; th_row += th_count) {
        
        for (int th_col = 0; th_col < cols; th_col++) {

            double div = 0.0;
            double new_val = 0.0;

            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    div += mas2[i][j];
                    if ( (th_row + dir[i] >= 0) && (th_row + dir[i] < rows) &&
                    (th_col + dir[j] >= 0) && (th_col + dir[j] < cols) ) {
                        new_val += mas1[th_row + dir[i]][th_col + dir[j]] * mas2[j][i];
                    }
                }
            }
            res1[th_row][th_col] = new_val/div;
        }

    }

    return NULL;
}


TMatrix MatrixConvolution(TMatrix mas1, TMatrix mas2, int max_thread, int loop){

    int N = mas1.size();
    int M = mas1[0].size();

    TMatrix res1(N, vector<double>(M,0));

    vector<Data> data = vector<Data>(max_thread,{mas1,mas2,res1,0,0});
    
    vector<pthread_t> threads = vector<pthread_t>(max_thread);

    for(int k=0; k < loop; ++k){

        for(int p = 0; p < max_thread; p++){
            data[p].thread_count = max_thread;
            data[p].thread_num = p;
            if(p >= N*M){
                break;
            }

            if(int err = pthread_create(&threads[p], NULL, MatrixMult, (void *)&data[p])){
                cout << "Thread create error: " << err << '\n';
                exit(EXIT_FAILURE);
            }
        }

        //join
        for(int i = 0; i < max_thread; i++) {
            if (pthread_join(threads[i],NULL) != 0) {
                cout << "Can't wait for thread\n";
            }
        }

        swap(data[0].mas1,data[0].res1);

    }
    swap(data[0].mas1,data[0].res1);

    return res1;
}