#include <iostream>
#include <set>
#include <vector>
#include <cstdio>
#include <utility>
#include <algorithm>
using namespace std;
typedef pair<char,char> P;

void check(vector<P> vec){
    sort(vec.begin(),vec.end());
    int len=vec.size();

    set<char> s;
    for(int i=0;i<len;i++){
        char a=vec[i].first;
        char b=vec[i].second;
        if(!s.count(a)){
            s.insert(a);
        }

        if(!s.count(b)){
            s.insert(b);
        }
    }

    bool flag1=true,flag2=true,flag3=true;

    //自反
    for(set<char>::iterator it=s.begin();it!=s.end();it++){
        char a=*it;
        int index=lower_bound(vec.begin(),vec.end(),P(a,a))-vec.begin();
        if(vec[index].first!=a||vec[index].second!=a){
            flag1=false;
            break;
        }
    }

    if(flag1){
        cout<<"是自反的"<<endl;
    }
    else{
        cout<<"不是自反的"<<endl;
    }


    //对称
    for(int i=0;i<len;i++){
        P p=vec[i];
        char a=p.first,b=p.second;
        int index=lower_bound(vec.begin(),vec.end(),P(b,a))-vec.begin();
        if(vec[index].first!=b||vec[index].second!=a){
            flag2=false;
            break;
        }
    }

    if(flag2){
        cout<<"是对称的"<<endl;
    }

    else{
        cout<<"不是对称的"<<endl;
    }


    //传递
    for(int i=0;i<len;i++){
        for(int j=0;j<len&&j!=i;j++){
            P p1=vec[i],p2=vec[j];
            if(p1.second!=p2.first){
                continue;
            }

            char a=p1.first,b=p2.second;
            int index=lower_bound(vec.begin(),vec.end(),P(a,b))-vec.begin();
            if(vec[index].first!=a||vec[index].second!=b){
                flag3=false;
                i=j=len;
            }

        }
    }

    if(flag3){
        cout<<"是传递的"<<endl;
    }
    else{
        cout<<"不是传递的"<<endl;
    }


    if(flag1&&flag2&&flag3){
        cout<<"是等价的"<<endl;
    }

    else{
        cout<<"不是等价的"<<endl;
    }
}

int main(){
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);

    int n,kase=0;
    while(cin>>n){
        vector<P> vec;
        char a,b;
        for(int i=0;i<n;i++){
            cin>>a>>b;
            vec.push_back(P(a,b));
        }

        cout<<"Case "<<++kase<<":"<<endl;
        check(vec);
        cout<<endl<<endl;
    }

    return 0;
}
