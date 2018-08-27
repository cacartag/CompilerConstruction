#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

struct terminal {
  char * symbol;
  struct terminal * next;
};

typedef struct terminal *node;

int main()
{
  FILE *terminalFile;
  FILE *reservedFile;

  printf("Reading in file \n");

  terminalFile = fopen("Terminal.txt","r+");

  node terminalHead;
  node terminalForward = (node)malloc(sizeof(struct terminal));
  char * currentSymbol = malloc(8);
  node temp;  
 
  terminalHead = terminalForward;

  printf("Found Terminals:\n");
  while(fgets(currentSymbol,7,terminalFile) != NULL)
  {
    // printf("Single iteration\n");
    terminalForward->symbol = malloc(8);
    strcpy(terminalForward->symbol,currentSymbol);
    temp = (node)(malloc(sizeof(struct terminal)));

    terminalForward->next = temp; 
    terminalForward = temp;
  }

  terminalForward->next = NULL;

  printf("Terminal Linked List Printout:\n");
  terminalForward = terminalHead;
 
  int count = 1;

  while(terminalForward->next != NULL)
  {
    printf("%d %s", count, terminalForward->symbol);
    terminalForward = terminalForward->next; 
    count = count + 1;
  }

  return 0;
}



