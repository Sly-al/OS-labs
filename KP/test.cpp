#include <bits/stdc++.h>
#include <cstdlib>
// struct {



// };


// for executing programms
// FILE *fp;
//     char line[1000];
//     std::string result;
//     fp = popen("dir", "r");
//     while (fgets(line, 1000, fp))
//         result += line;
//     std::cout << result;


using graph = std::vector<std::vector<int> >;
const int maxn = 1e5;
std::vector<int> component(maxn); // тут будут номера компонент

int flag = 0;
int zam=0;
bool dfs(int u, int prev, const graph & g, std::vector<int> & vizit){
    if (vizit[u]==1){
        flag = 1;
        zam = u;
        return true;
    }
    vizit[u] = 1;
    for (int v: g[u]){
           if ( v!= prev && dfs(v, u, g, vizit)){
               if (u == zam){
                   flag = 0;
               }
               return true;
           }
    }
    return false;
}

int main(){
    int n, m;
    std::cin >> n >> m;
    graph g(n);
    for (int i = 0; i < m; ++i){
        int u, v;
        std::cin >> u >> v;
        --u;
		--v;
        g[u].push_back(v); 
    }
	std::vector<int> vizit(n);
	bool cycle = dfs(0,-1, g, vizit);

	std::cout<< "Check = " << cycle << '\n';
    return 0;
}