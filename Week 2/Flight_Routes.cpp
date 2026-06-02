#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m,k;
    cin>>n>>m>>k;
    vector<vector<pair<int,long long>>> adj(n+1);
    for(int i=0;i<m;i++){
        int a,b;
        long long c;
        cin>>a>>b>>c;
        adj[a].push_back({b,c});
    }
    vector<priority_queue<long long>> best_k(n+1);
    priority_queue<pair<long long,int>,vector<pair<long long,int>>,greater<pair<long long,int>>> pq;
    best_k[1].push(0);
    pq.push({0,1});
    while(!pq.empty()){
        auto [d,u]=pq.top();
        pq.pop();
        if(d>best_k[u].top())
            continue;
        for(auto& [v,w]:adj[u]){
            long long next_d=d+w;
            if(best_k[v].size()<k){
                best_k[v].push(next_d);
                pq.push({next_d,v});
            }
            else if(next_d<best_k[v].top()){
                best_k[v].pop();
                best_k[v].push(next_d);
                pq.push({next_d,v});
            }
        }
    }
    vector<long long> ans;
    while(!best_k[n].empty()){
        ans.push_back(best_k[n].top());
        best_k[n].pop();
    }
    reverse(ans.begin(),ans.end());
    for(int i=0;i<k;i++){
        cout<<ans[i]<<" ";
    }
    cout<<"\n";
    return 0;
}