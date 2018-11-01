# Engineer: Christopher Cartagena
#
# Description : 

import xlrd
from xlutils.copy import copy

loc = ("PascalParseTable.xlsx")

rb = xlrd.open_workbook(loc)
sheet = rb.sheet_by_index(0)

f1=open('SyntaxAnalyzer2.c','w+')
f2=open('Reserved.txt','r+')

parseTable = []

for x in range(1,41) :
  temp = []
  for y in range(0,34) :
    temp.append(sheet.cell_value(x,y))
  parseTable.append(temp)

terminalArray = []
terminalAssociative = {}

# this is hardcoded
for x in range(1,35) :
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

f1.write("\n \nunion attrib\n") 
f1.write("{\n")
f1.write("  uint32_t attr;\n")
f1.write("  uint32_t * attr_memory;\n")
f1.write("};\n")

f1.write("\nstruct token\n{")

f1.write("\n  uint32_t line;")
f1.write("\n  uint8_t * lexeme;")
f1.write("\n  uint32_t type;")
f1.write("\n  union attrib * attribute;")
f1.write("\n  struct token * next;")
f1.write("\n};")

f1.write("\n\ntypedef struct token *tokenNode;")
f1.write("\ntokenNode tok;\n")

createdFunctions = []

cCode = ""
# parsing through all of the symbols and their productions
for nonterminal in parseTable[0:41] :
  createdFunctions.append(nonterminal[0])
  cCode += "\nvoid "+nonterminal[0] + "()\n{"
  # parsing through all the productions

  epsilonArray = []
  synchArray = []
  
  if nonterminal[0] != "sgn" :
    cCode += "\n switch( tok->type )\n  {\n"
  else :
    cCode += "\n switch( tok->attribute->attr )\n  {\n"
  
  caseArray = []
  
  
  
  for index,production in enumerate(nonterminal[1:len(nonterminal)]) :
    # checking that there is an actual production for that terminal
    if production != "SE":
      
      if production != "e":      
        productionSplit = production.split(" ")
           
        # only add case if not previously added
        if terminalAssociative[terminalArray[index]] not in caseArray or nonterminal[0] == "sgn":
          if nonterminal[0] != "sgn":
            cCode += "\n    case " + str(terminalAssociative[terminalArray[index]]) + ": // terminal is " + terminalArray[index]
          else :
            if terminalArray[index] == "+" :
              cCode += "\n    case 71: //terminal is " + terminalArray[index]
            else :
              cCode += "\n    case 72: //terminal is " + terminalArray[index]
              
          for symbol in productionSplit :
          
            if symbol in terminalArray :
              cCode += "\n      match(\"" + symbol + "\");"
            else :
              cCode += "\n      "+symbol + "();" 
              
          cCode += "\n    break;\n"
          
          caseArray.append(terminalAssociative[terminalArray[index]])

      else :
        epsilonArray.append(terminalArray[index])

      synchArray.append(terminalArray[index])
 
    epTermExpress = ""
  
  if len(epsilonArray) > 0 :
  
    for index, terminal in enumerate(epsilonArray) :
      epTermExpress += "    case " + str(terminalAssociative[terminal]) + " : // terminal is " + terminal + ", epsilon do nothing\n    break;\n\n"
  
  cCode += "\n" + epTermExpress
  
  # otherwise statement used for default case
  synchArray.append("$")
  otherwise = "    default:\n      printf(\"Syntax Error: Expecting one of "
  synchWhile = "      int synchSet[] = {"
  
  for index, synchToken in enumerate(synchArray[:-1]) :
    otherwise += str(synchToken)
    synchWhile += str(terminalAssociative[synchToken])
    otherwise += " "
    synchWhile += ","

  otherwise += str(synchArray[len(synchArray) - 1]) + "\");\n";
  synchWhile += str(terminalAssociative[synchArray[len(synchArray) - 1]]) + "};"
  
  synchWhile += "\n\n      while( checkSynch(synchSet, tok->type, " + str(len(synchArray)) + ") )"
  synchWhile += "\n      {"
  synchWhile += "\n        tok = getToken();"
  synchWhile += "\n      }"
  
  otherwise += "\n" + synchWhile
  
  #switch statement end brace   
  cCode += otherwise + "\n  }\n}\n"

  
cCode += "\nvoid match(char * t)"
cCode += "\n{"
cCode += "\n  if ( !( strcmp(tok->lexeme, t) ) && ( strcmp(t, \"$\\0\") ) )"
cCode += "\n  {"
cCode += "\n    tok = getToken();"
cCode += "\n  }"
cCode += "\n  "
cCode += "\n  if ( !( strcmp(tok->lexeme, t) ) && !( strcmp(t, \"$\\0\") ) )"
cCode += "\n  {"
cCode += "\n    printf( \"Successfully parsed !\\n \");"
cCode += "\n  }"
cCode += "\n  "
cCode += "\n  if ( strcmp(tok->lexeme, t) )"
cCode += "\n  {"
cCode += "\n      printf(\"Syntax Error: Expecting %s, Received %s \\n\", t, tok->lexeme);"
cCode += "\n  }"
cCode += "\n}"

cCode += "\n\nint checkSynch(int * synchSet, int tokenType, int length)"
cCode += "\n{"
cCode += "\n    int found = 0;"
cCode += "\n"
cCode += "\n    for(int index = 0; index < length; index++)"
cCode += "\n    {"
cCode += "\n      if(synchSet[index] == tokenType)"
cCode += "\n      {"
cCode += "\n          found = 1;"
cCode += "\n      }"
cCode += "\n    }"
cCode += "\n    "
cCode += "\n    return found; "
cCode += "\n}"

cCode += "\n\nint main()\n{\n  printf(\"working now\\n\");\n}\n\n"
  
cCodeProto = ""
for function in createdFunctions :
  cCodeProto += "\nvoid " + function + "();"

cCodeProto += "\nvoid match();"
cCodeProto += "\nint checkSynch(int * synchSet, int tokenType, int length);"

cCodeProto += "\ntokenNode getToken();\n\n"
cCodeProto += "tokenNode getToken(){ \n  tokenNode tok;\n\n  return tok;\n}"
 
f1.write(cCodeProto + "\n\n" + cCode) 

f1.close()
