#include "BinarySearchTree.h"
#include <iostream>

int main()
{
	BinarySearchTree<int> tree;
	std::vector<int> vec{ 3,2,1,4,5,6,7,16,15,14,13,12,11,10,8,9 };
	for (int i : vec) {
		tree.insert(i);
		printTree(tree.getRoot());
		std::cout << std::endl;
	}
	
	system("pause");
}