
print ("Replacing printf with sprintf, and custom\n")

syntaxSource = open("SyntaxAnalyzer.c","r")
syntaxTarget = open("SyntaxAnalyzer2.c", "w")

for line in syntaxSource:
  index = line.find("printf")
  if index == -1:
    syntaxTarget.write(line.rstrip() + "\n")
  else:
    syntaxTarget.write("      ;\nchar synTempStr[100];\n")
    syntaxTarget.write("      sprintf(synTempStr,"+ line.rstrip()[index + 7:len(line.rstrip())] + "\n")
    syntaxTarget.write("      listingPrintf(synTempStr);\n")
    
#char * synTempStr;
#sprintf(synTempStr, );


