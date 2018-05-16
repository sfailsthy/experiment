#include <iostream>
#include <functional>
#include <string>
#include "HashTable.h"

int main() {
	HashTable<int> h(10);
	for (auto &x : { 89,18,49,58,69 }) {
		h.insert(x);
		h.print();
		std::cout<<"--------------"<<std::endl;
	}
	
	system("pause");
}