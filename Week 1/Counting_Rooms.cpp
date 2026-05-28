#include<bits/stdc++.h>
using namespace std;
int dx[]={-1,1,0,0};
int dy[]={0,0,-1,1};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n,m;
    cin>>n>>m;
    vector<string> board(n);
    vector<vector<bool>> visited(n,vector<bool>(m,false));
    for(int i=0;i<n;i++){
        cin>>board[i];
    }
    int rooms=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(board[i][j]=='.'&&(!visited[i][j])){
                rooms++;
                
                queue<pair<int,int>> q;
                visited[i][j]=true;
                q.push({i,j});
                while(!q.empty()) {
                    auto [r,c]=q.front();
                    q.pop();
                    for(int dir=0;dir<4;dir++){
                        int nr=r+dx[dir];
                        int nc=c+dy[dir];
                        if(nr<0 || nr>=n || nc<0 || nc>=m)
                            continue;
                        if(board[nr][nc]=='#')
                            continue;
                        if(visited[nr][nc])
                            continue;
                        visited[nr][nc]=true;
                        q.push({nr,nc});
                    }
                }
            }
        }
    }
    cout<<rooms<<"\n";
    return 0;
}