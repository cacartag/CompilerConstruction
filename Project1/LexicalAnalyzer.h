#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Reserved Words
#define IF 1
#define THEN 2
#define PROG 3
#define VAR 4
#define ARRA 5
#define NUM 6
#define OF 7
#define INT 8
#define REAL 9
#define FUNC 10
#define PROC 11
#define BEGI 12
#define END 13
#define ASSIGN 14
#define ELSE 15
#define WHILE 16
#define DO 17
#define OR 18
#define DIV 19
#define MOD 20
#define AND 21
#define NOT 22
#define INTGR 23
#define ID 24

#define LEXERR 30

#define LONGSTRING 31
#define LEADINGZERO 32
#define TRAILINGZERO 33
#define LONGINT 34
#define LONGREAL 35

#define ASSIGNOP 70
#define ADDSYM 71
#define SUBSYM 72
#define MULTSYM 73
#define DIVSYM 74
#define COLON 75
#define OPBRACK 76
#define CLBRACK 77
#define PERIOD 78
#define SEMICOLON 79
#define OPPARAN 81
#define CLPARAN 82
#define COMMA 83

#define GT 120
#define LT 121
#define GTE 122
#define LTE 123
#define EQU 124
 
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

typedef struct token *tokenNode;
typedef union attrib *attributes;
uint32_t currentLine = 1;

int WhiteSpaceMachine(int *bPosition, int *fPosition, uint8_t * buffer);
int AnalyzeLine(tokenNode *reservedHead, tokenNode *sourceTokens, uint8_t *buffer);
int IdResolutionMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *reservedHead, tokenNode *sourceTokens);
int CatchAllMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *terminalHead, tokenNode *sourceTokens);
int RelationalOperatorMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens);
int RetrieveAnyTypeNumber(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens);
int IntegerMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens);
int ShortRealMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens);
void RetrieveTerminals(tokenNode *reservedHead);
void RetrieveReservedWords(tokenNode *reservedHead);
uint32_t CheckReservedList(char * lexeme, tokenNode *reservedHead, uint32_t *type, uint32_t *attribute);
void AddToTokenLinked(tokenNode * sourceTokens, uint8_t * lexeme, uint32_t type, uint32_t attribute);


