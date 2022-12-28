#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include "parser.h"

const int maxn = 1e5;
std::vector<int> component(maxn);
int n;
std::vector<char> color;
std::vector<int> parent;
int cycle_start, cycle_end; 

void DFS(int v, int num, const graph& matr) {
    component[v] = num;
    for (int u : matr[v]){
        if (!component[u]){
            DFS(u, num, matr);
		}
	}
	return;
}

bool CheckOneComp(const graph& matr){
	int num = 0;
	for (unsigned long v = 0; v < matr.size(); v++){
    	if (!component[v]){
        	DFS(v, ++num, matr);
		}
	}
	return num == 1;
}

bool dfs(int v, const std::vector<Node>&matr) {
    color[v] = 1;
    for (int u : matr[v].childId) {
        if (color[u] == 0) {
            parent[u] = v;
            if (dfs(u, matr))
                return true;
        } else if (color[u] == 1) {
            cycle_end = v;
            cycle_start = u;
            return true;
        }
    }
    color[v] = 2;
    return false;
}

bool FindCycle(const std::vector<Node>&matr) {
	int n = matr.size();
    color.assign(n, 0);
    parent.assign(n, -1);
    cycle_start = -1;

    for (int v = 0; v < n; v++) {
        if (color[v] == 0 && dfs(v, matr))
            break;
    }

    if (cycle_start == -1) {
    	return false;
    }
	return true;
}

#endif