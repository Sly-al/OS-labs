#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <sstream>

using graph = std::vector<std::vector<int> >;
const int NOTREADY = 0;
const int READY = 1;
const int INWORKING = 2;
const int FINISH = 3;

struct Node {
    int id;
    std::vector<int> childId;
    std::string comToExec;
    std::string arguments;
    std::vector<int> parentId;
};

void JsonToVector(const std::string& nameFileJson, std::vector<Node> & vectorOfNodes, std::vector<int>& planning){
    std::string pathToFile = "/home/alex/Рабочий стол/OS-labs/KP/json/" + nameFileJson +".json";
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

    return;

}

#endif