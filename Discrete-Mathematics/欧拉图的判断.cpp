#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
using namespace std;
const int maxn =100+10;
//并查集
int par[maxn];
int rank[maxn];

void init(int n){
    for(int i=0;i<n;i++){
        par[i]=i;
        rank[i]=0;
    }
}

int find(int x){
    if(par[x]==x){
        return x;
    }
    else{
        return par[x]=find(par[x]);
    }
}

void unite(int x,int y){
    x=find(x);
    y=find(y);
    if(x==y) return;

    if(rank[x]<rank[y]){
        par[x]=y;
    }
    else{
        par[y]=x;
        if(rank[x]==rank[y]){
            rank[x]++;
        }
    }
}

bool same(int x,int y){
    return find(x)==find(y);
}

int n,m;
vector<int> G[maxn];

int main(){
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    while(cin>>n>>m){
        for(int i=0;i<m;i++){
            int a,b;
            cin>>a>>b;
            G[a].push_back(b);
            G[b].push_back(a);
            unite(a,b);
        }
        bool flag=true;
        for(int i=0;i<n;i++){
            for(int j=0;j<n&&j!=i;j++){
                if(!same(i,j)){
                    flag=false;
                    break;
                }
            }
        }

        for(int i=0;i<n;i++){
            int p=G[i].size();
            if(p%2){
                flag=false;
                break;
            }
        }

        if(flag){
            cout<<"yes"<<endl;
        }
        else{
            cout<<"no"<<endl;
        }
    }
    return 0;
}
