//
// Created by sfailsthy on 2018/4/4.
//

#ifndef EXPR_TREE_BINARYNODE_H
#define EXPR_TREE_BINARYNODE_H

#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <utility>
#include <iomanip>
#include <algorithm>
#include <cmath>


struct BinaryNode
{
    char val;
    int depth = 0;
    BinaryNode *left;
    BinaryNode *right;
    BinaryNode(char x = ' ', BinaryNode *l = nullptr, BinaryNode *r = nullptr) :
            val{ x }, left{ l }, right{ r } {}
};

BinaryNode* creat(int depth=0)
{
    char x;
    std::cin >> x;
    if (x == '#') {
        return nullptr;
    }
    else {
        BinaryNode *root = new BinaryNode;
        root->val = x;
        root->depth = depth;
        root->left = creat(depth+1);
        root->right = creat(depth+1);
        return root;
    }
}

void preorder(BinaryNode *root,int indent=0) {
    if (root) {
        std::cout << root->val << " ";
        preorder(root->left);
        preorder(root->right);
    }
}

void inorder(BinaryNode *root) {
    if (root) {
        inorder(root->left);
        std::cout << root->val << " ";
        inorder(root->right);
    }
}

void postorder(BinaryNode *root) {
    if (root) {
        postorder(root->left);
        postorder(root->right);
        std::cout << root->val << " ";
    }
}

void bfs(BinaryNode *root) {
    if (root) {
        std::map<int, std::vector<int>> m;
        std::queue<BinaryNode*> que;
        que.push(root);

        while (!que.empty()) {
            BinaryNode *p = que.front();
            std::cout << p->val << " ";
            if (m.find(p->depth)!=m.end()) {
                m[p->depth].push_back(p->val);
            }
            else {
                m[p->depth] = { p->val };
            }
            que.pop();
            if (p->left) {
                que.push(p->left);
            }
            if (p->right) {
                que.push(p->right);
            }
        }
        std::cout << std::endl;
        std::cout << "---------------------" << std::endl;
        for (auto it = m.begin(); it != m.end(); ++it) {
            std::cout << it->first << " : ";
            for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1) {
                std::cout << *it1 << " ";
            }
            std::cout << std::endl;
        }
    }
}

int maxLevel(BinaryNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return std::max(maxLevel(node->left), maxLevel(node->right)) + 1;
}

bool all_is_null(std::vector<BinaryNode*> nodes) {
    for (BinaryNode* node : nodes) {
        if (node != nullptr) {
            return false;
        }
    }
    return true;
}

void print_white_spaces(int count) {
    for (int i = 0; i != count; ++i) {
        std::cout << " ";
    }
}

void printNodes(std::vector<BinaryNode*> nodes, int level, int max_level) {
    /*if (nodes.empty() || all_is_null(nodes)) {
        return;
    }*/
    int floor = max_level - level;
    int endgeLines = static_cast<int>(pow(2, std::max(floor - 1, 0)));
    int firstSpaces = static_cast<int>(pow(2, floor) - 1);
    int betweenSpaces = static_cast<int>(pow(2, floor + 1) - 1);
    print_white_spaces(firstSpaces);

    std::vector<BinaryNode*> newNodes;
    for (BinaryNode* node : nodes) {
        if (node != nullptr) {
            std::cout << node->val ;
            newNodes.push_back(node->left);
            newNodes.push_back(node->right);
        }
        else {
            newNodes.push_back(nullptr);
            newNodes.push_back(nullptr);
            std::cout << " ";
        }

        print_white_spaces(betweenSpaces);
    }
    std::cout << std::endl;

    if (all_is_null(newNodes)) {
        return;
    }

    for (int i = 1; i <= endgeLines; i++) {
        for (int j = 0; j < nodes.size(); j++) {
            print_white_spaces(firstSpaces - i);
            if (nodes[j] == nullptr) {
                print_white_spaces(endgeLines * 2 + i + 1);
                continue;
            }

            if ((nodes[j]->left) != nullptr) {
                std::cout << "/";
            }
            else {
                print_white_spaces(1);
            }
            print_white_spaces(2 * i - 1);

            if ((nodes[j]->right) != nullptr) {
                std::cout << "\\";
            }
            else {
                print_white_spaces(1);
            }
            print_white_spaces(endgeLines * 2 - i);
        }
        std::cout << std::endl;
    }

    printNodes(newNodes, level + 1, max_level);

}


void printTree(BinaryNode *root) {
    int max_level = maxLevel(root);
    printNodes(std::vector<BinaryNode*>{root}, 1, max_level);
}

#endif //EXPR_TREE_BINARYNODE_H
