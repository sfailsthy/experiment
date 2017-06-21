# -*- coding: utf-8 -*-
# 修改版


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
        for i in range(len(self.nonterminals)):
            Ai = self.nonterminals[i]

            for j in range(i):
                Aj = self.nonterminals[j]
                Ai_Aj_productions = [(head, body) for (head, body) in self.productions if head == Ai and arr_to_str(body).startswith(Aj) and len(body)!=1]  # 形为Ai->Aj alpha的产生式
                # Ai_productions=[(head,body) for (head,body) in self.productions if head==Ai] # 以Ai为产生式头的产生式
                # 以Aj为产生式头的所有产生式的产生式体
                Aj_bodies = [body for (head, body) in self.productions if head == Aj]

                for production in Ai_Aj_productions:
                    self.productions.remove(production)
                    body = production[1][1:]

                    for item in Aj_bodies:
                        self.productions.append((Ai, str_to_arr(arr_to_str(item) + arr_to_str(body))))

            # 消除Ai产生式之间的立即左递归
            productions = [(head, body) for (head, body) in self.productions if head == Ai]  # 以Ai为产生式头的所有产生式
            productions1 = [(head, body) for (head, body) in productions if arr_to_str(body).startswith(Ai)]  # 形如Ai->Ai alpha的产生式
            if len(productions1) == 0:
                # Ai产生式之间无左递归
                continue
            productions2 = [(head, body) for (head, body) in productions if (head, body) not in productions1]  # 形如Ai->alpha的产生式

            for production in productions:
                self.productions.remove(production)

            bodies1 = [body[1:] for (head, body) in productions1]  # alpha1,alpha2,alpha3……
            # beta1,beta2,beta3……
            bodies2 = [body for (head, body) in productions2]
            for body in bodies2:
                body.append(Ai+'\'')
                self.productions.append((Ai, body))

            for body in bodies1:
                body.append(Ai+'\'')
                self.productions.append((Ai+'\'', body))
            self.productions.append((Ai+'\'', '~'))
            self.nonterminals.append(Ai+'\'')

        # 从开始符号能到达的所有的非终结符号
        def touch():
            bodies = [body for (head, body) in self.productions if head == self.start]
            queue = list(set([symbol for body in bodies for symbol in body if symbol in self.nonterminals]))
            queue.append(self.start)
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

        # 去除那些从开始符号出发永远无法到达的非终结符的产生规则
        # res = touch()
        # if len(res) == len(self.nonterminals):
        #     return
        # productions = [(head, body) for (head, body) in self.productions if head in res]
        # nonterminals= [head for (head, body) in productions]
        # terminals = [terminal for (head, body) in productions for terminal in body if terminal in self.terminals]
        # terminals = list(set(terminals))
        # nonterminals = list(set(nonterminals))
        # self.productions = productions
        # self.nonterminals = nonterminals
        # self.terminals = terminals


    # 判断是否有左公因子
    def have_left_factor(self):
        for nonterminal in self.nonterminals:
            bodies=[arr_to_str(body) for (head,body) in self.productions if head==nonterminal]
            prefix=common_prefix(bodies)
            if len(prefix)==0:
                continue
            return True
        return False

    # 提取左公因子
    def left_factoring(self):
        updated=True
        while updated:
            updated=False
            for nonterminal in self.nonterminals:
                bodies=[arr_to_str(body) for (head,body) in self.productions if head==nonterminal]
                prefix=common_prefix(bodies)

                if len(prefix)==0:
                    continue
                updated=True

                bodies1=[body for body in bodies if body.startswith(prefix)]

                for body in bodies1:
                    production=(nonterminal,str_to_arr(body))
                    self.productions.remove(production)

                self.productions.append((nonterminal,str_to_arr(prefix+nonterminal+'\'')))
                self.nonterminals.append(nonterminal+'\'')

                length=len(prefix)
                bodies1=[body[length:] for body in bodies1]
                for body in bodies1:
                    if len(body)==0:
                        body='~'
                    self.productions.append((nonterminal+'\'',str_to_arr(body)))

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

    def is_LL1(self):
        follow=self.follow()
        for nonterminal in self.nonterminals:
            bodies=[arr_to_str(body) for (head,body) in self.productions if head==nonterminal]
            if len(bodies)<2:
                continue

            for i in range(len(bodies)):
                for j in range(i):
                    if len(self.first(bodies[i])&self.first(bodies[j]))!=0:
                        return False

                    if '~' in self.first(bodies[j]):
                        if len(self.first(bodies[i])&follow[nonterminal])!=0:
                            return False

                    if '~' in self.first(bodies[i]):
                        if len(self.first(bodies[j])&follow[nonterminal])!=0:
                            return False
        return True


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
                    table[head, item].append(head + '->' + arr_to_str(body))

            if '~' in self.first(body):
                for item in follow[head]:
                    if item in self.terminals:
                        table[head, item].append(
                            head + '->' + arr_to_str(body))

            if '~' in self.first(body) and '$' in follow[head]:
                table[head, '$'].append(head + '->' + arr_to_str(body))

        for nonter in self.nonterminals:
            for ter in self.terminals:
                if len(table[nonter, ter]) == 0:
                    table[nonter, ter] = None
            if len(table[nonter, '$']) == 0:
                table[nonter, '$'] = None

        return table

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

                body = None
                if production[1] == '\'':
                    body = production[4:]
                else:
                    body = production[3:]

                body = str_to_arr(body)

                if body[0] == '~':
                    pass
                else:
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

def str_to_arr(body):
    res = []
    i = 0
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
    productions = []
    nonters = []
    symbols = []

    for cur in arr:
        if cur[1] == "'":
            head = cur[0:2]
            body = cur[4:]
        else:
            head = cur[0]
            body = cur[3:]

        nonters.append(head)

        bodies = body.split('|')
        for item in bodies:
            temp = []
            i = 0
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
    start = nonters[0]

    ter = list(set(symbols) - set(nonters) - set(['|']) - set('~'))
    return start, ter, nonters, productions

def common_prefix(arr):
    def foo(str1, str2):
        index = 0
        while index < min(len(str1), len(str2)):
            if str1[index] != str2[index]:
                break
            index += 1

        return str1[:index]

    length = len(arr)
    longest_prefix = ''
    for i in range(length):
        for j in range(i):
            cur = foo(arr[i], arr[j])
            if len(cur) > len(longest_prefix):
                longest_prefix = cur

    return longest_prefix

if __name__ == '__main__':
    # arr=['A->[B','B->X]|BA','X->Xa|Xb|a|b']
    # arr=['S->(T)|a+S|a','T->T,S|S']
    # arr=['E->E*T|E/T|T','T->T+F|T-F|F','F->(E)|i|iei']
    # arr=["E->TE'","E'->+E|~","T->FT'","T'->T|~","F->PF'","F'->*F'|~",'P->(E)|a|b|^']
    # arr=['S->BA','A->BS|d','B->aA|bS|c']
    # arr=['S->S*aP|aP|*aP','P->+aP|+a']
    # arr=['V->N|N[E]','E->V|V+E','N->i']
    # arr=['M->TB','T->Ba|~','B->Db|eT|~','D->d|~']
    # arr=['S->aSb|P','P->bPc|bQc','Q->Qa|a']
    # arr=['A->BaC|CbB','B->AC|c','C->Bb|b']
    # arr=['S->a|^|(T)','T->T,S|S']
    # arr=['E->[F]E|[F]','F->i|Fi']
    # arr=['S->iEtS|iEtSeS|a','E->b']
    # arr=['S->Qc|c','Q->Rb|b','R->Sa|a']
    # arr = ['S->aSe|B', 'B->bBe|C', 'C->cCe|d']
    # arr=["E->TE'","E'->+TE'|~","T->FT'","T'->*FT'|~","F->(E)|i"]
    arr=['E->E+T|T','T->T*F|F','F->(E)|i']
    # arr=['S->Aa|b','A->Ac|Sd|~']
    # arr = ['S->aSe|B', 'B->bBe|C', 'C->cCe|d']
    grammar = Grammar()
    grammar.insert_from_arr(arr)
    print(grammar.start)
    print(grammar.terminals)
    print(grammar.nonterminals)
    for (head,body) in grammar.productions:
        print(head+'->'+arr_to_str(body))

    print('左递归:',grammar.have_left_recursion())
    if grammar.have_left_recursion()==True:
        print('有左递归')
        grammar.eliminating_left_recursion()
        print('消除左递归后')
        print(grammar.start)
        print(grammar.terminals)
        print(grammar.nonterminals)

        for (head,body) in grammar.productions:
            print(head+'->'+arr_to_str(body))

    print('左公因子:',grammar.have_left_factor())
    if grammar.have_left_factor()==True:
        print('有左公因子')
        grammar.left_factoring()
        print('提取左公因子后')
        print(grammar.start)
        print(grammar.terminals)
        print(grammar.nonterminals)

        for (head,body) in grammar.productions:
            print(head+'->'+arr_to_str(body))


    print('-----first------')
    for symbol in grammar.nonterminals:
        print(symbol, grammar.first(symbol))

    follow=grammar.follow()
    print('-----follow------')
    for nonter in grammar.nonterminals:
        print(nonter, follow[nonter])

    print('LL(1):',grammar.is_LL1())
    table=grammar.consuct_predictive_parsing_table(follow)
    print('--------------table---------------')
    for nonter in grammar.nonterminals:
        for ter in grammar.terminals:
            print(nonter,ter,table[nonter,ter])
        print(nonter,'$',table[nonter,'$'])

    # step=grammar.predictive_parsing(table,'adccd')

    # for i in range(len(step)):
    #     print(step[i])
