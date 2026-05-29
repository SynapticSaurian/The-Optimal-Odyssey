#include<bits/stdc++.h>
using namespace std;

bool check_component(int start,vector<vector<int>>& adj,vector<int>& team) {
    queue<int> q;
    q.push(start);
    team[start]=1; 
    
    while(!q.empty()) {
        int u=q.front();
        q.pop();
        for(int v:adj[u]) {
            if(team[v]==0) {
                team[v]=3-team[u];
                q.push(v);
            } 
            else if(team[v]==team[u]) {
                return false; 
            }
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);   
    int n,m;
    cin>>n>>m;  
    vector<vector<int>> adj(n+1);
    for(int i=0;i<m;i++) {
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> team(n+1,0); 
    for(int i=1;i<=n;i++) {
        if(team[i]==0) {
            if(!check_component(i,adj,team)) {
                cout<<"IMPOSSIBLE\n";
                return 0;
            }
        }
    }
    for(int i=1;i<=n;i++) {
        cout<<team[i]<<" ";
    }
    cout<<"\n";
    return 0;
}