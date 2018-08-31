#include "LexicalAnalyzer.h"

int main()
{
  FILE * pFile;
  uint8_t sourceLine[73];

  // start by opening up file with program
  pFile = fopen("program1","r+");

  // read single line of code

  // printf("\n\nExtracted: \n");

  // read all lines of file
  // fgets(Hello,72,pFile);
  // printf("%s", Hello);
  // AnalyzeLine(Hello);
  // while(fgets(Hello,72,pFile) != NULL)

  fgets(sourceLine,72,pFile);
  tokenNode reservedHead;
  tokenNode sourceTokens; 
  
// terminalNode reservedHead;
  
  RetrieveReservedWords(&reservedHead);
  AnalyzeLine(&reservedHead, &sourceTokens, sourceLine);

// RetrieveTerminals(&reservedHead);

  // used to check the values of the token linked list
   
 /* 

  int count = 1;

  while(reservedHead->next != NULL)
  {
    if (reservedHead->lexeme[strlen(reservedHead->lexeme)-1] == '\n')
    {
      // printf("%c, length is: %d\n", terminalHead->lexeme[2], strlen(terminalHead->lexeme));
      // printf("last character is: %c", terminalHead->lexeme[strlen(terminalHead->lexeme)-1]);
      reservedHead->lexeme[strlen(reservedHead->lexeme)-2] = '\0';
    }

    printf("%d %s %d %d \n", count, reservedHead->lexeme, reservedHead->type, reservedHead->attribute->attr);
    reservedHead = reservedHead->next; 
    count = count + 1;
  }
*/
  
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
    stuck = basePosition;

    atEnd = WhiteSpaceMachine(&basePosition, &forwardPosition, buffer);
    atEnd = IdResolutionMachine(&basePosition, &forwardPosition, buffer);
    
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
int IdResolutionMachine(int *bPosition, int *fPosition, uint8_t * buffer)
{
  char * id = malloc(13);

  int other = 1;
  *fPosition = *bPosition;

  // check if first letter is an alphabetical letter
  // printf("Current index %i\n", *fPosition);
  // printf("Analyzing %c\n", buffer[*bPosition]);
  if(isalpha(buffer[*bPosition]) == 0)
  {
    printf("Not a valid id token\n");
    other = 0;
  } else {

    // printf("Found valid Id token\n");
    id[0] = buffer[*fPosition];
    *fPosition = *fPosition + 1;
  }
  
  // go through line looking for the rest of the identifier 
  while(other == 1)
  {
   
    // check if the next value is either an alphabetical letter or a digit 
    if(isalpha(buffer[*fPosition]) != 0 || isdigit(buffer[*fPosition]) != 0 )
    {
      id[*fPosition - *bPosition] = buffer[*fPosition];
      *fPosition = *fPosition + 1;
    } else {
      other = 0;
      printf("Found token: %s\n",id);
    }

  }

  id[(*bPosition - *fPosition) + 1] = '\0';
  *bPosition = *fPosition;
 

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

// Retrieves terminal values from file, and places them into a linked list

/*
void RetrieveTerminals(terminalNode *head)
{
  FILE *terminalFile;

  // Setup a structure for linked list creation
  terminalNode terminalHead;
  terminalNode terminalForward = (terminalNode)malloc(sizeof(struct terminal));
  char * currentSymbol = malloc(8);
  terminalNode temp;  
 
  // Change file name here if terminals in different file
  terminalFile = fopen("Terminal.txt","r+");

  terminalHead = terminalForward;

  // keep reading in terminals until eof is reached
  while(fgets(currentSymbol,7,terminalFile) != NULL)
  {
    terminalForward->symbol = malloc(8);
    strcpy(terminalForward->symbol,currentSymbol);
    temp = (terminalNode)(malloc(sizeof(struct terminal)));

    terminalForward->next = temp; 
    terminalForward = temp;
  }

  // sets last nodes next value to null to signify end of linked list
  terminalForward->next = NULL;

  *head = terminalHead;
  
}
*/


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

//    printf("type %d\n", reservedForward->type);

    reservedForward->attribute = (attributes)(malloc(sizeof(union attrib)));

    fgets(currentSymbol,15,reservedFile);
    reservedForward->attribute->attr = atoi(currentSymbol);

//    printf("attribute %d \n",reservedForward->attribute->attr);

    temp = (tokenNode)(malloc(sizeof(struct token)));

    reservedForward->next = temp;
    reservedForward = temp;
  }

  // sets last nodes next value to null to signify end of linked list
  reservedForward->next = NULL;

  *rHead = reservedHead;
}
