#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cstdio>
using namespace std;
typedef pair<char,char> P;

int n;//n是集合中的元素个数
char opt;//集合中的运算符
char e;//幺元
vector<char> elements;//保存集合的元素
map<P,char> table;//保存运算表<a,b>→(a*b)

//判断元素e是否是幺元
bool checkIdentity(char e){
    for(vector<char>::iterator it=elements.begin();it!=elements.end();it++){
        char a=*it;
        char ae=table[P(a,e)];
        char ea=table[P(e,a)];
        if((ae!=ea)||(ae!=a)||(ea!=a)){
            return false;
        }
    }
    return true;
}

//判断元素a是否存在逆元
bool checkInvertible(char a){
    for(vector<char>::iterator it=elements.begin();it!=elements.end();it++){
        char b=*it;
        char ab=table[P(a,b)];
        char ba=table[P(b,a)];
        if((ab==ba)&&(ab==e)&&(ba==e)){
            return true;
        }
    }
    return false;
}

//判断是否是群
bool check(){
    //封闭性
    for(vector<char>::iterator it1=elements.begin();it1!=elements.end();it1++){
        char a=*it1;
        for(vector<char>::iterator it2=elements.begin();it2!=elements.end();it2++){
            char b=*it2;
            char c=table[P(a,b)];
            bool flag=false;
            for(vector<char>::iterator it=elements.begin();it!=elements.end();it++){
                if(c==*it){
                    flag=true;
                    break;
                }
            }
            if(!flag){
                return false;
            }
        }
    }

    //结合律
    for(vector<char>::iterator it1=elements.begin();it1!=elements.end();it1++){
        char a=*it1;
        for(vector<char>::iterator it2=elements.begin();it2!=elements.end();it2++){
            char b=*it2;
            for(vector<char>::iterator it3=elements.begin();it3!=elements.end();it3++){
                char c=*it3;
                char ab=table[P(a,b)],bc=table[P(b,c)];
                if(table[P(ab,c)]!=table[P(a,bc)]){
                    return false;
                    //不满足结合律
                }
            }
        }
    }

    //寻找幺元
    bool flag=false;
    for(vector<char>::iterator it=elements.begin();it!=elements.end();it++){
        char c=*it;
        if(checkIdentity(c)){
            flag=true;
            e=c;
            break;
        }
    }

    if(!flag){
        return false;
        //不存在幺元
    }
    //否则幺元是e

    for(vector<char>::iterator it=elements.begin();it!=elements.end();it++){
        char a=*it;
        if(!checkInvertible(a)){
            //a不存在逆元，不是群
            return false;

        }
    }

    //到这里，群的条件都已满足
    return true;

}

int main(){
    cout<<"请输入代数系统的阶数:"<<endl;
    while(cin>>n&&n){
        elements.clear();
        cout<<"请输入集合中的元素:"<<endl;
        for(int i=0;i<n;i++){
            char c;
            cin>>c;
            elements.push_back(c);
        }
        cout<<"输入运算符:"<<endl;
        cin>>opt;
        cout<<"请输入运算表:"<<endl;
        for(vector<char>::iterator it1=elements.begin();it1!=elements.end();it1++){
            char a=*it1;
            for(vector<char>::iterator it2=elements.begin();it2!=elements.end();it2++){
                char b=*it2,c;
                cin>>c;
                table[P(a,b)]=c;
            }
        }

        bool flag=check();
        if(flag){
            cout<<"YES"<<endl;
        }
        else{
            cout<<"NO"<<endl;
        }
    }
    return 0;
}
