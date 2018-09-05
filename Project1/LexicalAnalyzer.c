#include "LexicalAnalyzer.h"

int main()
{
  FILE * pFile;
  uint8_t sourceLine[73];


  tokenNode reservedHead;
  tokenNode sourceTokens = (tokenNode)(malloc(sizeof(struct token))); 
  sourceTokens->next = NULL;  
  
  RetrieveReservedWords(&reservedHead);

  int count = 1;

  tokenNode tempReservedHead = reservedHead;
  while(tempReservedHead->next != NULL)
  {
    if (tempReservedHead->lexeme[strlen(tempReservedHead->lexeme)-1] == '\n')
    {
      tempReservedHead->lexeme[strlen(tempReservedHead->lexeme)-2] = '\0';
    }
    tempReservedHead = tempReservedHead->next; 
  }

/*
    printf("%d %s %d %d \n", count, reservedHead->lexeme, reservedHead->type, reservedHead->attribute->attr);
    reservedHead = reservedHead->next; 
    count = count + 1;
  }
*/

  // start by opening up file with program
  pFile = fopen("program1","r+");

/*
   while(fgets(sourceLine,72,pFile) != NULL)
   {
     //printf("%s",sourceLine);
     AnalyzeLine(&reservedHead, &sourceTokens, sourceLine);
     currentLine = currentLine + 1;
   }
*/

  fgets(sourceLine,72,pFile);
  AnalyzeLine(&reservedHead, &sourceTokens, sourceLine);

  if(sourceTokens->next != NULL)
  {
    sourceTokens = sourceTokens->next;
  }

  while(sourceTokens->next != NULL)
  {
    printf("%d %s %d %d\n", sourceTokens->line, sourceTokens->lexeme, sourceTokens->type, sourceTokens->attribute->attr);
    sourceTokens = sourceTokens->next; 
  }

  // printf("%d %s %d %d\n", sourceTokens->line, sourceTokens->lexeme, sourceTokens->type, sourceTokens->attribute->attr);

  // RetrieveTerminals(&reservedHead);

  // used to check the values of the token linked list
   
  return 0;
}

// processes buffer containing a single line of code
int AnalyzeLine(tokenNode *reservedHead, tokenNode *sourceTokens, uint8_t * buffer)
{
  // base position keeps track of starting position
  // forward position moves to keep track of current processed character
  // atend keeps track of if the end of the buffer has been reached
  // stuck keeps track of wether anything new has been processed
  int basePosition = 0;
  int forwardPosition = 0;
  int atEnd = 1;
  int stuck = 0;  

  // while loop goes through each finite statemachine for processing
  // until the end of the buffer is reached or none of the machines can process
  // the current values. 

  while((atEnd == 1) && (stuck == 0))
  {

    tokenNode headSourceTokens = *sourceTokens;

    stuck = basePosition;

    atEnd = RetrieveAnyTypeNumber(&basePosition, &forwardPosition, buffer, sourceTokens);
/*
    atEnd = WhiteSpaceMachine(&basePosition, &forwardPosition, buffer);
    atEnd = IdResolutionMachine(&basePosition, &forwardPosition, buffer, reservedHead, &headSourceTokens);
    atEnd = CatchAllMachine(&basePosition, &forwardPosition, buffer, reservedHead, sourceTokens);
    atEnd = RelationalOperatorMachine(&basePosition, &forwardPosition, buffer, sourceTokens);
    atEnd = IntegerMachine(&basePosition, &forwardPosition, buffer, sourceTokens);
*/
    if((basePosition - stuck) <= 0)
    {
      stuck = 1;
    } else {
      stuck = 0;
    }
     
  }

  return 0;
}

// moves index past whitespace if possible, else does not move index
// returns 0 if end of line reached, 1 otherwise
int WhiteSpaceMachine(int *bPosition, int *fPosition, uint8_t * buffer)
{
  int other = 1;
  *fPosition = *bPosition;
  while(other == 1)
  {

    // printf("fPosition = %i, char is %c \n", *fPosition, buffer[*fPosition]);

    if((buffer[*fPosition] == '\b') || (buffer[*fPosition] == '\t') || (buffer[*fPosition] == '\n') || (buffer[*fPosition] == ' '))
    {
      *fPosition = *fPosition + 1;
    } else {
      other = 0;
    }
  }

  *bPosition = *fPosition;

  if(buffer[*fPosition] == '\n')
  {
    return 0;
  } else {
    return 1;
  }
  
}

// moves index past identifiers if possible, else does not move index
// returns 0 if end of line reached, 1 otherwise
// if any were found they will be added to provided token linked list
int IdResolutionMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *reservedHead, tokenNode *sourceTokens)
{
  char * id = malloc(13);

  int other = 1;
  *fPosition = *bPosition;

  // check if first letter is an alphabetical letter
  if(isalpha(buffer[*bPosition]) != 0)
  {
    uint32_t type;
    uint32_t attribute;

    id[0] = buffer[*fPosition];
    *fPosition = *fPosition + 1;
 
    // go through line looking for the rest of the identifier 
    while(other == 1 && (*fPosition - *bPosition) <= 10)
    {
      // check if the next value is either an alphabetical letter or a digit 
      if(isalpha(buffer[*fPosition]) != 0 || isdigit(buffer[*fPosition]) != 0 )
      {
        id[*fPosition - *bPosition] = buffer[*fPosition];
        *fPosition = *fPosition + 1;
      } else {
        other = 0;
      }

    }
    
    id[(*fPosition - *bPosition) + 1] = '\0';

    if((*fPosition - *bPosition) > 10)
    {
      AddToTokenLinked(sourceTokens,id,LEXERR, LONGSTRING);
    } else {
      CheckReservedList(id, reservedHead, &type, &attribute);
      AddToTokenLinked(sourceTokens, id, type, attribute);
    }

    *bPosition = *fPosition;
  }

  if(buffer[*fPosition] == '\n') {
    return 0;
  } else {
    return 1;
  }
}

// moves index past catch all terminals if possible, else does not move index
// returns 0 if end of line reached, 1 otherwise
// if any were found they will be added to provided token linked list
int CatchAllMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *reservedHead, tokenNode *sourceTokens)
{
  *fPosition = *bPosition;  

  int type = 0;
  int attribute = 0;
  char * tempBuff = malloc(3);
  tempBuff[0] = buffer[*fPosition];
  tempBuff[1] = '\0';

  *fPosition = *fPosition + 1;
  
  if(CheckReservedList(tempBuff, reservedHead, &type, &attribute))
  {
    if((tempBuff[0] == ':') && (buffer[*fPosition] == '='))
    {
      tempBuff[1] = buffer[*fPosition];
      tempBuff[2] = '\0';
      *fPosition = *fPosition + 1;
      CheckReservedList(tempBuff, reservedHead, &type, &attribute);
    }
    
    AddToTokenLinked(sourceTokens,tempBuff,type, attribute);
  } else {
    *fPosition = *fPosition - 1;
  }

  *bPosition = *fPosition;

  if(buffer[*fPosition] == '\n') {
    return 0;
  } else {
    return 1;
  }
}

// moves index past relation operators if possible, else does not move index
// returns 0 if end of line reached, 1 otherwise
// if any were found they will be added to provided token linked list
int RelationalOperatorMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens)
{
  *fPosition = *bPosition;  

  int type = 0;
  int attribute = 0;
  char * tempBuff = malloc(3);


  if(buffer[*fPosition] == '<')
  {
    // check for superstring less than or equals to
    // before settling for less than

    tempBuff[0] = buffer[*fPosition];

    *fPosition = *fPosition + 1;
    
    if(buffer[*fPosition] == '=')
    {
      tempBuff[1] = buffer[*fPosition];
      *fPosition = *fPosition + 1;
      type = LTE;
    } else {
      type = LT;
    }
    
    tempBuff[*fPosition - *bPosition] = '\0';
    
  }else if(buffer[*fPosition] == '>')
  {
    // check for superstring greater than or equals to
    // before settling for greater than

    tempBuff[0] = buffer[*fPosition];  

    *fPosition = *fPosition + 1;
    
    if(buffer[*fPosition] == '=')
    {
      tempBuff[1] = buffer[*fPosition];
      *fPosition = *fPosition + 1;
    }
 
    tempBuff[*fPosition - *bPosition] = '\0';

  } else if(buffer[*fPosition] == '=') 
  {
    // check for equality operator

    tempBuff[0] = buffer[*fPosition];  

    *fPosition = *fPosition + 1;
 
    tempBuff[1] = '\0';
  }

  // if any of the above were found then the buffer 
  // position will increase
  if(*fPosition > *bPosition)
  {
    AddToTokenLinked(sourceTokens, tempBuff, type, attribute); 
  }
 
  *bPosition = *fPosition;

  if(buffer[*fPosition] == '\n') {
    return 0;
  } else {
    return 1;
  }
}

int RetrieveAnyTypeNumber(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens)
{
  *fPosition = *bPosition;  

  int type = 0;
  int attribute = 0;
  char * tempBuff = malloc(40);  

  int buffIndex = 0;

  while(isdigit(buffer[*fPosition]) != 0)
  { 
    tempBuff[buffIndex] = buffer[*fPosition];
    
    *fPosition = *fPosition + 1;
    buffIndex = *fPosition - *bPosition;
  }
  
  if((buffer[*fPosition] == '.') && (isdigit(buffer[*fPosition + 1]) != 0))
  {
    tempBuff[buffIndex] = buffer[*fPosition];
    *fPosition = *fPosition + 1;
    buffIndex = *fPosition - *bPosition;

    while(isdigit(buffer[*fPosition]) != 0)
    {
      tempBuff[buffIndex] = buffer[*fPosition];

      *fPosition = *fPosition + 1;
      buffIndex = *fPosition - *bPosition;
    }
  }

  if((buffer[*fPosition] == 'E'))
  {
    tempBuff[buffIndex] = buffer[*fPosition];
    *fPosition = *fPosition + 1;
    buffIndex = *fPosition - *bPosition;

    if((buffer[*fPosition] == '+') || (buffer[*fPosition] == '-'))
    {
      tempBuff[buffIndex] = buffer[*fPosition];

      *fPosition = *fPosition + 1;
      buffIndex = *fPosition - *bPosition;
    }
    
    while(isdigit(buffer[*fPosition]) != 0)
    {
      tempBuff[buffIndex] = buffer[*fPosition];

      *fPosition = *fPosition + 1;
      buffIndex = *fPosition - *bPosition;
    }
  }

  printf("%s\n",tempBuff);

  *bPosition = *fPosition;

  return 0;
}

int IntegerMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens)
{
  *fPosition = *bPosition;  

  int type = 0;
  int attribute = 0;
  char * tempBuff = malloc(11);

  int buffIndex = 0;

  while((isdigit(buffer[*fPosition]) != 0) && (buffIndex <= 10))
  {
    buffIndex = *fPosition - *bPosition;
  
    tempBuff[buffIndex] = buffer[*fPosition];
 
    *fPosition = *fPosition + 1; 
  }

  if((*fPosition - *bPosition) > 1 && (tempBuff[0] == '0'))
  {
    AddToTokenLinked(sourceTokens,tempBuff,LEXERR,LEADINGZERO);
  } else if((*fPosition - *bPosition) > 10)
  {
    AddToTokenLinked(sourceTokens,tempBuff,LEXERR, LONGSTRING);
  } else if((*fPosition - *bPosition) > 0)
  {
    AddToTokenLinked(sourceTokens,tempBuff,INTGR, 0);
  }

  *bPosition = *fPosition;

  if(buffer[*fPosition] == '\n') {
    return 0;
  } else {
    return 1;
  }
}

int ShortRealMachine(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens)
{
  *fPosition = *bPosition;  

  int type = 0;
  int attribute = 0;
  char * tempBuff = malloc(13);
  int buffIndex = 0;

  while((isdigit(buffer[*fPosition]) != 0) && (buffIndex <= 5))
  {
    buffIndex = *fPosition - *bPosition;
  }

  if((buffIndex > 5) && (isdigit(buffer[*fPosition]) != 0))
  {
    while(isdigit(buffer[*fPosition]) != 0)
    {
      buffIndex = *fPosition - *bPosition;
    } 
  }

  if(buffer[*fPosition] == '\n') {
    return 0;
  } else {
    return 1;
  }
}

void RetrieveReservedWords(tokenNode *rHead)
{
  FILE *reservedFile;

  // Setup a structure for linked list creation
  tokenNode reservedHead;
  tokenNode reservedForward = (tokenNode)malloc(sizeof(struct token));
  char * currentSymbol = malloc(11);
  tokenNode temp;  
 
  // Change file name here if reserved words in different file
  reservedFile = fopen("Reserved.txt","r+");

  reservedHead = reservedForward;

  // keep reading in terminals until eof is reached
  while(fgets(currentSymbol,15,reservedFile) != NULL)
  {
    reservedForward->lexeme = malloc(15);
    strcpy(reservedForward->lexeme,currentSymbol);

    fgets(currentSymbol,15,reservedFile);
    reservedForward->type = atoi(currentSymbol);

    reservedForward->attribute = (attributes)(malloc(sizeof(union attrib)));

    fgets(currentSymbol,15,reservedFile);
    reservedForward->attribute->attr = atoi(currentSymbol);

    temp = (tokenNode)(malloc(sizeof(struct token)));

    reservedForward->next = temp;
    reservedForward = temp;
  }

  // sets last nodes next value to null to signify end of linked list
  reservedForward->next = NULL;

  *rHead = reservedHead;
}

void AddToTokenLinked(tokenNode *sourceTokens, uint8_t * lexeme, uint32_t type, uint32_t attribute)
{
  tokenNode tempToken = (tokenNode)(malloc(sizeof(struct token)));
  tempToken-> attribute = (attributes)(malloc(sizeof(union attrib)));
  tempToken->lexeme = malloc(15);

  // going to add some checking to this to handle > 15
  tempToken->line = currentLine;
  strcpy(tempToken->lexeme,lexeme);
  tempToken->type = type;
  tempToken->attribute->attr = attribute;
  tempToken->next = NULL; 

  tokenNode forwardToken = *sourceTokens;

  while(forwardToken->next != NULL)
  {
    forwardToken = forwardToken->next;
  }

  forwardToken->next = tempToken;
}


// checks reserved list if lexeme exists
// if true returns 1, along with the type and attribute values
uint32_t CheckReservedList(char * lexeme, tokenNode *reservedHead, uint32_t *type, uint32_t *attribute)
{
  tokenNode forwardHead = *reservedHead;

  while(forwardHead->next != NULL)
  {

    if(strcmp(lexeme,forwardHead->lexeme)  == 0)
    {
      *type = forwardHead->type;
      *attribute = forwardHead->attribute->attr;
 
      return 1;
    }
    forwardHead = forwardHead->next;
  }

  *type = ID; 
  *attribute = 0;

  return 0;
}


