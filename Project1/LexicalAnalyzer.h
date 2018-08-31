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

int WhiteSpaceMachine(int *bPosition, int *fPosition, uint8_t * buffer);
int AnalyzeLine(tokenNode reservedHead, tokenNode sourceTokens, uint8_t *buffer);
int IdResolutionMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode reservedHead, tokenNode sourceTokens);
int CatchAll(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *terminalHead);
void RetrieveTerminals(tokenNode *rHead);
void RetrieveReservedWords(tokenNode *rHead);
void AddToTokenLinked(tokenNode * sourceTokens, uint8_t * lexeme, uint32_t type, uint32_t attribute);






