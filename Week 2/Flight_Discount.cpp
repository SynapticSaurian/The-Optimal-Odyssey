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
    vector<vector<long long>> dist(n+1,vector<long long>(2,LLONG_MAX));
    //{distance,city,coupon_used}
    priority_queue<tuple<long long,int,int>,vector<tuple<long long,int,int>>,greater<tuple<long long,int,int>>> pq;
    dist[1][0]=0;
    pq.push({0,1,0});
    while(!pq.empty()){
        auto [d,u,coupon_used]=pq.top();
        pq.pop();
        if(d>dist[u][coupon_used])
            continue;
        for(auto& [v,w]:adj[u]){
            if(coupon_used==0){
                //Option 1: Dont use coupon now
                if(dist[u][0]+w<dist[v][0]){
                    dist[v][0]=dist[u][0]+w;
                    pq.push({dist[v][0],v,0});
                }
                //Option 2: Use coupon right now
                if(dist[u][0]+(w/2)<dist[v][1]){
                    dist[v][1]=dist[u][0]+(w/2);
                    pq.push({dist[v][1],v,1});
                }
            }   
            else{
                if(dist[u][1]+w<dist[v][1]){
                    dist[v][1]=dist[u][1]+w;
                    pq.push({dist[v][1],v,1});
                }
            }
        }
    }
    cout<<dist[n][1]<<"\n";
    return 0;
}