#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    vector<tuple<long long,int,int>> edges(m);
    for(int i=0;i<m;i++){
        int a,b;
        long long c;
        cin>>a>>b>>c;
        edges[i]={c,a,b};
    }
    sort(edges.begin(),edges.end());
    vector<int> parent(n+1);
    vector<int> rnk(n+1,0);
    for(int i=1;i<=n;i++)
        parent[i]=i;

    auto find_set=[&](auto& self,int i) -> int {
        if(parent[i]==i) return i;
        return parent[i]=self(self,parent[i]);
    };
    long long total_cost=0;
    int edges_added=0;
    for(auto& [w, u, v]:edges){
        int root_u=find_set(find_set, u);
        int root_v=find_set(find_set, v);
        if(root_u!=root_v){
            if(rnk[root_u]<rnk[root_v])
                parent[root_u]=root_v;
            else if(rnk[root_u]>rnk[root_v]) 
                parent[root_v]=root_u;
            else{
                parent[root_v]=root_u;
                rnk[root_u]++;
            }
            total_cost+=w;
            edges_added++;
            if(edges_added==n-1)
                break;
        }
    }
    if(edges_added==n-1) 
        cout<<total_cost<<"\n";
    else 
        cout<<"IMPOSSIBLE\n";
    return 0;
}