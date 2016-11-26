#include <iostream>
#include <set>
#include <vector>
#include <cstdio>
#include <utility>
#include <algorithm>
using namespace std;
typedef pair<char,char> P;

set<P> multiply(set<P> R1,set<P> R2){
    set<P> res;
    for(set<P>::iterator it1=R1.begin();it1!=R1.end();it1++){
        P p1=*it1;
        for(set<P>::iterator it2=R2.begin();it2!=R2.end();it2++){
            P p2=*it2;
            if(p1.second==p2.first){
                res.insert(P(p1.first,p2.second));
            }
        }
    }
    return res;
}

set<P> pow(set<P> R,int n){
    if(n==1) return R;
    set<P> res=multiply(R,R);
    for(int i=0;i<n-2;i++){
        res=multiply(res,R);
    }
    return res;
}

set<P> unite(set<P> R1,set<P> R2){
    set<P> res;
    for(set<P>::iterator it=R1.begin();it!=R1.end();it++){
        res.insert(*it);
    }
    for(set<P>::iterator it=R2.begin();it!=R2.end();it++){
        res.insert(*it);
    }
    return res;
}

void closure(set<P> R){
    int len=R.size();
    set<char> s;
    for(set<P>::iterator it=R.begin();it!=R.end();it++){
        P p=*it;
        char a=p.first;
        char b=p.second;
        if(!s.count(a)){
            s.insert(a);
        }

        if(!s.count(b)){
            s.insert(b);
        }
    }

    //自反闭包
    set<P> res1;
    for(set<P>::iterator it=R.begin();it!=R.end();it++){
        res1.insert(*it);
    }

    for(set<char>::iterator it=s.begin();it!=s.end();it++){
        char a=*it;
        res1.insert(P(a,a));
    }

    cout<<"自反闭包:"<<endl;
    for(set<P>::iterator it=res1.begin();it!=res1.end();it++){
        P p=*it;
        cout<<"<"<<p.first<<","<<p.second<<">"<<endl;
    }

    set<P> res2;
    for(set<P>::iterator it=R.begin();it!=R.end();it++){
        res2.insert(*it);
    }

    //对称闭包
    for(set<P>::iterator it=R.begin();it!=R.end();it++){
        P p=*it;
        char a=p.first,b=p.second;
        res2.insert(P(b,a));
    }

    cout<<"对称闭包:"<<endl;
    for(set<P>::iterator it=res2.begin();it!=res2.end();it++){
        P p=*it;
        cout<<"<"<<p.first<<","<<p.second<<">"<<endl;
    }

    cout<<"传递闭包:"<<endl;
    set<P> res3;
    for(set<P>::iterator it=R.begin();it!=R.end();it++){
        res3.insert(*it);
    }
    for(int i=2;i<=len;i++){
        res3=unite(res3,pow(R,i));
    }
    for(set<P>::iterator it=res3.begin();it!=res3.end();it++){
        P p=*it;
        cout<<"<"<<p.first<<","<<p.second<<">"<<endl;
    }
}

int main(){
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);

    int n,kase=0;
    while(cin>>n){
        set<P> s;
        char a,b;
        for(int i=0;i<n;i++){
            cin>>a>>b;
            s.insert(P(a,b));
        }

        cout<<"Case "<<++kase<<":"<<endl;
        closure(s);
        cout<<endl;
    }
    return 0;
}
