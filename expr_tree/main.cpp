#include <iostream>
#include <stack>
#include "BinaryNode.h"

void expr_tree(std::string &str) {
    std::stack<BinaryNode*> s;
    for (char c : str) {
        if (c != '+' && c != '-' && c != '*' && c != '/') {
            /*BinaryNode *p = new BinaryNode;
            p->val = c;
            p->left = nullptr;
            p->right = nullptr;
            s.push(p);*/
            s.push(new BinaryNode{ c });
        }
        else {
            if (s.size() < 2) {
                std::cerr << "The size of the stack is less than 2." << std::endl;
                return;
            }
            else {
                auto t1 = s.top();
                s.pop();
                auto t2 = s.top();
                s.pop();
                s.push(new BinaryNode(c, t2, t1));
            }
        }
    }
    if (s.empty() || s.size() > 1) {
        std::cerr << "the size of the stack is not 1." << std::endl;
    }
    else {
        printTree (s.top ());
        std::cout << std::endl;
    }
}

int main()
{
    std::string s = "ab+cde+**";
    expr_tree(s);
    return 0;
}