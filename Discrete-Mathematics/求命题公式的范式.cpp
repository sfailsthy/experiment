#include <iostream>
#include <stack>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;
const int maxn =1000+10;

//判断运算符优先级
int judge(char op1,char op2){
    if(op1=='#'){
        if(op2=='#') return 0;
        else return -1;
    }

    else if(op1=='!'){
        if(op2=='('){
            return -1;
        }
        else return 1;
    }

    else if(op1=='^'){
        if(op2=='!'||op2=='('){
            return -1;
        }
        else return 1;
    }

    else if(op1=='|'){
        if(op2=='!'||op2=='^'||op2=='('){
            return -1;
        }
        else return 1;
    }

    else if(op1=='>'){
        if(op2=='!'||op2=='^'||op2=='|'||op2=='('){
            return -1;
        }
        else return 1;
    }

    else if(op1=='-'){
        if(op2=='!'||op2=='^'||op2=='|'||op2=='>'||op2=='('){
            return -1;
        }
        else return 1;
    }

    else if(op1=='('){
        if(op2==')') return 0;
        else return -1;
    }

    else if(op1==')'){
        return 1;
    }
}

//计算真值
int cacl(int p,char op,int q){
    switch(op){
    case '^':
        return p&q;
    case '|':
        return p||q;
    case '>':
        return (!p)||q;
    case '-':
        return ((!p)||q)&((!q)||p);
    }
}

int solve(char *str){
    map<char,int> m;
    //建立映射，T→1，F→0
    m['T']=1;
    m['F']=0;

    char exp[maxn];
    int n=strlen(str);
    for(int i=0;i<n;i++){
        exp[i]=str[i];
    }
    exp[n]='#';
    exp[n+1]='\0';

    stack<int> numStack;//存储操作数的栈
    stack<char> opStack;//存储运算符的栈
    opStack.push('#');

    int i=0;
    while(exp[i]!='\0'){
        if(isalpha(exp[i])){
            //数字直接进栈
            numStack.push(m[exp[i]]);
            i++;
        }

        else{
            char op=exp[i];
            //判断当前运算符与栈顶运算符优先级大小的关系

            //栈顶运算符优先级大于当前运算符的优先级，需要进行计算
            if(judge(opStack.top(),op)==1){
                //如果是否定联结词，从栈顶取一个数即可
                if(opStack.top()=='!'){
                    int p=numStack.top();
                    numStack.pop();
                    numStack.push(!p);
                    opStack.pop();
                }

                //否则取两个数，注意顺序
                else{
                    int q=numStack.top();
                    numStack.pop();
                    int p=numStack.top();
                    numStack.pop();

                    int res=cacl(p,opStack.top(),q);
                    numStack.push(res);
                    opStack.pop();
                }
            }

            //栈顶运算符优先级等于当前运算符的优先级，可能是括号匹配，也可能是运算结束
            else if(judge(opStack.top(),op)==0){
                if(op==')'){
                    opStack.pop();
                    i++;
                }
                else{
                    return numStack.top();
                }
            }

            //栈顶运算符优先级小于当前运算符的优先级，当前运算符进栈
            else{
                opStack.push(op);
                i++;
            }
        }
    }
}

//正整数n的二进制表示
vector<int> binary(int n){
    vector<int> res;
    while(n!=0){
        res.push_back(n%2);
        n=n/2;
    }
    reverse(res.begin(),res.end());
    return res;
}

//求命题公式的主析取范式、主合取范式
void result(char *str){
    map<char,int> m;
    m['T']=1;
    m['F']=0;

    char c[2];
    c[0]='F';
    c[1]='T';

    set<char> s;
    vector<char> vec;//保存所有不同的命题变元
    for(int i=0;str[i]!='\0';i++){
        if(isalpha(str[i])){
           if(!s.count(str[i])){
               //集合中无该命题变元，进入vec中
               vec.push_back(str[i]);
               s.insert(str[i]);
           }
        }
    }

    int cnt=s.size();//不同命题变元的个数

    string str1[maxn],str2[maxn];
    int num1=0,num2=0;
    //二进制枚举所有的真值解释
    for(int i=0;i<(1<<cnt);i++){
        if(i==0){
            for(int j=0;j<cnt;j++){
                m[vec[j]]=0;
            }
        }

        else{
            int x=i;
            vector<int> v=binary(x);
            //不足cnt位时，前补0
            int p=cnt-v.size();
            for(int j=0;j<p;j++){
                m[vec[j]]=0;
            }
            for(int j=p;j<cnt;j++){
                m[vec[j]]=v[j-p];
            }
        }

        char exp[maxn];
        for(int j=0;str[j]!='\0';j++){
            exp[j]=str[j];
            if(isalpha(str[j])){
                exp[j]=c[m[str[j]]];
            }
        }

        if(solve(exp)==0){
            //真值为1，是极小项
            for(int j=0;j<cnt;j++){
                //命题变元与0对应，命题变元的否定与1对应
                if(m[vec[j]]==1){
                    if(j) str1[num1]+='V';
                    str1[num1]+='~';
                    str1[num1]+=vec[j];
                }
                else{
                    if(j) str1[num1]+='V';
                    str1[num1]+=vec[j];
                }
            }
            num1++;
        }

        else if(solve(exp)==1){
            //真值为0，是极大项
            for(int j=0;j<cnt;j++){
                //命题变元与1对应，命题变元的否定与0对应
                if(m[vec[j]]==1){
                    if(j) str2[num2]+='^';
                    str2[num2]+=vec[j];
                }
                else{
                    if(j) str2[num2]+='^';
                    str2[num2]+='~';
                    str2[num2]+=vec[j];
                }
            }
            num2++;
        }
    }

    cout<<"主合取范式:"<<endl;
    if(num1==0){
        //永真公式
        cout<<1<<endl;
    }
    else{
        for(int i=0;i<num1;i++){
            if(i) cout<<"^";
            cout<<"("<<str1[i]<<")";
        }
        cout<<endl;
    }

    cout<<"主析取范式:"<<endl;
    if(num2==0){
        //永假公式
        cout<<0<<endl;
    }
    else{
        for(int i=0;i<num2;i++){
            if(i) cout<<"V";
            cout<<"("<<str2[i]<<")";
        }
    }
    cout<<endl;
}

int main(){
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    char str[maxn];
    cin>>str;
    result(str);
    return 0;
}
