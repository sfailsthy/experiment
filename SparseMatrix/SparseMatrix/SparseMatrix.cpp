#include "SparseMatrix.h"
#include <algorithm>
#include <cassert>

SparseMatrix::SparseMatrix(const std::vector<std::vector<int>> &arr) :
	row(arr.size()), col(arr[0].size()) {
	for (int i = 0; i != row; ++i) {
		for (int j = 0; j != col; ++j) {
			if (arr[i][j]) {
				data.push_back({ i,j,arr[i][j] });
			}
		}
	}
	cnt = data.size();

	std::vector<int> num(row);
	for (int i = 0; i != row; ++i) {
		num[i] = 0;
	}

	for (int index = 0; index != cnt; ++index) {
		++num[std::get<0>(data[index])];
	}

	rpos.resize(row);
	rpos[0] = 0;
	for (int i = 1; i != row; ++i) {
		rpos[i] = rpos[i - 1] + num[i - 1];
	}
}

SparseMatrix::SparseMatrix(const SparseMatrix &rhs) :
	data(rhs.data.size()), row{ rhs.row }, col{ rhs.col }, cnt{ rhs.cnt } {
	for (int i = 0; i != cnt; ++i) {
		data[i] = rhs.data[i];
	}

	for (int i = 0; i != row; ++i) {
		rpos[i] = rhs.rpos[i];
	}
}

SparseMatrix::SparseMatrix(SparseMatrix &&rhs) :
	data(std::move(rhs.data)), row{ rhs.row }, col{ rhs.col }, cnt{ rhs.cnt }, 
	rpos(std::move(rhs.rpos)) { }


SparseMatrix& SparseMatrix::operator=(const SparseMatrix &rhs) {
	SparseMatrix copy = rhs;
	std::swap(*this, copy);
	return *this;
}

SparseMatrix& SparseMatrix::operator=(SparseMatrix &&rhs) {
	std::swap(data, rhs.data);
	std::swap(row, rhs.row);
	std::swap(col, rhs.col);
	std::swap(cnt, rhs.cnt);
	std::swap(rpos, rhs.rpos);
	return *this;
}

SparseMatrix SparseMatrix::Transpose() const {
	SparseMatrix res;
	res.row = col;
	res.col = row;
	res.cnt = cnt;
	
	if (cnt) {
		for (int j = 0; j != col; ++j) {
			for (int p = 0; p != cnt; ++p) {
				if (std::get<1>(data[p]) == j) {
					res.data.push_back({ std::get<1>(data[p]),std::get<0>(data[p]),std::get<2>(data[p]) });
				}
			}
		}
	}
	return res;
}

SparseMatrix SparseMatrix::FastTranspose() const {
	SparseMatrix res;
	res.row = col;
	res.col = row;
	res.cnt = cnt;
	res.data.resize(cnt);

	std::vector<int> num(col), cpos(col);
	//num:每一列非零元素个数
	//cpot:某列第一个非零元素在data中位置

	if (cnt) {
		for (int j = 0; j != col; ++j) {
			num[j] = 0;
		}

		for (int index = 0; index != cnt; ++index) {
			++num[std::get<1>(data[index])];
		}

		cpos[0] = 0;
		for (int j = 1; j != col; ++j) {
			cpos[j] = cpos[j - 1] + num[j - 1];
		}

		for (int p = 0; p != cnt; ++p) {
			int c = std::get<1>(data[p]);
			int q = cpos[c];
			res.data[q] = { std::get<1>(data[p]) ,
				std::get<0>(data[p]) ,
				std::get<2>(data[p])
			};
			++cpos[c];
		}
	}
	return res;
}

std::ostream& operator<<(std::ostream &out, const SparseMatrix &s) {
	int index = 0;
	for (int i = 0; i != s.row; ++i) {
		for (int j = 0; j != s.col; ++j) {
			if (index < s.cnt && std::get<0>(s.data[index]) == i 
				&& std::get<1>(s.data[index]) == j) {
				out << std::get<2>(s.data[index]) << " ";
				++index;
			}
			else {
				out << 0 << " ";
			}
		}
		out << std::endl;
	}
	return out;
}

SparseMatrix operator+(const SparseMatrix &lhs,const SparseMatrix &rhs) {
	assert(lhs.row == rhs.row && lhs.col == rhs.col);
	SparseMatrix res;
	res.row = lhs.row;
	res.col = lhs.col;

	int index1 = 0, index2 = 0;
	while (index1 != lhs.data.size() && index2 != rhs.data.size()) {
		int address1 = std::get<0>(lhs.data[index1])*lhs.col + std::get<1>(lhs.data[index1]);
		int address2 = std::get<0>(rhs.data[index2])*rhs.col + std::get<1>(rhs.data[index2]);

		if (address1 < address2) {
			res.data.push_back(lhs.data[index1]);
			++index1;
		}

		else if (address1 > address2) {
			res.data.push_back(rhs.data[index2]);
			++index2;
		}

		else {
			using Triple = std::tuple<int, int, int>;
			Triple temp(lhs.data[index1]);
			std::get<2>(temp) += std::get<2>(rhs.data[index2]);
			if (std::get<2>(temp)) {
				res.data.push_back(temp);
				++index1;
				++index2;
			}
		}
	}

	while (index1 != lhs.data.size()) {
		res.data.push_back(lhs.data[index1]);
		++index1;
	}

	while (index2 != rhs.data.size()) {
		res.data.push_back(rhs.data[index2]);
		++index2;
	}
	res.cnt = res.data.size();
	return res;
}

SparseMatrix operator*(const SparseMatrix &lhs, const SparseMatrix &rhs) {
	assert(lhs.col == rhs.row);
	SparseMatrix res;
	res.row = lhs.row;
	res.col = rhs.col;
	res.rpos.resize(res.row);
	res.cnt = 0;
	if (lhs.cnt*rhs.cnt != 0) {
		for (int row1 = 0; row1 != lhs.row; ++row1) {
			std::vector<int> temp(rhs.col);
			res.rpos[row1] = res.cnt;

			int t1, t2;
			if (row1 < lhs.row - 1) {
				t1 = lhs.rpos[row1 + 1];
			}
			else {
				t1 = lhs.cnt;
			}

			for (int p = lhs.rpos[row1]; p != t1; ++p) {
				int row2 = std::get<1>(lhs.data[p]);
				if (row2 < rhs.row - 1) {
					t2 = rhs.rpos[row2 + 1];
				}
				else {
					t2 = rhs.cnt;
				}

				for (int q = rhs.rpos[row2]; q != t2; ++q) {
					int col = std::get<1>(rhs.data[q]);
					temp[col] += std::get<2>(lhs.data[p]) * std::get<2>(rhs.data[q]);
				}
			}

			for (int col = 0; col != res.col; ++col) {
				if (temp[col]) {
					res.data.push_back({ row1,col,temp[col] });
					++res.cnt;
				}
			}
		}
	}
	return res;
}
