#include <bits/stdc++.h>
// #include <graph.h>

std::vector<bool> jobsCompleting(6);

int u;

struct Node {
    int id;
    int parentId;
    std::string comToExec;
    std::string arguments;
    std::vector<int> childrenId;
};

void addChild(std::vector<Node>& vectorOfNodes, int parentId, int childId, std::string comToExec){
    vectorOfNodes[parentId].childrenId.push_back(childId);
    vectorOfNodes[childId].parentId = parentId;
    vectorOfNodes[childId].comToExec = comToExec;
}

int main(){
    int amountThreads = 3;
    std::vector<Node> vectorOfNodes(7);
    std::vector<int> planning(7);
    addChild(vectorOfNodes, 0,  2, "l");
    addChild(vectorOfNodes, 0, 1, "v");
    addChild(vectorOfNodes, 2, 3, "K");
    for (int i = 1; i < vectorOfNodes.size(); ++i){
        if(vectorOfNodes[i].childrenId.empty()){
            planning[i] = 1;
        }
    }

    while(true) {
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
                //запуск 
            }
        }
    }

}