#include <bits/stdc++.h>

using graph = std::vector<std::vector<int> >;
const int maxn = 1e5;
std::vector<int> component(maxn); 
int flag = 0;
int zam=0;

void DFS(int v, int num, const graph& matr) {
    component[v] = num;
    for (int u : matr[v]){
        if (!component[u]){
            DFS(u, num, matr);
		}
	}
}

bool CheckOneComp(const graph& matr){
	int num = 0;
	for (int v = 0; v < matr.size(); v++){
    	if (!component[v]){
        	DFS(v, ++num, matr);
		}
	}
	return num == 1;
}


bool FindCycle(int u, int prev, const graph & g, std::vector<int> & vizit){
    if (vizit[u]==1){
        flag = 1;
        zam = u;
        return true;
    }
    vizit[u] = 1;
    for (int v: g[u]){
           if ( v!= prev && FindCycle(v, u, g, vizit)){
               if (u == zam){
                   flag = 0;
               }
               return true;
           }
    }
    return false;
}