#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2017-06-09 14:31:47
# @Author  : Your Name (you@example.org)
# @Link    : http://example.org
# @Version : $Id$

import os


class Grammar(object):

    def __init__(self, start=None, terminals=[], nonterminals=[], productions=[]):
        self.start = start  # 开始符号
        self.terminals = terminals  # 终结符号集合
        self.nonterminals = nonterminals  # 非终结符号集合
        self.productions = productions  # 产生式集合

    def insert(self, head, body):
        self.productions.append((head, body))

    def insert_from_arr(self, arr):
        start, ter, nonter, productions = process(arr)
        self.start = start
        self.terminals = ter
        self.nonterminals = nonter

        for production in productions:
            head, body = production[0], production[1]
            self.insert(head, body)

        # 求first集
    def first(self, s):
        if s == '~':
            return set('~')

        # 终结符号
        if s in self.terminals:
            return set(s)

        # 非终结符号
        elif s in self.nonterminals:
            # 以s为产生式头的所有产生式体
            bodys = [body for (head, body) in self.productions if head == s]
            res = set([])
            for body in bodys:
                # s->~是一个产生式
                if body == '~':
                    res.add('~')
                    continue

                cur = self.first(body[0]) - set('~')

                if '~' not in self.first(body[0]):
                    res.update(cur)
                    continue

                cnt = 0
                for x in body[1:]:
                    cur.update(self.first(x) - set('~'))
                    if '~' in self.first(x):
                        cnt += 1
                        continue
                    else:
                        break

                if cnt == len(body) - 1:
                    res.add('~')

                res.update(cur)

            return res

        # 任意符号串
        else:
            res = self.first(s[0]) - set('~')
            if '~' not in self.first(s[0]):
                return res

            cnt = 0
            for x in s[1:]:
                res.update(self.first(x) - set('~'))
                if '~' in self.first(x):
                    cnt += 1
                    continue
                else:
                    break

            if cnt == len(s) - 1:
                res.add('~')
            return res

    # 求follow集
    def follow(self):
        res = {}
        for nonter in self.nonterminals:
            res[nonter] = set()
        res[self.start].add('$')

        updated = True
        while updated:
            updated = False
            for (head, body) in self.productions:

                length = len(body)
                for i in range(length):
                    if body[i] in self.terminals:
                        continue
                    elif body[i] in self.nonterminals:
                        s = len(res[body[i]])  # 初始化follow集大小
                        has_left = True  # 是否包含产生式头的follow集

                        for j in range(i + 1, length):
                            if body[j] in self.terminals:
                                res[body[i]].add(body[j])
                                has_left = False
                                break
                            elif body[j] in self.nonterminals:
                                if '~' in self.first(body[j]):
                                    res[body[i]].update(
                                        self.first(body[j]) - set('~'))
                                    continue
                                else:
                                    res[body[i]].update(self.first(body[j]))
                                    has_left = False
                                    break
                        if has_left:
                            res[body[i]].update(res[head])
                        # 结束时follow集大小
                        e = len(res[body[i]])
                        if e > s:
                            updated = True

        return res

    def nullable(self, nonterminal):
        if nonterminal in self.terminals:
            return False

        bodies = [body for (head, body) in self.productions if head == nonterminal]
        if ['~'] in bodies:
            return True

        for body in bodies:
            flag = True
            for item in body:
                if item==nonterminal:
                    flag=False
                    break
                flag &= self.nullable(item)
            if flag == True:
                return True
        return False

    def eliminate_ε_rules(self):
        def foo(body):
            arr = [item for item in body if self.nullable(item) == True]
            k = len(arr)
            if k == 0:
                return set()
            else:
                res = set()
                for item in arr:
                    temp = body.copy()
                    del temp[temp.index(item)]
                    if len(temp)==0:
                        continue
                    res.add(tuple(temp))
                    res.update(foo(temp))
                return res

        for (head, body) in self.productions:
            temp = foo(body)
            if len(temp)==0:
                continue
            for item in temp:
                self.productions.append((head, list(item)))

        temp=[]
        for (head,body) in self.productions:
            temp.append((head,tuple(body)))
        self.productions=list(set(temp))

        temp=[]
        for (head,body) in self.productions:
            temp.append((head,list(body)))
        self.productions=temp

        for (head, body) in self.productions:
            if body == ['~']:
                index=self.productions.index((head, body))
                del self.productions[index]


    def closure(self, I):
        updated=True
        while updated:
            updated=False
            for (head, body) in I:
                if arr_to_str(body).endswith('·'):
                    continue

                B=body[body.index('·') + 1]

                if B in self.terminals:
                    continue

                bodies=[body for (head, body)
                          in self.productions if head == B]
                for b in bodies:
                    if (B, str_to_arr('·' + arr_to_str(b))) not in I:
                        updated=True
                        I.append((B, str_to_arr('·' + arr_to_str(b))))
        return I

    def goto(self, I, X):
        J=[]
        for (head, body) in I:
            if arr_to_str(body).find('·' + X) != -1:
                index=body.index('·')
                if index == len(body) - 2:
                    temp=body[0:index]
                    temp.append(X)
                    temp.append('·')
                    J.append((head, temp))
                else:
                    temp=body[0:index]
                    temp.append(X)
                    temp.append('·')
                    temp.extend(body[(index + 2):])
                    J.append((head, temp))

        return self.closure(J)

    # 计算规范LR(0)项集族
    def items(self):
        head, body=self.productions[0]
        C=[self.closure([(head, str_to_arr('·' + arr_to_str(body)))])]
        updated=True
        while updated:
            updated=False
            for I in C:
                for X in self.terminals + self.nonterminals:
                    if self.goto(I, X) != [] and self.goto(I, X) not in C:
                        C.append(self.goto(I, X))
                        updated=True

        return C

    def Constructing_an_SLR_parsing_table(self):
        action={}
        goto={}
        follow=self.follow()
        C=self.items()
        del self.nonterminals[-1]
        for I in C:
            i=C.index(I)
            for (head, body) in I:
                if arr_to_str(body).endswith('·'):
                    if head == self.start:
                        action[(i, '$')]='accept'
                    else:
                        for a in follow[head]:
                            temp=body[0:(len(body) - 1)]
                            action[(i, a)]='r' + \
                                str(self.productions.index((head, temp)))
                else:
                    index=body.index('·')
                    a=body[index + 1]
                    if a in self.nonterminals:
                        continue

                    J=self.goto(I, a)
                    if J == []:
                        continue
                    j=C.index(J)
                    action[(i, a)]='s' + str(j)


            for A in self.nonterminals:
                J=self.goto(I, A)
                if J == []:
                    continue
                j=C.index(J)
                goto[(i, A)]=j

        for I in C:
            i=C.index(I)
            for terminal in self.terminals:
                if (i,terminal) not in action.keys():
                    action[(i,terminal)]=''
            if (i,'$') not in action.keys():
                action[(i,'$')]=''

            for nonterminal in self.nonterminals:
                if (i,nonterminal) not in goto.keys():
                    goto[(i,nonterminal)]=''
        return action, goto

    def LR_parsing_program(self, action, goto, w):
        w += '$'
        a=w[0]
        stack=[0]
        step=[]
        symbols=[]

        while True:
            s=stack[-1]
            if action[(s,a)]=='':
                print('error')
                return

            temp=action[s, a]
            cur=[]
            cur.append(arr_to_str(stack))
            cur.append(arr_to_str(symbols))
            cur.append(w)

            if temp[0] == 's':
                # print(stack,symbols,w,'移入')
                cur.append('移入')
                step.append(cur)
                symbols.append(a)
                stack.append(int(temp[1]))
                w=w[1:]
                a=w[0]

            elif temp[0] == 'r':
                (head, body)=self.productions[int(temp[1])]
                # print(stack,symbols,w,'根据'+head+'->'+arr_to_str(body)+'归约')
                for i in range(len(body)):
                    del stack[-1]
                    del symbols[-1]

                cur.append('根据' + head + '->' + arr_to_str(body) + '归约')
                step.append(cur)
                t=stack[-1]
                stack.append(goto[t, head])
                symbols.append(head)


            elif temp[0] == 'a':
                # print(stack,symbols,w,'接受')
                cur.append('接受')
                step.append(cur)
                # print('accept')
                break

        return step


def adjust(I):
    res=''
    for (head, body) in I:
        res += head + '->' + arr_to_str(body) + '\n'
    res=res[:(-1)]
    return res


def arr_to_str(arr):
    s=''
    for c in arr:
        s += str(c)
    return s


def str_to_arr(body):
    res=[]
    i=0
    while i < len(body):
        if i == len(body) - 1:
            res.append(body[i])
            i += 1
        else:
            if body[i + 1] == "'":
                res.append(body[i] + body[i + 1])
                i += 2
            else:
                res.append(body[i])
                i += 1
    return res


def process(arr):
    productions=[]
    nonters=[]
    symbols=[]

    for cur in arr:
        if cur[1] == "'":
            head=cur[0:2]
            body=cur[4:]
        else:
            head=cur[0]
            body=cur[3:]

        nonters.append(head)

        bodies=body.split('|')
        for item in bodies:
            temp=[]
            i=0
            while i < len(item):
                if i == len(item) - 1:
                    temp.append(item[i])
                    i += 1
                else:
                    if item[i + 1] == "'":
                        temp.append(item[i] + item[i + 1])
                        i += 2
                    else:
                        temp.append(item[i])
                        i += 1
            productions.append((head, temp))
            symbols.extend(temp)
    start=nonters[0]

    ter=list(set(symbols) - set(nonters) - set(['|']) - set('~'))
    temp=start
    start=start + '\''
    nonters.append(start)
    productions.insert(0, (start, [temp]))
    return start, ter, nonters, productions


if __name__ == '__main__':

    # grammar = Grammar(start='E',terminals=['+', '*', '(', ')', 'i'],
    #                       nonterminals=['E', 'T', 'A', 'F', 'B'])
    # grammar.insert('E', 'TA')
    # grammar.insert('A', '+TA')
    # grammar.insert('A', '~')
    # grammar.insert('T', 'FB')
    # grammar.insert('B', '*FB')
    # grammar.insert('B', '~')
    # grammar.insert('F', '(E)')
    # grammar.insert('F', 'i')

    grammar=Grammar()
    # arr=['S->S(S)|~']
    # arr=['S->AbB|C', 'B->AA|AC', 'C->b|c', 'A->a|~']
    # arr=['S->BB','B->aB|b']
    # arr=['S->AdB','A->a|~','B->b|~|Bdb']
    # arr = ["E'->E", 'E->E+T|T', 'T->T*F|F', 'F->(E)|i']
    # arr=['M->TB','T->Ba|~','B->Db|eT|~','D->d|~']
    arr=['E->TG','G->+TG|—TG','T->FS','S->*FS|/FS','F->(E)','F->i']
    # arr = ['E->E+T|T', 'T->T*F|F', 'F->(E)|i']
    # arr=['S->Aa|b','A->Ac|Sd|~']
    # arr = ['S->ABc', 'A->a|~', 'B->b|~']
    # arr = ['S->Ab', 'A->a|B|~', 'B->b|~']
    # arr = ['S->ABBA', 'A->a|~', 'B->b|~']
    # arr = ['S->aSe|B', 'B->bBe|C', 'C->cCe|d']
    # arr=['A->iB*e','B->SB|~','S->[eC]|.i','C->eC|~']

    grammar.insert_from_arr(arr)

    # print(grammar.terminals)
    # print(grammar.nonterminals)
    # print(grammar.start)

    # for nonterminal in grammar.nonterminals:
    #     print(nonterminal,grammar.nullable(nonterminal))
    # grammar.eliminate_ε_rules()
    # for nonterminal in grammar.nonterminals:
    #     bodies=[body for (head,body) in grammar.productions if head==nonterminal]
    #     for body in bodies:
    #         print(nonterminal,body)

    # print(grammar.terminals)
    # print(grammar.nonterminals)
    # print(grammar.start)
    # print(grammar.closure([('A','·E')]))
    C = grammar.items()
    # print(grammar.start)
    # for i, item in enumerate(C):
    #     print(i, item)

    # # # LR(0)自动机
    # transitions = []

    # for item in C:
    #     for symbol in grammar.terminals + grammar.nonterminals:
    #         if grammar.goto(item, symbol) != []:
    #             transitions.append((adjust(item), adjust(
    #                 grammar.goto(item, symbol)), symbol))

    # # for item in transitions:
    # #     print(item)

    # from graphviz import Digraph

    # f = Digraph('finite_state_machine', filename='fsm.gv')
    # f.attr(rankdir='LR', size='8,5')

    # f.attr('node', shape='square')
    # for transition in transitions:
    #     f.edge(transition[0], transition[1], label=transition[2])

    # f.view()


    action, goto = grammar.Constructing_an_SLR_parsing_table()

    # print(action)
    # print(goto)
    # print(grammar.nonterminals)
    # print(grammar.start)
    # index=grammar.nonterminals.index(grammar.start)
    # del grammar.nonterminals[index]
    # print(grammar.nonterminals)
    step = grammar.LR_parsing_program(action, goto, 'i*i+i')
    # # print(step)
    # for i in range(len(step)):
    #     print(step[i])

    # LR(0)自动机
    '''
    transitions = []

    for item in C:
        for symbol in grammar.terminals + grammar.nonterminals:
            if grammar.goto(item, symbol) != []:
                transitions.append((adjust(item), adjust(
                    grammar.goto(item, symbol)), symbol))

    # for item in transitions:
    #     print(item)

    from graphviz import Digraph

    f = Digraph('finite_state_machine', filename='fsm.gv')
    f.attr(rankdir='LR', size='8,5')

    f.attr('node', shape='square')
    for transition in transitions:
        f.edge(transition[0], transition[1], label=transition[2])

    f.view()
    '''
