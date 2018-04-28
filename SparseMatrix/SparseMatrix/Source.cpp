#include <tuple>
#include <iostream>
#include "SparseMatrix.h"
#include "CrossList.h"

int main() {
	/*SparseMatrix sm({
		{0,12,9,0,0,0,0},
		{0,0,0,0,0,0,0},
		{-3,0,0,0,0,14,0},
		{0,0,24,0,0,0,0},
		{0,18,0,0,0,0,0},
		{15,0,0,-7,0,0,0}
		});
	std::cout << sm << std::endl;

	SparseMatrix t = sm.FastTranspose();
	std::cout << t << std::endl;
	SparseMatrix s1({
		{ 1,0,3,0,5 },
		{ 0,0,0,0,0 },
		{ 0,0,0,0,0 },
		{ 1,0,3,0,5 },
		{ 0,0,0,0,0 },
		{ 0,0,0,0,0 } });
	SparseMatrix s2({
		{ 1,0,3,0,5 },
		{ 0,0,0,0,0 },
		{ 0,0,2,4,0 },
		{ 1,0,3,0,5 },
		{ 0,0,0,1,0 },
		{ 0,0,0,0,1 } });
	std::cout << s1 << std::endl;
	std::cout << s1.Transpose() << std::endl;
	std::cout << s1 + s2 << std::endl;
	SparseMatrix m({
		{3,0,0,5},
		{0,-1,0,0},
		{2,0,0,0}
		});
	SparseMatrix n({
		{0,2},
		{1,0},
		{-2,4},
		{0,0}
		});*/

	CrossList c1({
		{ 1,0,3,0,5 },
		{ 0,0,0,0,0 },
		{ 0,0,0,0,0 },
		{ 1,0,3,0,5 },
		{ 0,0,0,0,0 },
		{ 0,0,0,0,0 } });
	CrossList c2({
		{ 1,0,3,0,-5 },
		{ 0,0,0,0,0 },
		{ 0,0,2,4,0 },
		{ 1,0,3,0,5 },
		{ 0,0,0,1,0 },
		{ 0,0,0,0,1 } });
	std::cout << c1 + c2 << std::endl;
	system("pause");
}