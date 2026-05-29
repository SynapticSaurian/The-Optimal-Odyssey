#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> adj;
vector<int> visited; //unvisited = 0, currently visiting = 1, fully processed = 2
vector<int> parent_node;
int cycle_start=-1;
int cycle_end=-1;

bool dfs(int u,int p) {
    visited[u]=1;
    parent_node[u]=p;
    for(int v:adj[u]) {
        if(v==p)
            continue;
        if(visited[v]==1) {
            cycle_start=v;
            cycle_end=u;
            return true;
        }
        if(visited[v]==0) {
            if(dfs(v,u))
                return true;
        }
    }
    visited[u]=2;
    return false;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    adj.resize(n+1);
    visited.assign(n+1,0);
    parent_node.assign(n+1,-1);
    for(int i=0;i<m;i++) {
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    for(int i=1;i<=n;i++) {
        if(visited[i]==0) {
            if(dfs(i,-1)) {
                vector<int> cycle_path;
                cycle_path.push_back(cycle_start);
                int current=cycle_end;
                while(current!=cycle_start) {
                    cycle_path.push_back(current);
                    current=parent_node[current];
                }
                cycle_path.push_back(cycle_start);
                reverse(cycle_path.begin(),cycle_path.end());
    
                cout<<cycle_path.size()<<"\n";
                for(int node:cycle_path) {
                    cout<<node<<" ";
                }
                cout<<"\n";
                return 0;
            }
        }
    }
    cout<<"IMPOSSIBLE\n";
    return 0;
}