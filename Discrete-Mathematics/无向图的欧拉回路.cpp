//created by sfailsthy 2016/12/1 0:29
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <stack>
using namespace std;
const int maxn =100+10;

int n,m;
stack<int> s;
vector<int> G[maxn];
int mark[maxn][maxn];

void dfs(int u){
    s.push(u);
    for(int i=0;i<G[u].size();i++){
        int v=G[u][i];
        if(mark[u][v]){
            mark[u][v]=mark[v][u]=0;
            dfs(v);
            break;
        }
    }
}

void fleury(int ss){
    while(!s.empty()){
        s.pop();
    }

    s.push(ss);//将起点放入Euler路径中
    while(!s.empty()){
        int u=s.top();
        //s.pop();
        bool bridge=1;
        for(int i=0;i<G[u].size();i++){//试图搜索一条边不是桥
            int v=G[u][i];
            if(mark[u][v]){
                bridge=0;
                break;
            }
        }

        if(bridge){// 如果没有点可以扩展，输出并出栈
            printf("%d ",u+1);
            s.pop();
        }
        else{//否则继续搜索欧拉路径
            s.pop();
            dfs(u);

        }
    }
}

int main(){
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    while(cin>>n>>m){
        for(int i=0;i<n;i++){
            G[i].clear();
        }
        memset(mark,0,sizeof(mark));
        for(int i=0;i<m;i++){
            int a,b;
            cin>>a>>b;
            a--;
            b--;
            mark[a][b]=mark[b][a]=1;
            G[a].push_back(b);
            G[b].push_back(a);
        }
        fleury(0);
        cout<<endl;
    }
    return 0;
}
