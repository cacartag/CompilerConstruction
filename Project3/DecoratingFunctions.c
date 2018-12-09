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

int checkAddBlueNode(char * idLex, int type, int procParam)
{
  infraEye = infraTail;
  nodeInfrastructure tempGreenNode;
  
  if(procParam == 1)
  {
    tempGreenNode = infraTail;
  }

  while((infraEye != NULL) && (infraEye->type != HEAD))
  {
      //   
    if(!strcmp(idLex, infraEye->idLex) && (infraEye->greenBlue == BLUE_NODE) && (infraEye->type == type))
    {
      char semTempStr[100];
      sprintf(semTempStr,"Semantic Error: ID: %s already exists in this scope\n", idLex);
      listingPrintfSemantic(semTempStr);
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
  temp->procParam = procParam;
  temp->closed = 0;
  
  if(procParam == 1 )
  {
    if(tempGreenNode->procParam != 1)
    {
      tempGreenNode->procParam = 1;
    
      tempGreenNode->paramList = temp;
    }
    else
    {
      infraEye = tempGreenNode->paramList;
      
      while(infraEye->next != NULL)
      {
        infraEye = infraEye->next;
      }
      
      infraEye->next = temp;
    }
  }
  else
  {
    infraTail->next = temp;
    infraTail = temp;
  }
  
  return 0;
}

int checkAddGreenNode(char * idLex, int type)
{
  infraEye = infraTail;

  while((infraEye != NULL) && (infraEye->type != HEAD))
  {
    if(!strcmp(idLex, infraEye->idLex) && (infraEye->greenBlue == GREEN_NODE) && (infraEye->type == type))
    {
      char semTempStr[100];
      sprintf(semTempStr,"Semantic Error: ID: %s already exists in this scope\n", idLex);
      listingPrintfSemantic(semTempStr);
      return 1;
    }
    
    infraEye = infraEye->previous;
  }
  
  nodeInfrastructure temp = (nodeInfrastructure)malloc(sizeof(struct Node));
  temp->idLex = malloc(100);
  temp->previous = infraTail;
  temp->next = NULL;
  temp->paramList = NULL;
  strcpy(temp->idLex, idLex);
  temp->greenBlue = GREEN_NODE;
  temp->type = type;
  temp->closed = 0;
  
  infraTail->next = temp;
  infraTail = temp;
  
  return 0;  
}

void printInfrastructure()
{
  infraHead = infraHead->next;
  
  printf("\n\n");

  while(infraHead != NULL)
  {
    printf("%s    %s\n", infraHead->idLex,NumberToString(infraHead->type));
    
    if(infraHead->procParam == 1)
    {
      nodeInfrastructure tempEye = infraHead->paramList;
      
      while(tempEye != NULL)
      {
        printf("%s   %s,  this is a parameter\n", tempEye->idLex, NumberToString(tempEye->type));
        
        tempEye = tempEye->next;
      }

    }
    
    infraHead = infraHead->next;
  }

}

int checkArray(tokenNode first, tokenNode second)
{
    //printf("checking array %s %s\n", first->lexeme, second->lexeme);
    
    if(atoi(first->lexeme) > atoi(second->lexeme))
    {
      char semTempStr[100];
      sprintf(semTempStr,"Semantic Error: First number in array greater than second\n");
      listingPrintfSemantic(semTempStr);
      return 1;
    }
    
    if((first->type != INTEGER) || (second->type != INTEGER))
    {
      char semTempStr[100];
      sprintf(semTempStr,"Semantic Error: Numbers in array not integers\n");
      listingPrintfSemantic(semTempStr);
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

int checkAddOperator(int operation)
{
  if(operation == ADD_SYMBOL || operation == DIV_SYMBOL)
  {
    return 1;
  } else if(operation == OR)
  {
	return 0;
  }
  
  return -1;
}

int variableTypeRetrieval(tokenNode tok)
{
  infraEye = infraTail;
  
  while((infraEye != NULL) && (infraEye->type != HEAD))
  {
    if(!strcmp(tok->lexeme, infraEye->idLex))
    {
      return infraEye->type;
    }
    
    infraEye = infraEye->previous;
  }
  
  if(tok->type == INTEGER)
  {
	return INTEGER;
  } else {
  
    char semTempStr[100];
    sprintf(semTempStr,"Semantic Error: Could not find type for %s\n", tok->lexeme);
    listingPrintfSemantic(semTempStr);
  }
  
  return ERR;
}

void closeScope()
{
  infraEye = infraTail;
  nodeInfrastructure temp;
  int postGreen = 0;
  
  if(infraEye->greenBlue == GREEN_NODE)
  {
    temp = infraEye;
    postGreen = 1;
  }
  
  while(infraEye->greenBlue != GREEN_NODE || infraEye->closed == 1)
  {
    infraEye = infraEye->previous;
  }
  infraEye->closed = 1;
  
  if(postGreen == 1)
  {
    infraEye->next = temp;
  } else {
    infraEye->next = NULL;
    infraTail = infraEye;
  }
}



