#import xlrd
#from xlutils.copy import copy
#
#terminalSymbolsFile = open("TerminalSymbols.txt","r")
#
#termSymbols = []
#currSymbol = terminalSymbolsFile.readline()
#
#while currSymbol is not "" :
#  termSymbols.append(currSymbol.rstrip())
#  currSymbol = terminalSymbolsFile.readline()
#  
#duplicates = [] 
# 
#for terminal in termSymbols :
#  if (termSymbols.count(terminal) > 1) and (duplicates.count(terminal) == 0):
#    duplicates.append(terminal)
#    print "Multiple instances of " + terminal
#  
#if len(duplicates) > 0 :
#  print "Could not proceed duplicates exist"
#
#loc = ("PascalParseTable.xls")
#
#rb = xlrd.open_workbook(loc)
#wb = copy(rb)
##sheet = wb.sheet_by_index(0)
#sheet = wb.get_sheet(0)
#
#x = 1
#for terminal in termSymbols :
#  sheet.write(0,x, terminal)
#  x = x + 1
#  
#wb.save("PascalParseTable.xls")
#
##print(sheet.cell_value(0,1))
##print(sheet.nrows)
#
#








