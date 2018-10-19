import xlrd
from xlutils.copy import copy

loc = ("PascalParseTable.xlsx")

rb = xlrd.open_workbook(loc)
sheet = rb.sheet_by_index(0)

f1=open('testfile.c','w+')
f2=open('Reserved.txt','r+')

parseTable = []

for x in range(1,41) :
  temp = []
  for y in range(0,34) :
    temp.append(sheet.cell_value(x,y))
  parseTable.append(temp)

terminalArray = []
terminalAssociative = {}

for x in range(1,34) :
  terminalArray.append(sheet.cell_value(0,x))
  terminalAssociative[sheet.cell_value(0,x)] = -1  
  #print(sheet.cell_value(0,x))
 
for line in f2 :
  if line.rstrip() in terminalAssociative : 
    terminalAssociative[line.rstrip()] = f2.next().rstrip()

#for key, value in terminalAssociative.iteritems():
#  print key, value




f1.write("#include <stdio.h>")
f1.write("\n#include <stdint.h>")
f1.write("\n#include <ctype.h>")
f1.write("\n#include <stdlib.h>")
f1.write("\n#include <string.h>")
f1.write("\n#include <stdbool.h>")

f1.write("\n\nstruct token\n{")

f1.write("\n  uint32_t line;")
f1.write("\n  uint8_t * lexeme;")
f1.write("\n  uint32_t type;")
f1.write("\n  union attrib * attribute;")
f1.write("\n  struct token * next;")
f1.write("\n};")

f1.write("\n\ntypedef struct token *tok;\n") 

createdFunctions = []

cCode = ""
# parsing through all of the symbols and their productions
for nonterminal in parseTable[0:41] :
  createdFunctions.append(nonterminal[0])
  cCode += "\nvoid "+nonterminal[0] + "()\n{"
  # parsing through all the productions

  epsilonArray = []
  
  cCode += "\n switch( tok )\n  {\n"
  
  for index,production in enumerate(nonterminal[1:len(nonterminal)]) :
    # checking that there is an actual production for that terminal
    if production != "SE":
      
      if production != "e" :      
        productionSplit = production.split(" ")

        #cCode += "\n    if(strcmp(\""+terminalArray[index]+"\",\"place\"))\n  {"
                        
        cCode += "\n    case "+terminalArray[index]+":  "
                
        for symbol in productionSplit :
        
          if symbol in terminalArray :
            cCode += "\n      match(\"" + symbol + "\");"
          else :
            cCode += "\n      "+symbol + "();" 

      else :
        #print(terminalArray[index])
        epsilonArray.append(terminalArray[index])
 
    epTermExpress = ""
  
  #switch statement end brace
  cCode += "\n  }\n"
    
  if len(epsilonArray) > 0 :
  
    epTermExpress += "  if ("
    for index, terminal in enumerate(epsilonArray) :
      #epTermExpress += "strcmp(\"" + terminal + "\", \"place\")  "
     
      if index != len(epsilonArray) - 1 :
        epTermExpress += "|| \n    "
      else :
        epTermExpress += ")"
  
    epTermExpress += "\n  {\n\n  }\n"       
     
  cCode += "\n" + epTermExpress
    
  cCode += "\n}\n"

cCode += "\nvoid match(char * mat)\n{\n}\n\n"
  
cCode += "\n\nint main()\n{\n  printf(\"working now\\n\");\n}\n\n"
  
cCodeProto = ""
for function in createdFunctions :
  cCodeProto += "\nvoid " + function + "();"

cCodeProto += "\nvoid match();"
 
f1.write(cCodeProto + cCode) 
 

      #print(production)

#for x in range(1,40) :
#  for y in range(0,42) :
#    print(parseTable[x][y])
    #print("{} and {}".format(x,y))
  
#  f1.write(sheet.cell_value(x,0) + "\n")
#  f1.write('{\n')
#  for x in range(10) :
#    f1.write('\n')
#  f1.write('}\n\n\n')
  
f1.close()

#print(sheet.cell_value(2,0))
#print(sheet.nrows)