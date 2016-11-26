/*
只支持10以内的正整数的加减乘除，本来学数据结构时曾写过支持多位整数、小数、负数、乘方运算……，然而，没有了23333
离散数学上机实验要求计算命题公式的真值，和计算器差不多，就重新写了，只实现了简单功能。或许，哪天会增加功能。。。
*/
#include <iostream>
#include <stack>
#include <cstdio>
#include <cctype>
#include <cstring>
using namespace std;
const int maxn =1000+10;

int judge(char op1,char op2){
    if(op1=='#'){
        if(op2=='#') return 0;
        else return -1;
    }

    else if(op1=='+'||op1=='-'){
        if(op2=='*'||op2=='/'||op2=='('){
            return -1;
        }
        else return 1;
    }

    else if(op1=='*'||op1=='/'){
        if(op2=='('){
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

int cacl(int p,char op,int q){
    switch(op){
    case '+':
        return p+q;
    case '-':
        return p-q;
    case '*':
        return p*q;
    case '/':
        return p/q;
    }
}

int solve(char *str){
    char exp[maxn];
    int n=strlen(str);
    for(int i=0;i<n;i++){
        exp[i]=str[i];
    }
    exp[n]='#';
    exp[n+1]='\0';
    stack<int> numStack;
    stack<char> opStack;
    opStack.push('#');
    int i=0;
    while(exp[i]!='\0'){
        if(isalnum(exp[i])){
            numStack.push(exp[i]-'0');
            i++;
        }

        else{
            char op=exp[i];
            if(judge(opStack.top(),op)==1){
                int q=numStack.top();
                numStack.pop();
                int p=numStack.top();
                numStack.pop();

                int res=cacl(p,opStack.top(),q);
                numStack.push(res);
                opStack.pop();
            }

            else if(judge(opStack.top(),op)==0){
                if(op==')'){
                    opStack.pop();
                    i++;
                }
                else{
                    return numStack.top();
                }
            }

            else{
                opStack.push(op);
                i++;
            }
        }
    }
}
int main(){
    //freopen("in.txt","r",stdin);
    char str[maxn];
    cin>>str;
    cout<<solve(str)<<endl;
    return 0;
}
