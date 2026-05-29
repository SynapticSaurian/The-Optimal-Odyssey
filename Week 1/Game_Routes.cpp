#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    vector<vector<int>> adj(n+1);
    vector<int> in_degree(n+1,0);
    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
        in_degree[v]++;
    }
    vector<long long> dp(n+1,0);
    dp[1]=1;
    queue<int> q;
    for(int i=1;i<=n;i++){
        if(in_degree[i]==0){
            q.push(i);
        }
    }
    long long MOD=1000000007;
    while(!q.empty()){
        int u=q.front();
        q.pop();
        for(int v:adj[u]){
            dp[v]=(dp[v]+dp[u])%MOD;
            in_degree[v]--;
            if(in_degree[v]==0){
                q.push(v);
            }
        }
    }
    cout<<dp[n]<<"\n";
    return 0;
}