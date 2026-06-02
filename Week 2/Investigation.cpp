#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    vector<vector<pair<int,long long>>> adj(n+1);
    for(int i=0;i<m;i++){
        int a,b;
        long long c;
        cin>>a>>b>>c;
        adj[a].push_back({b,c});
    }
    const long long MOD=1e9+7;
    vector<long long> dist(n+1,LLONG_MAX);
    vector<long long> routes(n+1,0);
    vector<int> min_f(n+1,INT_MAX);
    vector<int> max_f(n+1,-1);
    priority_queue<pair<long long,int>,vector<pair<long long,int>>,greater<pair<long long,int>>> pq;
    dist[1]=0;
    routes[1]=1;
    min_f[1]=0;
    max_f[1]=0;
    pq.push({0,1});
    while(!pq.empty()){
        auto [d,u]=pq.top();
        pq.pop();
        if(d>dist[u])
            continue;
        for(auto& [v,w]:adj[u]){
            if(dist[u]+w<dist[v]){
                dist[v]=dist[u]+w;
                routes[v]=routes[u];
                min_f[v]=min_f[u]+1;
                max_f[v]=max_f[u]+1;
                pq.push({dist[v],v});
            }
            else if(dist[u]+w==dist[v]){
                routes[v]=(routes[v]+routes[u])%MOD;
                min_f[v]=min(min_f[v],min_f[u]+1);
                max_f[v]=max(max_f[v],max_f[u]+1);
            }
        }
    }
    cout<<dist[n]<<" "<<routes[n]<<" "<<min_f[n]<<" "<<max_f[n]<<"\n";
    return 0;
}