#include<bits/stdc++.h>
using namespace std;

int main() {
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
        graph[b].push_back({a,c});
    }
    vector<long long> dist(n+1,LLONG_MAX);
    vector<int> parent(n+1,-1);
    priority_queue<pair<long long,int>,vector<pair<long long,int>>,greater<pair<long long,int>>> pq;
    dist[1]=0;
    pq.push({0,1});
    while(!pq.empty()){
        long long d=pq.top().first;
        int u=pq.top().second;
        pq.pop();
        if(d>dist[u])
            continue;
        for(auto& edge:graph[u]){
            int v=edge.first;
            long long w=edge.second;
            if(dist[u]+w<dist[v]){
                dist[v]=dist[u]+w;
                parent[v]=u;
                pq.push({dist[v],v});
            }
        }
    }
    if(dist[n]==LLONG_MAX){
        cout<<-1<<"\n";
        return 0;
    }
    vector<int> path;
    for(int v=n;v!=-1;v=parent[v]){
        path.push_back(v);
    }
    reverse(path.begin(),path.end());

    for(int i=0;i<path.size();i++){
        cout<<path[i]<<" ";
    }
    cout<<"\n";
    return 0;
}