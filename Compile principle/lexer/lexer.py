#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

keywords = ['do', 'end', 'for', 'if',
            'printf', 'scanf', 'then', 'while']  # 关键字表
delimiters = [',', ';', '(', ')', '[', ']']  # 分界符表
arithmetic_operators = ['+', '-', '*', '']  # 算术运算符表
relational_operators = ['<', '<=', '>', '>=', '=', '<>']


is_alpha = lambda char: True if 'a' <= char <= 'z' or 'A' <= char <= 'Z' else False
is_digit = lambda char: True if '0' <= char <= '9' else False


def analy(program):
    tokens = []

    for row, curline in enumerate(program):
        column = 1

        index = 0
        while index < len(curline):
            while curline[index] == ' ' or curline[index] == '\t':  # 空格或者制表符，则跳过
                index += 1
                if index >= len(curline):
                    break

            if is_alpha(curline[index]):
                # 是字母
                token = curline[index]
                index += 1
                if index >= len(curline):
                    if token in keywords:
                        # 是关键字
                        tokens.append(
                            (token, (1, token), 'keyword', (row + 1, column)))

                    else:
                        # 是标识符
                        tokens.append(
                            (token, (6, token), 'identifier', (row + 1, column)))

                    break

                while is_alpha(curline[index]) or is_digit(curline[index]):
                    token += curline[index]
                    index += 1
                    if index >= len(curline):
                        break

                if token in keywords:
                    # 是关键字
                    tokens.append(
                        (token, (1, token), 'keyword', (row + 1, column)))

                else:
                    # 是标识符
                    tokens.append(
                        (token, (6, token), 'identifier', (row + 1, column)))

                column += 1

                if index >= len(curline):
                    break

            elif is_digit(curline[index]):
                # 是数字
                value = 0
                while is_digit(curline[index]):
                    value = 10 * value + int(curline[index])
                    index += 1
                    if index >= len(curline):
                        break

                if index >= len(curline):
                    tokens.append(
                        (value, (5, value), 'num', (row + 1, column)))

                    break

                if curline[index] != '.':
                    if is_alpha(curline[index]):
                        s = str(value)
                        while is_alpha(curline[index]):
                            s += curline[index]
                            index += 1
                            if index >= len(curline):
                                break

                        tokens.append((s, 'Error', 'Error', (row + 1, column)))
                        if index >= len(curline):
                            break
                    else:
                        tokens.append(
                            (value, (5, value), 'num', (row + 1, column)))
                    column += 1

                else:
                    x = value
                    d = 10
                    index += 1
                    while is_digit(curline[index]):
                        x = x + int(curline[index]) / d
                        d = d * 10
                        index += 1
                        if index >= len(curline):
                            break

                    if index >= len(curline):
                        tokens.append((x, (5, x), 'num', (row + 1, column)))

                        column += 1
                        break

                    tokens.append((x, (5, x), 'num', (row + 1, column)))

                    column += 1

            elif curline[index] == '+':
                index += 1
                if index >= len(curline):
                    tokens.append(
                        ('+', (3, '+'), 'arithmetic_operator', (row + 1, column)))
                    # print(tokens[-1])
                    break
                else:
                    if curline[index] == '+':
                        tokens.append(
                            ('++', 'Error', 'Error', (row + 1, column)))
                        # print(tokens[-1])
                        column += 1
                        index += 1
                    else:
                        tokens.append(
                            ('+', (3, '+'), 'arithmetic_operator', (row + 1, column)))
                        # print(tokens[-1])
                        index += 1
                        continue

            elif curline[index] == '-':
                tokens.append(
                    ('-', (3, '-'), 'arithmetic_operator', (row + 1, column)))
                # print(tokens[-1])
                index += 1
                column += 1

            elif curline[index] == '*':
                tokens.append(
                    ('*', (3, '*'), 'arithmetic_operator', (row + 1, column)))
                index += 1
                column += 1

            elif curline[index] == '/':
                index += 1
                if curline[index] == '/':
                    break
                else:
                    tokens.append(
                        ('/', (3, '/'), 'arithmetic_operator', (row + 1, column)))
                    column += 1

            elif curline[index] == '<':
                index += 1
                if index >= len(curline):
                    tokens.append(
                        ('<', (4, '<'), 'relational_operator', (row + 1, column)))
                    break
                else:
                    if curline[index] == '=':
                        tokens.append(
                            ('<=', (4, '<='), 'relational_operator', (row + 1, column)))
                        column += 1
                        index += 1

                    else:
                        tokens.append(
                            ('<', (4, '<'), 'relational_operator', (row + 1, column)))
                        index += 1
                        continue

            elif curline[index] == '>':
                index += 1
                if index >= len(curline):
                    tokens.append(
                        ('>', (4, '>'), 'relational_operator', (row + 1, column)))

                    break
                else:
                    if curline[index] == '=':
                        tokens.append(
                            ('>=', (4, '>='), 'relational_operator', (row + 1, column)))

                        column += 1
                        index += 1
                    else:
                        tokens.append(
                            ('>', (4, '>'), 'relational_operator', (row + 1, column)))

                        index += 1
                        column += 1
                        continue

            elif curline[index] == '=':
                tokens.append(
                    ('=', (4, '='), 'relational_operator', (row + 1, column)))

                index += 1
                column += 1

            elif curline[index] in delimiters:
                tokens.append(
                    (curline[index], (2, curline[index]), 'delimiter', (row + 1, column)))

                index += 1
                column += 1
                continue

            else:
                tokens.append((curline[index], 'Error',
                               'Error', (row + 1, column)))
                index += 1
                column += 1

    return tokens


if __name__ == '__main__':
    fr = open('program.txt', encoding='utf-8')
    program = ''
    for line in fr.readlines():
        program += line

    program = program.split('\n')
    tokens = analy(program)

    print('-' * 20)
    for item in tokens:
        print(item)
