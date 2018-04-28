#pragma once
//三元组
#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <tuple>
#include <vector>
#include <iostream>

class SparseMatrix
{
	friend std::ostream& operator<<(std::ostream&, const SparseMatrix&);
	friend SparseMatrix operator+(const SparseMatrix&, const SparseMatrix&);
	friend SparseMatrix operator*(const SparseMatrix&, const SparseMatrix&);

public:
	SparseMatrix() = default;
	SparseMatrix(const std::vector<std::vector<int>> &arr);
	SparseMatrix(const SparseMatrix &rhs);
	SparseMatrix(SparseMatrix &&rhs);
	SparseMatrix& operator=(const SparseMatrix &rhs);
	SparseMatrix& operator=(SparseMatrix &&rhs);
	~SparseMatrix() { data.clear(); }

	SparseMatrix Transpose() const;
	SparseMatrix FastTranspose() const;

private:
	using Triple = std::tuple<int, int, int>;//行下标、列下标、值
	int row, col,cnt;//行数、列数、非零个数
	std::vector<int> rpos;
	std::vector<Triple> data;
};

std::ostream& operator<<(std::ostream&, const SparseMatrix&);
SparseMatrix operator+(const SparseMatrix&, const SparseMatrix&);
SparseMatrix operator*(const SparseMatrix&, const SparseMatrix&);
#endif // !SPARSEMATRIX_H
