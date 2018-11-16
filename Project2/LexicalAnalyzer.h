#ifndef LEXICALANALYZER_H
#define	LEXICALANALYZER_H

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Reserved Words
#define IF 1
#define THEN 2
#define PROGRAM 3
#define VAR 4
#define ARRAY 5
#define OF 6
#define INTEGER 7
#define REAL 8
#define PROCEDURE 9
#define BEGIN 10
#define END 11
#define ELSE 12
#define WHILE 13
#define DO 14
#define OR 15
#define DIV 16
#define MOD 17
#define AND 18
#define NOT 19
#define ID 20
#define CALL 21
#define VALUE 22

// I don't know about this one
#define NUM 23

// Lexical error
#define LEXERR 30

#define LONG_STRING 31
#define LEADING_ZERO 32
#define TRAILING_ZERO 33
#define INT_TOO_LONG 34
#define REAL_BEFORE_DECIMAL_TOO_LONG 35
#define REAL_AFTER_DECIMAL_TOO_LONG 36
#define LONG_INT 37
#define ENDING_ZEROS 38
#define EXPONENT_TOO_LONG 39
#define UNIDENTIFIED_SYMBOL 40
#define EXPONENT_IS_ZERO 41
#define NO_EXPONENT_SPECIFIED 42

// Symbols
#define ADD_SYMBOL 71
#define SUB_SYMBOL 72
#define MUL_SYMBOL 73
#define DIV_SYMBOL 74
#define COLON 75
#define OPEN_CURLY_BRACKET 76
#define CLOSED_CURLY_BRACKET 77
#define PERIOD 78
#define SEMICOLON 79
#define OPEN_PARENTHESES 80
#define CLOSED_PARENTHESES 81
#define COMMA 82
#define DOUBLE_PERIOD 83
#define OPEN_BRACKET 84
#define CLOSED_BRACKET 85

#define GT 120
#define LT 121
#define GTE 122
#define LTE 123
#define EQU 124
#define NE 125

#define RELOP 160
#define ADDOP 161
#define MULOP 162
#define ASSIGNOP 163
 
#define END_OF_FILE 200
 
struct terminal { 
  char * symbol; 
  struct terminal * next; 
};

union attrib 
{
  uint32_t attr;
  uint32_t * attr_memory;
};

struct token
{
  uint32_t line;
  uint8_t * lexeme;
  uint32_t type;
  union attrib * attribute;
  struct token * next;
};

struct syntaxError
{
  uint32_t line;
  char * syntaxErr;
  char * lexemeTest;
  struct syntaxError * next;
};

typedef struct token *tokenNode;
typedef union attrib *attributes;
typedef struct syntaxError *syntax;

tokenNode sourceTokens;
tokenNode symbolTable;
tokenNode symbolTableHead;
syntax syntaxErrors;
syntax syntaxHead;

int WhiteSpaceMachine(int *bPosition, int *fPosition, uint8_t * buffer);
int AnalyzeLine(tokenNode *reservedHead, tokenNode *sourceTokens, uint8_t *buffer);
int IdResolutionMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *reservedHead, tokenNode *sourceTokens);
int CatchAllMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *terminalHead, tokenNode *sourceTokens);
int RelationalOperatorMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens);
int RetrieveAnyTypeNumber(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens);
int UnidentifiedSymbol(int *bPosition, uint8_t * buffer, tokenNode *sourceTokens);
void IntegerMachine(uint8_t * buffer, tokenNode *sourceTokens);
void ShortRealMachine(uint8_t * buffer, tokenNode *sourceTokens);
void LongRealMachine(uint8_t * buffer, tokenNode *sourceTokens);
void RetrieveTerminals(tokenNode *reservedHead);
void RetrieveReservedWords(tokenNode *reservedHead);
uint32_t CheckReservedList(char * lexeme, tokenNode *reservedHead, uint32_t *type, uint32_t *attribute);
void AddToTokenLinked(tokenNode * sourceTokens, uint8_t * lexeme, uint32_t type, uint32_t attribute);
char * NumberToString(int Number);
int TrailingZeroCheck(char tempBuff);
int LeadingZeroCheck(char * tempBuff);
void OutputTokens(tokenNode sourceTokens);
void OutputListings(tokenNode sourceTokens, FILE * pFile);
void PrintLexicalErrors(tokenNode sourceTokens, int tempLine, FILE * pFile);
void AddToSymbolTable(tokenNode *sourceTokens, uint8_t * lexeme, uint32_t type, uint32_t attribute);
void PrintSyntaxErrors(int tempLine, FILE * pFile);


void parse();
void prgrm();
void prgrmp();
void prgrmpp();
void id_list();
void id_listp();
void decls();
void declsp();
void type();
void stndrd_type();
void subprgrm_decls();
void subprgrm_declsp();
void subprgrm_decl();
void subprgrm_declp();
void subprgrm_declpp();
void subprgrm_head();
void subprgrm_headp();
void args();
void param_lst();
void param_lstp();
void cmpnd_stmnt();
void cmpnd_stmntp();
void opt_stmnt();
void stmnt_lst();
void stmnt_lstp();
void stmnt();
void variable();
void variablep();
void procdr_stmnt();
void procdr_stmntp();
void express_lst();
void express_lstp();
void express();
void expressp();
void simp_express();
void simp_expressp();
void term();
void termp();
void factor();
void factorp();
void sgn();
void match(const char * t);
int checkSynch(int * synchSet, int tokenType, int length);
tokenNode getToken();
void listingPrintf(char * synTempStr);

// need to add detection for token types of mult, add, and assign
#endif	
