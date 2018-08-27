#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
 
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
  union attrib attribute;
};

typedef struct terminal *terminalNode;

int WhiteSpaceMachine(int *bPosition, int *fPosition, uint8_t * buffer);
int AnalyzeLine();
int IdResolutionMachine(int *bPosition, int *fPosition, uint8_t * buffer);
void RetrieveTerminals(terminalNode *head);



