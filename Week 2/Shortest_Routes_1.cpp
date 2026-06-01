#include<bits/stdc++.h>
using namespace std;

vector<long long> dijkstra(int start,int n,vector<vector<pair<int,long long>>>& graph){
    vector<long long> dist(n+1,LLONG_MAX);
    dist[start]=0;
    priority_queue<pair<long long,int>,vector<pair<long long,int>>,greater<pair<long long,int>>> pq;
    pq.push({0,start});
    while(!pq.empty()){
        long long d=pq.top().first;
        int u=pq.top().second;
        pq.pop();
        if(d>dist[u])continue;
        for(auto& edge:graph[u]){
            int v=edge.first;
            long long w=edge.second;
            if(dist[u]+w<dist[v]){
                dist[v]=dist[u]+w;
                pq.push({dist[v],v});
            }
        }
    }
    return dist;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    vector<vector<pair<int,long long>>> graph(n+1);
    for(int i=0;i<m;i++){
        int a,b;
        long long c;
        cin>>a>>b>>c;
        graph[a].push_back({b,c}); 
    }
    vector<long long> d=dijkstra(1,n,graph);
    for(int i=1;i<=n;i++){
        cout<<d[i]<<" ";
    }
    cout<<"\n";
    return 0;
}