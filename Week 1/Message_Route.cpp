#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    vector<vector<int>> adj(n+1);
    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> p(n+1,-1);
    vector<bool> visited(n+1,false);
    queue<int> q;
    q.push(1);
    visited[1]=true;
    while(!q.empty()){
        int u=q.front();
        q.pop();
        if(u==n)
            break;
        for(int v:adj[u]){
            if(!visited[v]){
                visited[v]=true;
                p[v]=u;
                q.push(v);
            }
        }
    }
    if(!visited[n]){
        cout<<"IMPOSSIBLE\n";
        return 0;
    }
    vector<int> path;
    int curr=n;
    while(curr!=-1){
        path.push_back(curr);
        curr=p[curr];
    }
    reverse(path.begin(),path.end());
    cout<<path.size()<<"\n";
    for(int i=0;i<path.size();i++){
        cout<<path[i]<<" ";
    }
    cout<<"\n";
    return 0;
}