#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    vector<int> parent(n+1),sz(n+1,1);
    for(int i=1;i<=n;i++)
        parent[i]=i;
    auto find_set=[&](auto& self,int i)->int{
        if(parent[i]==i)
            return i;
        return parent[i]=self(self,parent[i]);
    };
    int num_comp=n,max_sz=1;
    for(int i=0;i<m;i++){
        int a,b;
        cin>>a>>b;
        int root_a=find_set(find_set,a);
        int root_b=find_set(find_set,b);
        if(root_a!=root_b){
            if(sz[root_a]<sz[root_b])swap(root_a,root_b);
            parent[root_b]=root_a;
            sz[root_a]+=sz[root_b];
            num_comp--;
            max_sz=max(max_sz,sz[root_a]);
        }
        cout<<num_comp<<" "<<max_sz<<"\n";
    }
    return 0;
}