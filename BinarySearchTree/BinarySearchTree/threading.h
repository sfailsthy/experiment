#pragma once
#ifndef THREADING_H
#define THREADING_H

#include "BinaryNode.h"

template<typename Object>
void preThreading(BinaryNode<Object> *p, BinaryNode<Object> *&pre) {
	if (p != nullptr) {
		if (p->left == nullptr) {
			p->ltag = thread;
			p->left = pre;
		}

		if (pre != nullptr && pre->right == nullptr) {
			pre->rtag = thread;
			pre->right = p;
		}

		pre = p;
		if (p->ltag == link) {
			preThreading(p->left, pre);
		}
		if (p->rtag == link) {
			preThreading(p->right, pre);
		}
	}
}

template<typename Object>
void preThreadingOrder(BinaryNode<Object> *root) {
	BinaryNode<Object> *pre = root;
	preThreading(root, pre);

	if (root != nullptr) {
		BinaryNode<Object> *p = root;

		while (p != nullptr) {
			std::cout << p->val << " ";
			if (p->rtag == thread) {
				p = p->right;
			}
			else {
				if (p->ltag == link) {
					p = p->left;
				}
				else {
					p = p->right;
				}
			}
		}
		clear(root);
	}

}

template<typename Object>
void inThreading(BinaryNode<Object> *p,BinaryNode<Object> *&pre) {
	if (p != nullptr) {
		inThreading(p->left, pre);
		if (p->left == nullptr) {
			p->ltag = thread;
			p->left = pre;
		}

		if (pre != nullptr && pre->right == nullptr) {
			pre->rtag = thread;
			pre->right = p;
		}

		pre = p;
		inThreading(p->right, pre);
	}
}

template<typename Object>
void inThreadingOrder(BinaryNode<Object> *root) {
	BinaryNode<Object> *pre = root;
	inThreading(root, pre);

	if (root != nullptr) {
		BinaryNode<Object> *p = root;
		while (p != nullptr && p->ltag == link) {
			p = p->left;
		}

		do {
			std::cout << p->val << " ";
			if (p->rtag == thread) {
				p = p->right;
			}
			else {
				p = p->right;
				while (p != nullptr && p->ltag == link) {
					p = p->left;
				}
			}
		} while (p != nullptr);
		clear(root);
	}
}

template<typename Object>
void postThreading(BinaryNode<Object> *p, BinaryNode<Object> *&pre) {
	if (p != nullptr) {
		postThreading(p->left, pre);
		postThreading(p->right, pre);

		if (p->left == nullptr) {
			p->ltag = thread;
			p->left = pre;
		}

		if (pre != nullptr && pre->right == nullptr) {
			pre->rtag = thread;
			pre->right = p;
		}
		pre = p;
	}
}

template<typename Object>
void postThreadingOrder(BinaryNode<Object> *root) {
	BinaryNode<Object> *pre = root;
	postThreading(root, pre);

	if (root != nullptr) {
		findParents(root);
		
		BinaryNode<Object> *p = root;
		while (p != nullptr && p->ltag == link) {
			p = p->left;
		}


		while (p != nullptr) {
			std::cout << p->val << " ";
			if (p == root) {
				p = nullptr;
			}
			else if (p == p->parent->right 
				 || p->parent->rtag == thread) {
				p = p->parent;
			}
			else {
				BinaryNode<Object> *parent = p->parent;
				while (parent->rtag == link) {
					parent = parent->right;
					while (parent->ltag == link) {
						parent = parent->left;
					}
				}
				p = parent;
			}
		} 

		clear(root);
	}
}

template<typename Object>
void clear(BinaryNode<Object> *root) {
	if (root != nullptr) {
		if (root->ltag == link) {
			clear(root->left);
		}

		if (root->ltag == thread) {
			root->ltag = link;
			root->left = nullptr;
		}

		if (root->rtag == thread) {
			root->rtag = link;
			root->right = nullptr;
		}

		if (root->rtag == link) {
			clear(root->right);
		}
	}
}

template<typename Object>
void findParents(BinaryNode<Object> *root) {
	if (root != nullptr) {
		std::queue<BinaryNode<Object>*> que;
		que.push(root);

		while (!que.empty()) {
			BinaryNode<Object> *p = que.front();
			que.pop();
			if (p->ltag == link && p->left != nullptr) {
				p->left->parent = p;
				que.push(p->left);
			}
			if (p->rtag == link && p->right != nullptr) {
				p->right->parent = p;
				que.push(p->right);
			}
		}
	}
}

#endif // !THREADING_H
