#include <bits/stdc++.h>
// #include <graph.h>

pthread_mutex_t mutex;

struct Node {
    int id;
    int parentId;
    std::string comToExec;
    std::string arguments;
    std::vector<int> childrenId;
};

std::vector<Node> vectorOfNodes(5);
std::vector<int> planning(5);

void* ExecUtilits(void* args){
    int id = ((Node *)args)->id;
    std::cout << "Thread ID recieve = " << id <<'\n';

    int parentId = ((Node *)args)->parentId;
    std::string comToExec = ((Node *)args)->comToExec;
    FILE *fp;
    char line[1000];
    std::string result;
    fp = popen(comToExec.c_str(), "r");
    while (fgets(line, 1000, fp)){
        result += line;
    }

    long long er;
    vectorOfNodes[id].arguments = result; // simply check rites to itself
    if (er = pthread_mutex_lock(&mutex)){
        return (void*)er;
    }

    if (id != 0){
        int amountOfChildren = vectorOfNodes[parentId].childrenId.size() - 1;
        int countFinish;
        for (int j: vectorOfNodes[parentId].childrenId){
            if (planning[j] == 3){
                countFinish++;
            }
        }
        std::cout<< amountOfChildren << ' ' << countFinish <<'\n';
        if (amountOfChildren == countFinish){
            planning[parentId] = 1;
        }
    }

    

    if (er = pthread_mutex_unlock(&mutex)){
        return (void*)er;
    }
    planning[id] = 3;
    for(int i = 0; i < planning.size(); ++i){
            std::cout << planning[i] << ' ';
        }
    std::cout << '\n';
    std::cout<< "Thread end\n"; 
    return NULL;
}


void addChild(std::vector<Node>& vectorOfNodes, int parentId, int selfId, std::string comToExec){
    if (parentId != -1){
        vectorOfNodes[parentId].childrenId.push_back(selfId);    
        vectorOfNodes[selfId].parentId = parentId;
    }
    // std::cout << comToExec << '\n';
    vectorOfNodes[selfId].id = selfId;
    vectorOfNodes[selfId].comToExec = comToExec;
    return;
}

int main(){
    int amountThreads = 3;
    addChild(vectorOfNodes, -1, 0, "date");
    addChild(vectorOfNodes, 0,  1, "whoami");
    addChild(vectorOfNodes, 0,  2, "ls");
    addChild(vectorOfNodes, 1,  3, "pwd");
    addChild(vectorOfNodes, 1,  4, "uname");
    for (int i = 1; i < vectorOfNodes.size(); ++i){
        if(vectorOfNodes[i].childrenId.empty()){
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

    while(planning[0] != 1) {
        int countInWork = 0;
        for (int i = 1; i < planning.size(); ++i){
            if(planning[i] == 2){
                countInWork++;
            }
        }
        int dostup = amountThreads - countInWork;
        for (int i = 1; i < planning.size() && dostup > 0; ++i){
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
        // for(int i = 0; i < planning.size(); ++i){
        //     std::cout << planning[i] << ' ';
        // }
        // std::cout << '\n';
    }
    
    std::cout << planning[0] << '\n';

    if (planning[0] == 1){
            pthread_t thread;
                if(int err = pthread_create(&thread, NULL, ExecUtilits, (void *)&vectorOfNodes[0])){
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