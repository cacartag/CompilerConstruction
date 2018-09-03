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

   fgets(sourceLine,72,pFile);
   AnalyzeLine(&reservedHead, &sourceTokens, sourceLine);

/*
   while(fgets(sourceLine,72,pFile) != NULL)
   {
     //printf("%s",sourceLine);
     AnalyzeLine(&reservedHead, &sourceTokens, sourceLine);
     printf("\n");
   }
*/

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

    atEnd = WhiteSpaceMachine(&basePosition, &forwardPosition, buffer);
    atEnd = IdResolutionMachine(&basePosition, &forwardPosition, buffer, reservedHead, &headSourceTokens);
   
    if((basePosition - stuck) <= 0)
    {
      stuck = 1;
    } else {
      stuck = 0;
    }
  }


//  printf("Tokens:\n");
 
  int count = 0;


  *sourceTokens = (*sourceTokens)->next;

  while((*sourceTokens)->next != NULL)
  {
    printf("%d %s %d %d\n", count, (*sourceTokens)->lexeme, (*sourceTokens)->type, (*sourceTokens)->attribute->attr);
    *sourceTokens = (*sourceTokens)->next; 
    count = count + 1;
  }

  printf("%d %s %d %d\n", count, (*sourceTokens)->lexeme, (*sourceTokens)->type, (*sourceTokens)->attribute->attr);

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

    CheckReservedList(id, reservedHead, &type, &attribute);
    AddToTokenLinked(sourceTokens, id, type, attribute);

    if((*fPosition - *bPosition) > 10)
    {
      AddToTokenLinked(sourceTokens,"Id Too Long",LEXERR, LONGSTRING);
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
int CatchAll(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *reservedHead)
{
  int other = 1;
  *fPosition = *bPosition;  

  char * current = malloc(5);
  
  while(other == 1)
  {
    
    tokenNode reservedForward = *reservedHead; 
    while(reservedForward->next != NULL)
    {

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
  strcpy(tempToken->lexeme,lexeme);
  tempToken->type = type;
  tempToken->attribute->attr = attribute;
  tempToken->next = NULL; 

  while((*sourceTokens)->next != NULL)
  {
    *sourceTokens = (*sourceTokens)->next;
  }

  (*sourceTokens)->next = tempToken;
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

  *type = 0;
  *attribute = 0;

  return 0;
  
}


