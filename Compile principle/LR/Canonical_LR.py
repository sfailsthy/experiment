#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2017-06-10 09:20:07
# @Author  : Your Name (you@example.org)
# @Link    : http://example.org
# @Version : $Id$

import os

from Grammar import *


class Canonical_LR(object):

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
        if s in self.terminals or s=='$':
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

                cur = self.first(body) - set('~')

                if '~' not in self.first(body):
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

    def closure(self, I):
        updated = True
        while updated:
            updated = False
            for (head, body,external) in I:
                if arr_to_str(body).endswith('·'):
                    continue

                index = body.index('·')
                B = body[index + 1]

                if B in self.terminals:
                    continue

                bodies = [body for (head, body) in self.productions if head == B]
                for bd in bodies:
                    temp = None
                    if index == len(body) - 2:
                        temp = external
                    else:
                        temp = str_to_arr(arr_to_str(body[index + 2]) + external)

                    for b in self.first(temp):
                        if (B, str_to_arr('·' + arr_to_str(bd)), b) not in I:
                            I.append((B, str_to_arr('·' + arr_to_str(bd)), b))
                            updated = True
        return I


    def goto(self,I,X):
        J=[]
        for (head, body,external) in I:
            if arr_to_str(body).find('·' + X) != -1:
                index=body.index('·')
                if index == len(body) - 2:
                    temp=body[0:index]
                    temp.append(X)
                    temp.append('·')
                    J.append((head, temp,external))
                else:
                    temp=body[0:index]
                    temp.append(X)
                    temp.append('·')
                    temp.extend(body[(index + 2):])
                    J.append((head, temp,external))

        return self.closure(J)

    def items(self):
        head,body=self.productions[0]
        C=[self.closure([(head,str_to_arr('·' + arr_to_str(body)),'$')])]

        updated=True
        while updated:
            updated=False
            for I in C:
                for X in self.terminals+self.nonterminals:
                    if self.goto(I, X) != [] and self.goto(I, X) not in C:
                        C.append(self.goto(I, X))
                        updated=True
        return C

    def Canonical_LR_Parsing_Tables(self):
        action={}
        goto={}
        C=self.items()
        del self.nonterminals[-1]

        for I in C:
            i=C.index(I)
            for (head, body,external) in I:
                if arr_to_str(body).endswith('·'):
                    if head == self.start:
                        action[(i, '$')]='accept'
                    else:
                        temp=body[0:(len(body) - 1)]
                        action[(i,external)]='r'+str(self.productions.index((head,temp)))

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
                # print(stack,symbols,w,'根据'+head+'->'+body+'归约')
                for i in range(len(body)):
                    del stack[-1]
                    del symbols[-1]

                cur.append('根据' + head + '->' + arr_to_str(body) + '归约')
                step.append(cur)
                t=stack[-1]
                stack.append(goto[t, head])
                symbols.append(head)

                # print(head+'->'+body)

            elif temp[0] == 'a':
                # print(stack,symbols,w,'接受')
                cur.append('接受')
                step.append(cur)
                # print('accept')
                break

        return step

    def LR1_parsing_program(self, action, goto, w):
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
                # print(stack,symbols,w,'根据'+head+'->'+body+'归约')
                for i in range(len(body)):
                    del stack[-1]
                    del symbols[-1]

                cur.append('根据' + head + '->' + arr_to_str(body) + '归约')
                step.append(cur)
                t=stack[-1]
                stack.append(goto[t, head])
                symbols.append(head)

                # print(head+'->'+body)

            elif temp[0] == 'a':
                # print(stack,symbols,w,'接受')
                cur.append('接受')
                step.append(cur)
                # print('accept')
                break

        return step

def adjust(I):
    res=''
    for (head,body,external) in I:
        res+=head+'->'+arr_to_str(body)+','+external+'\n'
    res=res[:(-1)]
    return res


if __name__ == '__main__':
    arr = ['S->BB', 'B->aB|b']
    grammar = Canonical_LR()
    grammar.insert_from_arr(arr)
    # print(grammar.terminals)
    # print(grammar.nonterminals)
    # print(grammar.start)

    # for symbol in grammar.terminals+grammar.nonterminals:
        # print(symbol,grammar.first(symbol))
    # print(grammar.first('$'))
    # I=grammar.closure([("S'",['·','S'],'$')])
    # for item in I:
    #     print(item)
    # print('-'*30)
    # J=grammar.goto(I,'S')
    # print(J)

    C=grammar.items()
    for I in C:
        print(I)


    action,goto=grammar.Canonical_LR_Parsing_Tables()
    print(action)
    print(goto)

    step=grammar.LR1_parsing_program(action,goto,'aabab')
    for item in step:
        print(item)
    # LR(0)自动机
    transitions = []

    for item in C:
        for symbol in grammar.terminals + grammar.nonterminals:
            if grammar.goto(item, symbol) != []:
                transitions.append((adjust(item), adjust(grammar.goto(item, symbol)), symbol))

    # for item in transitions:
    #     print(item)

    from graphviz import Digraph

    f = Digraph('finite_state_machine', filename='fsm.gv')
    f.attr(rankdir='LR', size='8,5')

    f.attr('node', shape='square')
    for transition in transitions:
        f.edge(transition[0], transition[1], label=transition[2])

    f.view()