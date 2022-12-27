#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <jsoncpp/json/json.h>
#include <fstream>

struct Node {
    int id;
    int childId;
    std::string comToExec;
    std::string arguments;
    std::vector<int> parentId;
};

void JsonToVector(const std::string& nameFileJson, std::vector<Node> & vectorOfNodes, std::vector<int>& planning){

    std::ifstream file(nameFileJson);
    Json::Value actualJson;
    Json::Reader reader;

    reader.parse(file, actualJson);

    vectorOfNodes.resize(actualJson.size());
    planning.resize(actualJson.size());     

    for (int j = 0; j < actualJson.size(); ++j){
        vectorOfNodes[j].id = j;
        int childId = actualJson["Node"+ std::to_string(j)]["childId"].asInt();
        std::cout<< "childId = " << childId << ' ';
        vectorOfNodes[j].comToExec = actualJson["Node"+ std::to_string(j)]["comToExec"].asString();
        
        if (childId != -1){
            vectorOfNodes[childId].parentId.push_back(j);   
            vectorOfNodes[j].childId = childId;
        }
    }

    // for (int i = 0; i < vectorOfNodes.size(); ++i){
    //     for (int j = 0; j < vectorOfNodes[i].childrenId.size(); ++j)
    //     std::cout<< "Id = " << i << ' ' << "Result = " << vectorOfNodes[i].childrenId[j] <<'\n';
    // }
    return;

}

#endif