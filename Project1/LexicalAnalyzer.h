#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Reserved Words
#define IF 1
#define THEN 2
#define ID 3
#define PROG 4
#define VAR 5
#define ARRA 6
#define NUM 7
#define OF 8
#define INT 9
#define REAL 10
#define FUNC 11
#define PROC 12
#define BEGI 13
#define END 14
#define ASSIGN 15
#define ELSE 16
#define WHILE 17
#define DO 18
#define OR 19
#define DIV 20
#define MOD 21
#define AND 22
#define NOT 23

#define LEXERR 24

#define LONGSTRING 25

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
void RetrieveTerminals(tokenNode *reservedHead);
void RetrieveReservedWords(tokenNode *reservedHead);
uint32_t CheckReservedList(char * lexeme, tokenNode *reservedHead, uint32_t *type, uint32_t *attribute);
void AddToTokenLinked(tokenNode * sourceTokens, uint8_t * lexeme, uint32_t type, uint32_t attribute);





