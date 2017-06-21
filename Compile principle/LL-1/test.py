#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2017-06-01 01:00:41
# @Author  : sfailsthy (z1823339284@gmail.com)
# @Version : 实现first集合√，实现follow集合√，构造预测分析表√，预测语法分析√
# 用~表示Epsilon


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

    # 从非终结符号A能到达的所有的非终结符号
    def touch(self, A):
        bodies = [body for (head, body) in self.productions if head == A]
        queue = list(set(
            [symbol for body in bodies for symbol in body if symbol in self.nonterminals]))
        res = queue.copy()

        while len(queue) != 0:
            x = queue[0]
            del queue[0]
            bodies = [body for (head, body) in self.productions if head == x]
            for body in bodies:
                for symbol in body:
                    if symbol in self.nonterminals and symbol not in res:
                        res.append(symbol)
                        queue.append(symbol)

        return set(res)

    # 判断是否含有左递归
    def have_left_recursion(self):
        # 若A=>BC,且B是非终结符号，则B在foo(A)中
        def foo(A):
            queue = list(set([body[0] for (
                head, body) in self.productions if head == A and body[0] in self.nonterminals]))
            res = queue.copy()

            while len(queue) != 0:
                x = queue[0]
                del queue[0]
                temp = list(set([body[0] for (
                    head, body) in self.productions if head == x and body[0] in self.nonterminals]))
                for item in temp:
                    if item not in res:
                        res.append(item)
                        queue.append(item)
            return res

        for nonter in self.nonterminals:
            if nonter in foo(nonter):
                return True
        return False

    # 消除左递归
    def eliminating_left_recursion(self):
        Alphabet = [chr(i) for i in range(65, 91)
                    if chr(i) not in self.nonterminals]
        for i in range(len(self.nonterminals)):
            Ai = self.nonterminals[i]

            for j in range(i):
                Aj = self.nonterminals[j]
                Ai_Aj_productions = [(head, body) for (head, body) in self.productions if head == Ai and body.startswith(
                    Aj) and body != Aj]  # 形为Ai->Aj alpha的产生式
                # Ai_productions=[(head,body) for (head,body) in
                # self.productions if head==Ai] # 以Ai为产生式头的产生式
                # 以Aj为产生式头的所有产生式的产生式体
                Aj_bodies = [body for (head, body)
                             in self.productions if head == Aj]

                for production in Ai_Aj_productions:
                    self.productions.remove(production)
                    body = production[1][1:]

                    for item in Aj_bodies:
                        self.productions.append((Ai, item + body))

            # 消除Ai产生式之间的立即左递归
            productions = [(head, body) for (head, body)
                           in self.productions if head == Ai]  # 以Ai为产生式头的所有产生式
            productions1 = [(head, body) for (
                head, body) in productions if body.startswith(Ai)]  # 形如Ai->Ai alpha的产生式
            if len(productions1) == 0:
                # Ai产生式之间无左递归
                continue
            productions2 = [(head, body) for (head, body) in productions if (
                head, body) not in productions1]  # 形如Ai->alpha的产生式

            for production in productions:
                self.productions.remove(production)

            bodies1 = [body[1:]
                       for (head, body) in productions1]  # alpha1,alpha2,alpha3……
            # beta1,beta2,beta3……
            bodies2 = [body for (head, body) in productions2]
            for body in bodies2:
                self.productions.append((Ai, body + Alphabet[0]))

            for body in bodies1:
                self.productions.append((Alphabet[0], body + Alphabet[0]))
            self.productions.append((Alphabet[0], '~'))
            self.nonterminals.append(Alphabet[0])
            del Alphabet[0]

        # 去除那些从开始符号出发永远无法到达的非终结符的产生规则
        res = self.touch(self.start)
        if len(res) == len(self.nonterminals):
            return
        productions = [(head, body)
                       for (head, body) in self.productions if head in res]
        nonters = [head for (head, body) in productions]
        ters = [ter for (head, body) in productions for ter in body]
        ters = list(set(ters) - set('~'))
        nonters = list(set(nonters))
        self.productions = productions
        self.nonterminals = nonters
        self.terminals = ters

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

    # 构造预测分析表
    def consuct_predictive_parsing_table(self, follow):
        table = {}
        for nonter in self.nonterminals:
            for ter in self.terminals:
                table[nonter, ter] = []
            table[nonter, '$'] = []

        for (head, body) in self.productions:
            for item in self.first(body):
                if item in self.terminals:
                    table[head, item].append(head + '->' + body)

            if '~' in self.first(body):
                for item in follow[head]:
                    if item in self.terminals:
                        table[head, item].append(head + '->' + body)

            if '~' in self.first(body) and '$' in follow[head]:
                table[head, '$'].append(head + '->' + body)

        for nonter in self.nonterminals:
            for ter in self.terminals:
                if len(table[nonter, ter]) == 0:
                    table[nonter, ter] = None
            if len(table[nonter, '$']) == 0:
                table[nonter, '$'] = None

        return table

    # 判断是否是LL(1)文法
    def is_LL1(self):
        follow = self.follow()
        for nonter in self.nonterminals:
            bodies = [body for (head, body)
                      in self.productions if head == nonter]
            if len(bodies) < 2:
                continue

            for i in range(len(bodies)):
                for j in range(i):
                    if len(self.first(bodies[i]) & self.first(bodies[j])) != 0:
                        return False

                    if '~' in self.first(bodies[j]):
                        if len(self.first(bodies[i]) & follow[nonter]) != 0:
                            return False

                    if '~' in self.first(bodies[i]):
                        if len(self.first(bodies[j]) & follow[nonter]) != 0:
                            return False
        return True

    # 预测分析算法
    def predictive_parsing(self, table, s):
        s += '$'
        stack = ['$', self.start]

        ip = s[0]  # 字符串s的第一个符号
        X = stack[-1]  # 栈顶符号

        step = [['', self.start + '$', s, '初始化']]  # 保存预测分析时执行的步骤
        match = ''  # 已匹配

        while X != '$':  # 栈非空
            action = None  # 执行的动作
            if X == ip:
                match += ip
                action = '匹配 ' + ip
                del stack[-1]
                # ip=s[s.index(ip)+1]
                s = s[1:]
                ip = s[0]

            elif X in self.terminals:
                print('Error')
                return

            elif table[X, ip] == None:
                print('Error')
                return

            else:
                del stack[-1]
                arr = table[X, ip]
                production = arr[0]
                body = production[3:]

                if body != '~':
                    for i in range(len(body)):
                        stack.append(body[-i - 1])

                action = '输出 ' + production
            cur = ['' if match == '' else match, arr_to_str(
                list(reversed(stack))), s, action]
            step.append(cur)

            X = stack[-1]

        return step


def arr_to_str(arr):
    s = ''
    for c in arr:
        s += c
    return s


def process(arr):
    res = []
    nonter = []
    symbols = []
    start = arr[0][0]

    for cur in arr:
        head = cur[0]
        body = cur[3:]
        nonter.append(head)
        symbols.extend([item for item in cur[0] + cur[3:]])

        if '|' not in body:
            res.append((head, body))
        else:
            body = body.split('|')
            for item in body:
                res.append((head, item))

    ter = list(set(symbols) - set(nonter) - set(['|']) - set('~'))
    return start, ter, nonter, res


if __name__ == '__main__':

    # grammar = Grammar(start='E',terminals=['+', '*', '(', ')', 'i'],
    #                   nonterminals=['E', 'T', 'A', 'F', 'B'])
    # grammar.insert('E', 'TA')
    # grammar.insert('A', '+TA')
    # grammar.insert('A', '~')
    # grammar.insert('T', 'FB')
    # grammar.insert('B', '*FB')
    # grammar.insert('B', '~')
    # grammar.insert('F', '(E)')
    # grammar.insert('F', 'i')

    grammar = Grammar()
    # arr=['S->Qc|c','Q->Rb|b','R->Sa|a']
    # arr=['M->TB','T->Ba|~','B->Db|eT|~','D->d|~']
    # arr=['E->E+T|T','T->T*F|F','F->(E)|i']
    # arr=['S->Aa|b','A->Ac|Sd|~']
    # arr = ['S->ABc', 'A->a|~', 'B->b|~']
    # arr = ['S->Ab', 'A->a|B|~', 'B->b|~']
    # arr = ['S->ABBA', 'A->a|~', 'B->b|~']
    # arr = ['S->aSe|B', 'B->bBe|C', 'C->cCe|d']
    # arr=['A->iB*e','B->SB|~','S->[eC]|.i','C->eC|~']

    grammar.insert_from_arr(arr)
    # print(grammar.is_LL1())
    # for (key, value) in grammar.productions:
    #     print(key, value)

    # print(grammar.have_left_recursion())
    # for (key, value) in grammar.productions:
    #     print(key, value)

    print(grammar.terminals)
    print(grammar.nonterminals)
    print(grammar.start)

    print(grammar.have_left_recursion())
    # grammar.eliminating_left_recursion()

    # print('---------------------')
    # print(grammar.terminals)
    # print(grammar.nonterminals)
    # for (key, value) in grammar.productions:
    #     print(key+'->'+value)

    # '''
    # print(grammar.is_LL1())
    # print('-----first------')
    # for symbol in grammar.nonterminals:
    #     print(symbol, grammar.first(symbol))
    # follow = grammar.follow()
    # print('-----follow------')
    # for nonter in grammar.nonterminals:
    #     print(nonter, follow[nonter])

    # print('-------------')
    # table=grammar.consuct_predictive_parsing_table(follow)
    # print('-'*50)
    # for nonter in grammar.nonterminals:
    #     for ter in grammar.terminals:
    #         print(nonter,ter,table[nonter,ter])
    #     print(nonter,'$',table[nonter,'$'])

    # step=grammar.predictive_parsing(table,'i+i*i')

    # print(step)
    # for i in range(len(step)):
    # print(step[i])
    # '''
# E->E+T|T
# T->T*F|F
# F->(E)|i
