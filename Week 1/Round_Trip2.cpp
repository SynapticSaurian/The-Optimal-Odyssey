#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> adj;
vector<int> visited;
vector<int> p;
int cycle_start=-1,cycle_end=-1;

bool dfs(int u){
    visited[u]=1;
    for(int v:adj[u]){
        if(visited[v]==0){
            p[v]=u;
            if(dfs(v))
                return true;
        }
        else if(visited[v]==1){
            cycle_start=v;
            cycle_end=u;
            return true;
        }
    }
    visited[u]=2;
    return false;
}
int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    adj.resize(n+1);
    visited.assign(n+1,0);
    p.assign(n+1,-1);
    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
    }
    for(int i=1;i<=n;i++){
        if(visited[i]==0){
            if(dfs(i)){
                vector<int> path;
                int curr=cycle_end;
                while(curr!=cycle_start){
                    path.push_back(curr);
                    curr=p[curr];
                }
                path.push_back(cycle_start);
                reverse(path.begin(),path.end());
                path.push_back(cycle_start);
                cout<<path.size()<<"\n";
                for(int i=0;i<path.size();i++){
                    cout<<path[i]<<" ";
                }
                cout<<"\n";
                return 0;
            }
        }
    }
    cout<<"IMPOSSIBLE\n";
    return 0;
}