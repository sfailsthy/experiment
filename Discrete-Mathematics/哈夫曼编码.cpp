#include <iostream>
#include <cstdio>
#include <queue>
using namespace std;
const int maxn =1000+10;

struct Node{
    int val;
    int index;
    int left,right;
    bool operator < (const Node& s) const{
        return val>s.val;
    }
};

int n;
int cnt[maxn];
char s[maxn];
string str[maxn];
Node node[2*maxn];

void find_root(){
    priority_queue<Node> que;
    for(int i=0;i<n;i++){
        node[i+1]=(Node){cnt[i],i+1,0,0};
        que.push(node[i+1]);
    }

    int num=n;
    while(que.size()>1){
        Node u=que.top();
        que.pop();
        Node v=que.top();
        que.pop();
        num++;
        node[num]=(Node){u.val+v.val,num,u.index,v.index};
        que.push(node[num]);
    }
    node[2*n-1]=que.top();
}

void solve(){
    find_root();
    Node root=node[2*n-1];
    queue<Node> que;
    que.push(root);
    str[2*n-1]="";
    while(!que.empty()){
        Node u=que.front();
        que.pop();
        if(u.left!=0){
            que.push(node[u.left]);
            str[u.left]=str[u.index]+'0';
        }

        if(u.right!=0){
            str[u.right]=str[u.index]+'1';
            que.push(node[u.right]);
        }

        if(!u.left&&!u.right){
            cout<<(char)(u.index-1+'A')<<" "<<u.val<<" "<<str[u.index]<<endl;
        }
    }
}

int main(){
    //freopen("in.txt","r",stdin);
    while(cin>>n&&n){
        for(int i=0;i<n;i++){
            cin>>s[i]>>cnt[i];
        }
        solve();
    }
    return 0;
}

/*
6
A 30
B 25
C 20
D 10
E 10
F 5

5
A 7
B 8
C 9
D 12
E 16
*/
