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
    vector<vector<int>> m_time(n,vector<int>(m,1e9));
    vector<vector<int>> p_time(n,vector<int>(m,1e9));
    vector<vector<char>> parent_move(n,vector<char>(m,' '));
    queue<pair<int,int>> mq;
    queue<pair<int,int>> pq;
    int start_x=-1,start_y=-1;
    for(int i=0;i<n;i++){
        cin>>grid[i];
        for(int j=0;j<m;j++){
            if(grid[i][j]=='M'){
                mq.push({i,j});
                m_time[i][j]=0;
            }
            else if(grid[i][j]=='A'){
                start_x=i;
                start_y=j;
                pq.push({i,j});
                p_time[i][j]=0;
            }
        }
    }
    while(!mq.empty()){
        auto [x,y]=mq.front();
        mq.pop();
        for(int i=0;i<4;i++){
            int nx=x+dx[i],ny=y+dy[i];
            if(nx>=0&&nx<n&&ny>=0&&ny<m&&grid[nx][ny]!='#'&&m_time[nx][ny]==1e9){
                m_time[nx][ny]=m_time[x][y]+1;
                mq.push({nx,ny});
            }
        }
    }
    int end_x=-1,end_y=-1;
    while(!pq.empty()){
        auto [x,y]=pq.front();
        pq.pop();
        if(x==0||x==n-1||y==0||y==m-1){
            end_x=x;
            end_y=y;
            break;
        }
        for(int i=0;i<4;i++){
            int nx=x+dx[i],ny=y+dy[i];
            if(nx>=0&&nx<n&&ny>=0&&ny<m&&grid[nx][ny]!='#'&&p_time[nx][ny]==1e9){
                int next_time=p_time[x][y]+1;
                if(next_time<m_time[nx][ny]){
                    p_time[nx][ny]=next_time;
                    parent_move[nx][ny]=dir[i];
                    pq.push({nx,ny});
                }
            }
        }
    }
    if(end_x==-1){
        cout<<"NO\n";
        return 0;
    }
    cout<<"YES\n";
    string path="";
    int cx=end_x,cy=end_y;
    while(cx!=start_x||cy!=start_y){
        char move=parent_move[cx][cy];
        path+=move;
        if(move=='U')
            {cx++;}
        else if(move=='D')
            {cx--;}
        else if(move=='L')
            {cy++;}
        else if(move=='R')
            {cy--;}
    }
    reverse(path.begin(),path.end());
    cout<<path.size()<<"\n"<<path<<"\n";
    return 0;
}