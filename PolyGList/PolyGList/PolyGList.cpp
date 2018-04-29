#include "PolyGList.h"
#include <cmath>
#include <algorithm>
#include <cassert>

PolyNode* PolyGList::createTerm(term t, int n) {
	if (n < 0) {
		return nullptr;
	}

	PolyNode *p = new PolyNode{ LIST,n - 1 };
	PolyNode *q = new PolyNode{ LIST,t.second[n - 1] };
	p->tail = q;
	int i;
	for (i = n - 2; i >= 0 && t.second[i] == 0; --i);
	if (i < 0) {
		q->tag = ATOM;
		q->coef = t.first;
	}
	else {
		q->head = createTerm(t, n - 1);
	}
	return p;
}

void PolyGList::create() {
	head = new PolyNode;
	int flag = 1;
	std::cout << "Please input the number of the variants: ";
	std::cin >> head->exp;
	while (flag) {
		flag = 0;
		std::cout << "Please input the term of the polynomial,like (coeff,exp1,exp2,...,expn) :" << std::endl;
		term t;
		std::cin >> t.first;
		if (t.first == 0) { return; }
		for (int i = 0; i != head->exp; ++i) {
			int x;
			std::cin >> x;
			t.second.push_back(x);
		}
		flag = 1;

		PolyNode *p = createTerm(t, head->exp);
		insert(head->head, p);
	}
}

void PolyGList::insert(PolyNode *&p, PolyNode *pb) {
	if (p == nullptr) {
		p = pb;
		return;
	}
	if (pb == nullptr) {
		return;
	}

	PolyNode *pa, *prear = p, *nextp;
	pa = prear->tail;
	if (pb->tag == LIST && pb->head == nullptr) {
		//������ڵ��޽�㣬ɾ��֮
		nextp = pb->tail;
		delete pb;
		pb = nextp;
	}

	while (pa != nullptr && pb != nullptr) {
		for (; pa != nullptr && pa->exp > pb->exp; prear = pa, pa = pa->tail);//����pb�Ĳ���λ��
		if (pa == nullptr || pa->exp != pb->exp) {
			//��ͬ�����pbֱ�ӽ���prear֮��
			nextp = pb->tail; //�ݴ�pb���
			prear->tail = pb; //��pb���뵽prear֮��
			pb->tail = pa;
			pb = nextp; //pbָ����һ��Ҫ����Ľ��
			prear = prear->tail;
		}
		else {
			//ָ����ͬ��ϲ�
			if (pa->tag == ATOM && pb->tag == ATOM) {
				//��Ϊԭ�ӽ��
				pa->coef += pb->coef;
				if (pa->coef == 0) {
					//ϵ��Ϊ0����ɾ��
					prear->tail = pa->tail;
					delete pa;
					pa = prear->tail;
				}
				nextp = pb->tail;
				delete pb;
				pb = nextp;//��һ��Ҫ����Ľ��
			}
			else {
				if ((pa->tag == ATOM && pb->tag == LIST) || (pa->tag == LIST && pb->tag == ATOM)) {
					//һ����ԭ�ӽ�㣬һ���Ǳ���
					if (pa->tag == LIST) {
						//��������ԭ�ӽ��
						nextp = pb->tail;
						pb->exp = 0;
						insert(pa->head, pb);
					}
					else {
						//�������Ǳ���
						nextp = pb->tail;
						prear->tail = pb;//��pb��������
						pb->tail = pa->tail;
						pa->exp = 0;
						insert(pb->head, pa);
					}

					if (prear->tail->head == nullptr) {
						//���ڵ�ǰλ�ò�������Ϊ�գ���ɾ��֮
						pa = prear->tail->head;
						delete prear->tail;
						prear->tail = pa;
					}
					pa = prear->tail;
					pb = nextp;
				}

				else {
					//���Ǳ���
					nextp = pb->tail;
					insert(pa->head, pb->head);
					if (pa->head == nullptr) {
						//����ϲ�ͬ��������paΪ�գ���ɾ��֮
						prear->tail = pa->tail;
						delete pa;
						pa = prear->tail;
					}
					delete pb;
					pb = nextp;
				}
			}
		}
	}

	if (p->tag == LIST && p->head == nullptr&&p->tail == nullptr) {
		//��pb�еĽ��ȫ������p����p�գ���ɾ��֮
		PolyNode *tmp = p;
		p = nullptr;
		delete tmp;
	}

	if (pb != nullptr) {
		insert(head->head, pb);
	}
}

void PolyGList::print(PolyNode* p) const {
	if (p == nullptr) {
		std::cout << "����ʽ������" << std::endl;
		return;
	}
	
	char ch;  //����������ȷ���������δ֪����Ӧ����x,y����z
	int flag = 0;   //�����ж��Ƿ�Ҫ���+��
	PolyNode *pa;   //����
	while (p) {
		if (p->head == nullptr && p->tag == LIST) {
			//�Ǳ�ͷ�ڵ�
			ch = 'x' + p->exp;
			p = p->tail;
		}
		else {
			bool b = false;
			if (p->tail != nullptr) {
				std::cout << "(";
				b = true;
			}

			if (p->tag == LIST) {
				//�Ǳ��Ǳ�ͷ�����²����²��Ǳ�ͷ��һ����ͬ��
				while (p) {
					if (flag) {
						pa = p->head;
						while (pa != nullptr) {
							pa = pa->tail;
							if (pa != nullptr) {
								if (pa->tag == LIST) pa = pa->head;
								else if (pa->coef > 0) {
									std::cout << " + ";
									break;
								}
							}
						}
					}

					if (p->head != nullptr) {
						//���²㣬������²���ͬ��
						print(p->head);
					}
					if (p->exp == 0) std::cout << "";
					else {
						if (p->exp == 1) std::cout << ch;
						else std::cout << ch << "^" << p->exp;
					}
					flag = 1;
					p = p->tail;
					if (p == nullptr) std::cout << ")";
					if (p != nullptr && p->tag == ATOM) break;
				}
			}
			else {
				//ԭ�ӽ��
				while (p) {
					if (flag && p->coef > 0) {
						std::cout << " + ";
					}

					if (p->exp == 0) std::cout << p->coef;
					else {
						if (p->coef == 1 || p->coef == -1) {
							if (p->coef == -1) { std::cout << "-"; }
							if (p->exp == 1) std::cout << ch;
							else std::cout << ch << "^" << p->exp;
						}
						else {
							std::cout << p->coef;
							if (p->exp == 1) std::cout << ch;
							else std::cout << ch << "^" << p->exp;
						}
					}

					flag = 1;
					p = p->tail;
					if (p == nullptr) std::cout << ")";
					if (p != nullptr && p->tag == LIST) break;
				}
			}
		}
	}
}

int PolyGList::value(PolyNode *p, std::vector<int> &vec) const {
	if (p == nullptr) {
		return 0;
	}

	int i, res = 0;
	while (p != nullptr) {
		if (p->tag == LIST) {
			if (p->head == nullptr) {
				//��ͷ
				i = p->exp;
				p = p->tail;
			}
			else {
				//����Ǳ�ͷ
				while (p != nullptr && p->tag == LIST) {
					res += pow(vec[i], p->exp)*value(p->head, vec);
					p = p->tail;
				}

				if (p != nullptr && p->tag == ATOM) p = p;
				else return res;
			}
		}
		else {
			//ԭ�ӽ��
			while (p != nullptr && p->tag == ATOM) {
				res += p->coef*pow(vec[i], p->exp);
				p = p->tail;
			}
			if (p != nullptr && p->tag == LIST) p = p;
			else return res;
		}
	}
}

void PolyGList::negate(PolyNode *p) {
	while (p != nullptr) {
		if (p->tag == ATOM) {
			//ԭ�ӽ��
			p->coef *= -1;
			p = p->tail;
		}
		else {
			if (p->head != nullptr) negate(p->head);
			p = p->tail;
		}
	}
}

PolyGList PolyGList::operator+=(const PolyGList &rhs) {
	PolyNode *h, *tmp;
	int maxvar = std::max(head->exp, rhs.head->exp);
	tmp = head->exp > rhs.head->exp ? rhs.head : head;//tmpָ������Ԫ����ʽͷ���

	while (tmp->exp < maxvar) {
		tmp->exp++;
		int varname = tmp->head->exp + 1;
		PolyNode *p, *ph;
		ph = new PolyNode{ LIST,varname };
		p = new PolyNode{ LIST,0 };
		ph->tail = p;
		p->head = tmp->head;
		tmp->head = ph;
	}

	h = rhs.head->head;
	insert(head->head, h);
	return *this;
}

PolyGList operator+(const PolyGList &lhs, const PolyGList &rhs) {
	PolyGList res = lhs;
	res += rhs;
	return res;
}

PolyGList operator-(const PolyGList &lhs, const PolyGList &rhs) {
	PolyGList tmp = rhs;
	tmp.negate();
	PolyGList res = lhs;
	res += tmp;
	return res;
}