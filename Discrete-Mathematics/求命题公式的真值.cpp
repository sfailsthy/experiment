#include <iostream>
#include <stack>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <map>
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

int main(){
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    char str[maxn];
    while(cin>>str){
        cout<<solve(str)<<endl;
    }
    return 0;
}
