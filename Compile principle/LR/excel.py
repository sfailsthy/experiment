# -*- coding: utf-8 -*-

#把分析过程写入Excel文件中

from pyexcelerate import Workbook

data =  # data is a 2D array
wb = Workbook()
wb.new_sheet("step", data=data)
wb.save("output.xlsx")
