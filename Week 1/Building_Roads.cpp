#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    vector<vector<int>>adj(n+1);
    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<bool> visited(n+1,false);
    vector<int> components;
    for(int i=1;i<=n;i++){
        if(!visited[i]){
            components.push_back(i);
            
            queue<int> q;
            visited[i]=true;
            q.push(i);
            while(!q.empty()){
                int s=q.front();
                q.pop();
                for(auto u:adj[s]){
                    if(visited[u])
                        continue;
                    visited[u]=true;
                    q.push(u);
                }
            }
        }
    }
    int roads=components.size()-1;
    cout<<roads<<"\n";
    for(int i=0;i<roads;i++){
        cout<<components[i]<<" "<<components[i+1]<<"\n";
    }
    return 0;
}