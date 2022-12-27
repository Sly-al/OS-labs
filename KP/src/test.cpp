#include <bits/stdc++.h>
#include "parser.h"
// #include <graph.h>

pthread_mutex_t mutex;

std::vector<Node> vectorOfNodes;
std::vector<int> planning;

bool Stop = false;

void* ExecUtilits(void* args){
    int amountOfNodes = planning.size();
    int id = ((Node *)args)->id;
    int childId = ((Node *)args)->childId;
    std::string comToExec = ((Node *)args)->comToExec;

    std::array<char, 128> buffer;
    std::string result;

    auto pipe = popen(comToExec.c_str(), "r"); // get rid of shared_ptr
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe)) {
        if (fgets(buffer.data(), buffer.size(), pipe) != nullptr){
            result += buffer.data();
        }
    }

    auto rc = pclose(pipe);

    if (rc == EXIT_SUCCESS) { // == 0
        std::cout<< "Allright\n";
    } else {  // EXIT_FAILURE is not used by all programs, maybe needs some adaptation.
        std::cout<< "Bad\n";
        Stop = true;
        return NULL;
    }

    long long er;
    vectorOfNodes[id].arguments = result; // simply check rites to itself
    

    if (id != amountOfNodes - 1){
        int amountOfParents = vectorOfNodes[childId].parentId.size() - 1;
        int countFinish = 0;
        for (int j: vectorOfNodes[childId].parentId){
            if (planning[j] == 3){     
                countFinish++;
            }
        }
        if (er = pthread_mutex_lock(&mutex)){
            return (void*)er;
        }
        if (amountOfParents == countFinish){
            planning[childId] = 1;
        }
        if (er = pthread_mutex_unlock(&mutex)){
            return (void*)er;
        }
    }

    
    planning[id] = 3;
    for(int i = 0; i < planning.size(); ++i){
            std::cout << planning[i] << ' ';
        }
    std::cout << '\n';
    std::cout<< "Thread end\n"; 
    return NULL;
}


// void addChild(std::vector<Node>& vectorOfNodes, int parentId, int selfId, std::string comToExec){
//     if (parentId != -1){
//         vectorOfNodes[parentId].parentId.push_back(selfId);    
//         vectorOfNodes[selfId].parentId = parentId;
//     }
//     // std::cout << comToExec << '\n';
//     vectorOfNodes[selfId].id = selfId;
//     vectorOfNodes[selfId].comToExec = comToExec;
//     return;
// }

int main(){
    // addChild(vectorOfNodes, -1, 0, "ls");
    // addChild(vectorOfNodes, 0,  1, "rm -d dir2");
    // addChild(vectorOfNodes, 0,  2, "> file.txt");
    // addChild(vectorOfNodes, 1,  3, "mkdir dir2");
    // addChild(vectorOfNodes, 1,  4, "mkdir dir1");
    int amountThreads = 2;
    std::string fileJson = "data.json";
    // std::cout<< "Input amount of threads and file with DAG\n";
    // std::cin >> amountThreads >> fileJson;

    JsonToVector(fileJson, vectorOfNodes, planning);

    int amountOfNodes = planning.size();


    for (int i = 0; i < vectorOfNodes.size() - 1; ++i){
        if(vectorOfNodes[i].parentId.empty()){
            planning[i] = 1;
        }
    }

    for(int i = 0; i < planning.size(); ++i){
        std::cout << planning[i] << ' ';
    }
    std::cout << '\n';

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        std::cout << "mutex init failed\n";
        return -1;
    }

    while(planning[amountOfNodes - 1] != 1) {
        int countInWork = 0;
        for (int i = 0; i < planning.size() - 1; ++i){
            if(planning[i] == 2){
                countInWork++;
            }
        }
        int dostup = amountThreads - countInWork;
        for (int i = 0; i < (planning.size() - 1) && dostup > 0; ++i){
            if(planning[i] == 1){
                dostup--;
                planning[i] = 2;
                pthread_t thread;
                if(int err = pthread_create(&thread, NULL, ExecUtilits, (void *)&vectorOfNodes[i])){
                    std::cout << "Thread create error: " << err << '\n';
                    return -1;
                }
                if(int err = pthread_detach(thread)){
                    std::cout << "Thread detach error: " << err << '\n';
                    return -1;
                }
            } 
        }

        if (Stop){
            std::cout << "Job crushed\n";
            return -1;
        }
        // for(int i = 0; i < planning.size(); ++i){
        //     std::cout << planning[i] << ' ';
        // }
        // std::cout << '\n';
    }
    

    if (planning[amountOfNodes - 1] == 1){
            pthread_t thread;
                if(int err = pthread_create(&thread, NULL, ExecUtilits, (void *)&vectorOfNodes[amountOfNodes - 1])){
                    std::cout << "Thread create error: " << err << '\n';
                    return -1;
                }
                // if(int err = pthread_detach(thread)){
                //     std::cout << "Thread detach error: " << err << '\n';
                //     return -1;
                // }
                if(int err = pthread_join(thread, NULL)){
                    std::cout << "Thread join error: " << err << '\n';
                    return -1;
                }
        }

    for (int i = 0; i < vectorOfNodes.size(); ++i){
        std::cout<< "Id = " << i << ' ' << "Result = " << vectorOfNodes[i].arguments <<'\n';
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}