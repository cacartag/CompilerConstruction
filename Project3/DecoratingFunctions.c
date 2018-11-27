#include "LexicalAnalyzer.h"
#define HEAD 125

void initializeInfrastructure()
{
  infraHead = (nodeInfrastructure)malloc(sizeof(struct Node));
  infraHead->previous = NULL;
  infraHead->next = NULL;
  infraHead->type = HEAD;
  
  infraTail = infraHead;
  infraEye = infraHead;
}

int checkAddBlueNode(char * idLex, int type)
{
 
  return 0; 
}

int checkAddGreenNode(char * idLex, int type)
{
    if((infraEye != NULL) && (infraEye->type != HEAD))
    {
      if(!strcmp(idLex, infraEye->idLex) && (infraEye->greenBlue == GREEN_NODE) && (infraEye->type == type))
      {
        printf("Semantic Error: ID: %s already exists in this scope\n", idLex);
        return 1;
      }
      
      infraEye = infraEye->previous;
    }
    
    nodeInfrastructure temp = (nodeInfrastructure)malloc(sizeof(struct Node));
    temp->previous = infraTail;
    temp->next = NULL;
    strcpy(temp->idLex, idLex);
    temp->greenBlue = GREEN_NODE;
    temp->type = PGM;
    
    infraTail->next = temp;
    
    return 0;
    
}


