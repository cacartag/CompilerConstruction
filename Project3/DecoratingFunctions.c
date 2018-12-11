#include "LexicalAnalyzer.h" 
#define HEAD 500

FILE * pFile;

void initializeInfrastructure()
{
  pFile = fopen("MemoryMapping.txt","w");
    
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
  } else if(tok->type == REAL) 
  {
    return REAL;
  } else {
  
    char semTempStr[100];
    sprintf(semTempStr,"Semantic Error: Could not find type for %s\n", tok->lexeme);
    listingPrintfSemantic(semTempStr);
  }
  
  return ERR;
}

void closeScope(int * globalMemory)
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
  
  *globalMemory = pop();
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
	int matchAllTypes = 0;
	greenEye = greenNode->paramList;
	tempEye = parametersCurrentCall->next;
	
	while(greenEye != NULL)
	{
		printf("greenEye: %s , tempEye: %s\n", NumberToString(greenEye->type), NumberToString(tempEye->type));
		
		if(greenEye->type != tempEye->type)
		{
          char semTempStr[100];
          sprintf(semTempStr,"Semantic Error: Could not find procedure %s\n", idLex);
          listingPrintfSemantic(semTempStr);
		  
		  return 0;
		}	
		
		greenEye = greenEye->next;
		tempEye = tempEye->next;
	}
  }
  
  return 1;  
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

void addMemory(char * id, int type, int* sizeOfArray, int* globalMemory)
{
    
  if(type == INTEGER)
  {
    fprintf(pFile,"ID: %s, TYPE: %s, MEMORY: %i\n", id, NumberToString(type), *globalMemory);
    *globalMemory = *globalMemory + 4;
  }
    
  if(type == REAL)
  {
    fprintf(pFile,"ID: %s, TYPE: %s, MEMORY: %i\n", id, NumberToString(type), *globalMemory);
    *globalMemory = *globalMemory + 8;
  }
      
  if(type == AINT)
  {
    fprintf(pFile,"ID: %s, TYPE: %s, MEMORY: %i\n", id, NumberToString(type), *globalMemory);
    *globalMemory = *globalMemory + (*sizeOfArray * 4);
  }
      
  if(type == AREAL)
  {
    fprintf(pFile,"ID: %s, TYPE: %s, MEMORY: %i\n", id, NumberToString(type), *globalMemory);
    *globalMemory = *globalMemory + (*sizeOfArray * 8);
  }
  
  *sizeOfArray = 0;
      
  
  //printf("");
}

void newScope(int * globalMemory)
{
  push(*globalMemory);
  *globalMemory = 0;
}

void push (int item)
{
    if (s.top == (MAXSIZE - 1))
    {
        printf ("Stack is Full\n");
        return;
    }
    else
    {
        s.top = s.top + 1;
        s.stk[s.top] = item;
    }
    return;
}

/*  Function to delete an element from the stack */
int pop ()
{
    int num;
    if (s.top == - 1)
    {
        printf ("Stack is Empty\n");
        return (s.top);
    }
    else
    {
        num = s.stk[s.top];
        //printf ("poped element is = %dn", s.stk[s.top]);
        s.top = s.top - 1;
    }
    return(num);
}

/*  Function to display the status of the stack */
void display ()
{
    int i;
    if (s.top == -1)
    {
        printf ("Stack is empty\n");
        return;
    }
    else
    {
        printf ("\n The status of the stack is \n");
        for (i = s.top; i >= 0; i--)
        {
            printf ("%d\n", s.stk[i]);
        }
    }
    printf ("\n");
}

