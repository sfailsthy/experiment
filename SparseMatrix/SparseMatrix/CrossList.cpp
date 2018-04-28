#include "CrossList.h"
#include <algorithm>
#include <cassert>

CrossList::CrossList(const std::vector<std::vector<int>> &arr) :
	row(arr.size()), col(arr[0].size()) {
	int count = 0;
	rhead.resize(row);
	chead.resize(col);

	for (int i = 0; i != row; ++i) {
		for (int j = 0; j != col; ++j) {
			if (arr[i][j]) {
				++count;
				node *p = new node{ i,j,arr[i][j],nullptr,nullptr };
				insert(p);
			}
		}
	}
	cnt = count;
}

CrossList::CrossList(const CrossList &rhs) :
	rhead(rhs.rhead.size()), chead(rhs.chead.size()), row{ rhs.row }, col{ rhs.col }, cnt{ rhs.col } {
	for (int i = 0; i != row; ++i) {
		rhead[i] = rhs.rhead[i];
	}

	for (int j = 0; j != col; ++j) {
		chead[j] = rhs.chead[j];
	}
}

CrossList::CrossList(CrossList &&rhs) :
	rhead{ std::move(rhs.rhead) }, chead{ std::move(rhs.chead) }, row{ rhs.row }, col{ rhs.col }, cnt{ rhs.cnt } {}

CrossList& CrossList::operator=(const CrossList &rhs) {
	CrossList copy = rhs;
	std::swap(*this, copy);
	return *this;
}

CrossList& CrossList::operator=(CrossList &&rhs) {
	std::swap(rhead, rhs.rhead);
	std::swap(chead, rhs.chead);
	std::swap(row, rhs.row);
	std::swap(col, rhs.col);
	std::swap(cnt, rhs.cnt);
	return *this;
}

std::vector<std::vector<int>> CrossList::getArray() const {
	std::vector<std::vector<int>> res(row);
	//行遍历
	for (int i = 0; i != row; ++i) {
		res[i].resize(col);
		node *p = rhead[i];
		while (p != nullptr) {
			res[p->i][p->j] = p->val;
			p = p->right;
		}
	}
	//列遍历
	/*for (int i = 0; i != row; ++i) {
		res[i].resize(col);
	}

	for (int j = 0; j != col; ++j) {
		node *p = chead[j];
		while (p != nullptr) {
			res[p->i][p->j] = p->val;
			p = p->down;
		}
	}*/
	return res;
}

std::ostream& operator<<(std::ostream &out, const CrossList &s) {
	auto arr = s.getArray();
	for (auto &row : arr) {
		for (auto &x : row) {
			std::cout << x << " ";
		}
		std::cout << std::endl;
	}
	return out;
}

CrossList CrossList::transpose() const {
	CrossList res;
	res.row = col;
	res.col = row;
	res.cnt = cnt;
	res.rhead.resize(col);
	res.chead.resize(row);

	for (int i = 0; i != row; ++i) {
		node *p = rhead[i];
		while (p != nullptr) {
			node *q = new node{ p->j,p->i,p->val };
			res.insert(q);
			p = p->right;
		}
	}
	return res;
}

CrossList operator+(const CrossList &lhs, const CrossList &rhs) {
	assert(lhs.row == rhs.row&&lhs.col == rhs.col);
	CrossList res;
	res.row = lhs.row;
	res.col = lhs.col;
	res.rhead.resize(res.row);
	res.chead.resize(res.col);
	int cnt = 0;

	for (int i = 0; i != lhs.row; ++i) {
		node *pa = lhs.rhead[i], *pb = rhs.rhead[i];
		while (pa != nullptr && pb != nullptr) {
			if (pa->j > pb->j) {
				res.insert(new node{ pb->i,pb->j,pb->val });
				pb = pb->right;
				++cnt;
			}
			else if (pa->j < pb->j) {
				res.insert(new node{ pa->i,pa->j,pa->val });
				pa = pa->right;
				++cnt;
			}
			else {
				int x = pa->val + pb->val;
				if (x != 0) {
					res.insert(new node{ pa->i,pa->j,x });
					++cnt;
				}
				pa = pa->right;
				pb = pb->right;
			}
		}

		while (pa != nullptr) {
			res.insert(new node{ pa->i,pa->j,pa->val });
			pa = pa->right;
			++cnt;
		}

		while (pb != nullptr) {
			res.insert(new node{ pb->i,pb->j,pb->val });
			pb = pb->right;
			++cnt;
		}
	}
	res.cnt = cnt;
	return res;
}

void CrossList::insert(node *p) {
	int i = p->i, j = p->j;

	//行插入
	if (rhead[i] == nullptr) {
		rhead[i] = p;
	}
	else {
		node *t = rhead[i];
		while (t->right != nullptr) {
			t = t->right;
		}
		t->right = p;
	}

	//列插入
	if (chead[j] == nullptr) {
		chead[j] = p;
	}
	else {
		node *t = chead[j];
		while (t->down != nullptr) {
			t = t->down;
		}
		t->down = p;
	}
}
