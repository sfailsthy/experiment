#include <iostream>
#include <set>
#include <vector>
#include <cstdio>
#include <utility>
#include <algorithm>
#include <map>
using namespace std;
typedef pair<char,char> P;

void quotientSet(vector<P> vec){
    set<char> s;
    vector<char> v;
    int len=vec.size();

    for(int i=0;i<len;i++){
        char c1=vec[i].first;
        char c2=vec[i].second;
        if(!s.count(c1)){
            s.insert(c1);
            v.push_back(c1);
        }

        if(!s.count(c2)){
            s.insert(c2);
            v.push_back(c2);
        }
    }

    map<char,vector<char> > m;
    vector<char> res;
    while(v.size()){
        char a=v[0];
        res.push_back(a);
        for(int i=0;i<len;i++){
            P p=vec[i];
            if(p.first!=a) continue;
            char b=p.second;
            m[a].push_back(b);
            vector<char>::iterator it=lower_bound(v.begin(),v.end(),b);
            v.erase(it);
        }
        cout<<endl;
    }

    for(int i=0;i<res.size();i++){
        char a=res[i];
        cout<<a<<":";
        for(int j=0;j<m[a].size();j++){
            char b=m[a][j];
            cout<<" "<<b;
        }
        cout<<endl;
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
        quotientSet(vec);
        cout<<endl;
    }
    return 0;
}
