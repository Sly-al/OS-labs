#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <sstream>

using graph = std::vector<std::vector<int> >;

struct Node {
    int id;
    std::vector<int> childId;
    std::string comToExec;
    std::string arguments;
    std::vector<int> parentId;
};

void JsonToVector(const std::string& nameFileJson, std::vector<Node> & vectorOfNodes, std::vector<int>& planning){
    std::string pathToFile = "/home/alex/Рабочий стол/OS-labs/KP/" + nameFileJson;
    std::ifstream file(pathToFile);
    Json::Value actualJson;
    Json::Reader reader;

    reader.parse(file, actualJson);

    vectorOfNodes.resize(actualJson.size());
    planning.resize(actualJson.size());  

    for (unsigned int j = 0; j < actualJson.size(); ++j){
        vectorOfNodes[j].id = j;
        std::string childIdString = actualJson["Node"+ std::to_string(j)]["childId"].asString();
        std::stringstream iss( childIdString );
        int number;
        while ( iss >> number){
            if(number != -1){
                vectorOfNodes[number].parentId.push_back(j);   
                vectorOfNodes[j].childId.push_back(number);
            }
        }
       
        vectorOfNodes[j].comToExec = actualJson["Node"+ std::to_string(j)]["comToExec"].asString();
    }
    // for (int i = 0; i < vectorOfNodes.size(); ++i){
    //     for (int j = 0; j < vectorOfNodes[i].childrenId.size(); ++j)
    //     std::cout<< "Id = " << i << ' ' << "Result = " << vectorOfNodes[i].childrenId[j] <<'\n';
    // }
    return;

}

#endif