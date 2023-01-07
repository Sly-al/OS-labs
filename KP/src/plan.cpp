#include <pthread.h>
#include "parser.h"
#include "graph.h"

pthread_mutex_t mutex;

std::vector<Node> vectorOfNodes;
std::vector<int> planning;

bool Stop = false;

void* ExecUtilits(void* args){

    int amountOfNodes = planning.size();
    std::array<char, 128> buffer;
    std::string result;
    long long er;

    int id = ((Node *)args)->id;
    std::vector<int> childId = ((Node *)args)->childId;
    std::string comToExec = ((Node *)args)->comToExec;
    
    auto pipe = popen(comToExec.c_str(), "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe)) {
        if (fgets(buffer.data(), buffer.size(), pipe) != nullptr){
            result += buffer.data();
        }
    }

    int rc = pclose(pipe);

    if (rc != EXIT_SUCCESS) {
        er = pthread_mutex_lock(&mutex);
        if (er){
            return (void*)er;
        }
        Stop = true;
        er = pthread_mutex_unlock(&mutex);
        if (er){
            return (void*)er;
        }
        return NULL;
    }

    vectorOfNodes[id].arguments = result;

    if (id != amountOfNodes - 1){
        for(int k: childId){
            int amountOfParents = vectorOfNodes[k].parentId.size() - 1;
            int countFinish = 0;
            for (int j: vectorOfNodes[k].parentId){
                if (planning[j] == FINISH){     
                    countFinish++;
                }
                
            }
            
            if (amountOfParents == countFinish){
                er = pthread_mutex_lock(&mutex);
                if (er){
                    return (void*)er;
                }
                planning[k] = READY;
                er = pthread_mutex_unlock(&mutex);
                if (er){
                    return (void*)er;
                }
            }
            
        }
    }

    planning[id] = FINISH;
    for(unsigned long i = 0; i < planning.size(); ++i){
            std::cout << planning[i] << ' ';
        }
    std::cout << '\n';
     
    return NULL;
}

int main(){
    int amountThreads;
    std::string fileJson;
    
    std::cout<< "Input amount of threads and file with DAG\n";
    std::cin >> amountThreads >> fileJson;

    JsonToVector(fileJson, vectorOfNodes, planning);

    int amountOfNodes = planning.size();
    graph matr(amountOfNodes);

    for(int i = 0; i < amountOfNodes; ++i){
        for (int j : vectorOfNodes[i].childId){
            matr[i].push_back(j);
            matr[j].push_back(i);
        }
    }

    if(!CheckOneComp(matr) || FindCycle(vectorOfNodes) ){
        std::cout<< "There are cycle or more tnan one componet\n";
        return (EXIT_FAILURE);
    }

    for (int i = 0; i < amountOfNodes; ++i){
        if(vectorOfNodes[i].parentId.empty()){
            planning[i] = READY;
        }
    }

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        std::cout << "mutex init failed\n";
        return (EXIT_FAILURE);
    }

    while(planning[amountOfNodes - 1] != FINISH) {
        int countInWork = 0;
        for (int i = 0; i < amountOfNodes; ++i){
            if(planning[i] == INWORKING){
                countInWork++;
            }
        }
        int dostup = amountThreads - countInWork;
        for (int i = 0; (i < amountOfNodes) && dostup > 0; ++i){
            if(planning[i] == READY){
                dostup--;
                planning[i] = INWORKING;
                pthread_t thread;
                if(int err = pthread_create(&thread, NULL, ExecUtilits, (void *)&vectorOfNodes[i])){
                    std::cout << "Thread create error: " << err << '\n';
                    return (EXIT_FAILURE);
                }
                if(int err = pthread_detach(thread)){
                    std::cout << "Thread detach error: " << err << '\n';
                    return (EXIT_FAILURE);
                }
            } 
        }

        if (Stop){
            std::cout << "Job crushed\n";
            return (EXIT_FAILURE);
        }
    }

    for (int i = 0; i < amountOfNodes; ++i){
        std::cout<< "Id = " << i << ' ' << "Result = " << vectorOfNodes[i].arguments <<'\n';
    }
    pthread_mutex_destroy(&mutex);
    return (EXIT_SUCCESS);
}