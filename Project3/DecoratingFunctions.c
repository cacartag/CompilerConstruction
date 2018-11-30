#include "LexicalAnalyzer.h" 
#define HEAD 500

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
  infraEye = infraTail;
  
  //printf("Sanity Check blue node\n");
   
  while((infraEye != NULL) && (infraEye->type != HEAD))
  {
      //   
    if(!strcmp(idLex, infraEye->idLex) && (infraEye->greenBlue == BLUE_NODE) && (infraEye->type == type))
    {
      printf("Semantic Error: ID: %s already exists in this scope\n", idLex);
      return 1;
    }
    
    infraEye = infraEye->previous;
  }
  
  nodeInfrastructure temp = (nodeInfrastructure)malloc(sizeof(struct Node));
  temp->idLex = malloc(100);
  temp->previous = infraTail;
  temp->next = NULL;
  strcpy(temp->idLex, idLex);
  temp->greenBlue = BLUE_NODE;
  temp->type = type;
  
  infraTail->next = temp;
  infraTail = temp;
  
  return 0;
}

int checkAddGreenNode(char * idLex, int type)
{
  infraEye = infraTail;
  
  //printf("Sanity Check green node\n");
  
  while((infraEye != NULL) && (infraEye->type != HEAD))
  {
    if(!strcmp(idLex, infraEye->idLex) && (infraEye->greenBlue == GREEN_NODE) && (infraEye->type == type))
    {
      printf("Semantic Error: ID: %s already exists in this scope\n", idLex);
      return 1;
    }
    
    infraEye = infraEye->previous;
  }
  
  nodeInfrastructure temp = (nodeInfrastructure)malloc(sizeof(struct Node));
  temp->idLex = malloc(100);
  temp->previous = infraTail;
  temp->next = NULL;
  strcpy(temp->idLex, idLex);
  temp->greenBlue = GREEN_NODE;
  temp->type = type;
  
  infraTail->next = temp;
  infraTail = temp;
  
  return 0;  
}

void printInfrastructure()
{
  infraHead = infraHead->next;
  
  printf("\n\n");
  //printf("Inside printing \n");
  while(infraHead != NULL)
  {
    printf("%s    %s\n", infraHead->idLex,NumberToString(infraHead->type));
    
    infraHead = infraHead->next;
  }

}

int checkArray(tokenNode first, tokenNode second)
{
    //printf("checking array %s %s\n", first->lexeme, second->lexeme);
    
    if(atoi(first->lexeme) > atoi(second->lexeme))
    {
      printf("Semantic Error: First number in array greater than second\n");
      
      return 1;
    }
    
    if((first->type != INTEGER) || (second->type != INTEGER))
    {
        printf("Semantic Error: Numbers in array not integers\n");
        
        return 1;
    }
    
    return 0;
}

int checkMulOperator(int operation)
{
  if(operation == MUL_SYMBOL || operation == DIV_SYMBOL || operation == MOD || operation == DIV)
  {
      return 1;
  } else if(operation == AND)
  {
      return 0;
  }
  
  return -1;
}

