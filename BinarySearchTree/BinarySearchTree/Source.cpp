#include "BinarySearchTree.h"
#include <iostream>

int main()
{
	/*BinarySearchTree<int> tree;
	std::vector<int> vec{ 3,2,1,4,5,6,7,16,15,14,13,12,11,10,8,9 };
	for (int i : vec) {
		tree.insert(i);
		printTree(tree.getRoot());
		std::cout << std::endl;
	}*/

	std::vector<int> vec{ 5,29,7,8,14,23,3,11 };
	auto res = huffmanEncode(vec);
	BinaryNode<int> *root = res.first;
	auto codes = res.second;
	for (const auto &item : codes) {
		std::cout << item.first << " : " << item.second << std::endl;
	}
	printTree(root);

	std::string str = "000001111011110111111011001001";
	std::cout << str << std::endl;
	huffmanDecode(str, root);
	system("pause");
}