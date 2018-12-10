#include "LexicalAnalyzer.h" 
#define HEAD 500

void initializeInfrastructure()
{
  parametersCurrentCall = (nodeInfrastructure)malloc(sizeof(struct Node));
  parametersCurrentCall->previous = NULL;
  parametersCurrentCall->next = NULL;
  parametersCurrentCall->type = HEAD;
  
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

  if(procParam == 1)
  {
    nodeInfrastructure tempEye = infraTail;
    
    while(tempEye->greenBlue != GREEN_NODE)
    {
      tempEye = tempEye->previous;
    }
    
    addProcessParameter(tempEye,idLex, type);
  }
  while((infraEye != NULL) && (infraEye->type != HEAD))
  {

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

  infraTail->next = temp;
  infraTail = temp;
  
  return 0;
}

void addProcessParameter(nodeInfrastructure tempGreenNode, char * idLex, int type)
{
  nodeInfrastructure tempCopy = (nodeInfrastructure)malloc(sizeof(struct Node));
  tempCopy->idLex = malloc(100);
  tempCopy->next = NULL;
  strcpy(tempCopy->idLex, idLex);
  tempCopy->greenBlue = BLUE_NODE;
  tempCopy->type = type;
  tempCopy->procParam = 1;
  tempCopy->closed = 0;
  
  if(tempGreenNode->procParam != 1)
  {
    tempGreenNode->procParam = 1;
    tempGreenNode->paramList = tempCopy;
  }
  else
  {
    nodeInfrastructure tempEye = tempGreenNode->paramList;

    while(tempEye->next != NULL)
    {
      tempEye = tempEye->next;
    }

    tempEye->next = tempCopy;
  }
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

int checkIfProcedureExists(uint8_t * idLex)
{
  infraEye = infraTail;
  
  nodeInfrastructure greenNode;
  int foundGreenNode = 0;

  while((infraEye != NULL) && (infraEye->type != HEAD))
  {
    if(!strcmp(idLex, infraEye->idLex) && (infraEye->greenBlue == GREEN_NODE))
    {
      foundGreenNode = 1;
      greenNode = infraEye;
      //return 1;
    }
    
    infraEye = infraEye->previous;
  }    
  
  if(foundGreenNode == 0)
  {
        char semTempStr[100];
  sprintf(semTempStr,"Semantic Error: Could not find procedure %s\n", idLex);
  listingPrintfSemantic(semTempStr);
  
  return 0;
  }

  nodeInfrastructure tempEye = parametersCurrentCall;
  
  tempEye = tempEye->next;  
  
  int numberOfFunctionParameters = 1;
  int numberOfRetrievedArgs = 0;

  while(tempEye != NULL )
  {
    //printf("type:%s\n",NumberToString(tempEye->type));
    numberOfRetrievedArgs = numberOfRetrievedArgs + 1;
    tempEye = tempEye->next;
  }
  
  nodeInfrastructure greenEye = greenNode->paramList;
  
  while(greenEye->next != NULL)
  {
    numberOfFunctionParameters = numberOfFunctionParameters + 1;
    greenEye = greenEye->next;
  }

  if((foundGreenNode == 1)&&(numberOfFunctionParameters == numberOfRetrievedArgs))
  {
    return 1;
  }
  
  char semTempStr[100];
  sprintf(semTempStr,"Semantic Error: Could not find procedure %s\n", idLex);
  listingPrintfSemantic(semTempStr);
  
  return 0;  
}

void addParametersToCurrentProcedure(int type)
{
  //printf("adding parameter: %s\n", NumberToString(type));
  nodeInfrastructure tempEye = parametersCurrentCall;

  nodeInfrastructure temp = (nodeInfrastructure)malloc(sizeof(struct Node));
  temp->next = NULL;
  temp->type = type;
  
  while(tempEye->next != NULL)
  {
    tempEye = tempEye->next;
  }
  
  tempEye->next = temp;
}

void deleteParametersToCurrentProcedure()
{
  parametersCurrentCall->next = NULL;
}



