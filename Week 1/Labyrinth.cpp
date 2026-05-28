#include<bits/stdc++.h>
using namespace std;
int dx[]={-1,1,0,0};
int dy[]={0,0,-1,1};
char dir[]={'U','D','L','R'};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    vector<string> grid(n);
    for(int i=0;i<n;i++)
        cin>>grid[i];
    vector<vector<bool>> visited(n,vector<bool>(m,false));
    int si=-1,sj=-1,ei=-1,ej=-1;

    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(grid[i][j]=='A'){
                si=i;
                sj=j;
            }
            if(grid[i][j]=='B'){
                ei=i;
                ej=j;
            }
        }
    }
    vector<vector<char>> p(n,vector<char>(m,' '));
    queue<pair<int,int>> q;
    visited[si][sj]=true;
    q.push({si,sj});
    bool found=false;
    while(!q.empty()){
        auto [r,c]=q.front();
        q.pop();
        if(r==ei && c==ej){
            found=true;
            break;
        }
        for(int i=0;i<4;i++){
            int nr=r+dx[i];
            int nc=c+dy[i];
            if(nr<0 || nr>=n || nc<0 || nc>=m)
                continue;
            if(grid[nr][nc]=='#')
                continue;
            if(visited[nr][nc])
                continue;
            visited[nr][nc]=true;
            p[nr][nc]=dir[i];
            q.push({nr,nc});
        }
    }
    if(!found){
        cout<<"NO\n";
        return 0;
    }
    cout<<"YES\n";
    string path;
    int r=ei,c=ej;
    while(r!=si || c!=sj){
        char d=p[r][c];
        path+=d;
        if(d=='U') r++;
        else if(d=='D') r--;
        else if(d=='L') c++;
        else if(d=='R') c--;
    }
    reverse(path.begin(),path.end());
    cout<<path.size()<<"\n";
    cout<<path<<"\n";
    return 0;
}