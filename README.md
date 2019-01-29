# CompilerConstruction
## By: Christopher Cartagena

## Overview
In this project a pascal compiler will be made that will consist of a lexical analyzer, a syntax analyzer, a semantic analyzer, and finally a memory assignment module.  Each one will be written in their own project with the exception of the memory module which will be included into project 3.  C will be the primary language used to write the compiler, however python will be used to introduce some automatic programming to relieve some of the repetitive parts of the compiler code.  More information on the program as well as the grammar can be found in the final report.


## Input
The input for this program is pascal grammar, an example of this can be found inside the project 3 folder, which is the final program, under the name cor34.

## Output
The output consists of 3 files, the first is the ListingOutput.txt file which consists of the code that was written as well as any errors that were encountered.  The second is the TokenOutput.txt file which is the name and type of all the tokens that were retrieved from the file.  The final file is the MemoryMapping.txt file, which is all of the variables that were retrieved along with their memory location in their respective scopes.

## How to compile and run
The final program is inside the program 3 folder, the way to compile it is with the following command:

`
gcc DecoratingFunctions.c LexicalAnalyzer.c SyntaxAnalyzer.c -o Compiler.exe
`

The program is ran with the following command:
`
Compiler.exe <Name of file>
`  

Where name of file is the pascal program to parse.
