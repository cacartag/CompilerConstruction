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

  // removes the next character line at the end of the string
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


 while(fgets(sourceLine,72,pFile) != NULL)
 {
   //printf("%s",sourceLine);
   //fgets(sourceLine,72,pFile);
   AnalyzeLine(&reservedHead, &sourceTokens, sourceLine);
   currentLine = currentLine + 1;
 }



  //fgets(sourceLine,72,pFile);
  //AnalyzeLine(&reservedHead, &sourceTokens, sourceLine);

  if(sourceTokens->next != NULL)
  {
    sourceTokens = sourceTokens->next;
  }

  while(sourceTokens->next != NULL)
  {
    if(sourceTokens->attribute->attr == 0)
    {
      printf("%d %s %d (%s) %d\n", sourceTokens->line, sourceTokens->lexeme, sourceTokens->type, NumberToString(sourceTokens->type), sourceTokens->attribute->attr);
    } else {
      printf("%d %s %d (%s) %d (%s)\n", sourceTokens->line, sourceTokens->lexeme, sourceTokens->type, NumberToString(sourceTokens->type), sourceTokens->attribute->attr, NumberToString(sourceTokens->attribute->attr));        
    }
    sourceTokens = sourceTokens->next; 
  }

  if(sourceTokens->attribute->attr == 0)
  {
    printf("%d %s %d (%s) %d\n", sourceTokens->line, sourceTokens->lexeme, sourceTokens->type, NumberToString(sourceTokens->type), sourceTokens->attribute->attr);
  } else {
    printf("%d %s %d (%s) %d (%s)\n", sourceTokens->line, sourceTokens->lexeme, sourceTokens->type, NumberToString(sourceTokens->type), sourceTokens->attribute->attr, NumberToString(sourceTokens->attribute->attr));        
  }
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
    atEnd = CatchAllMachine(&basePosition, &forwardPosition, buffer, reservedHead, sourceTokens);
    atEnd = RetrieveAnyTypeNumber(&basePosition, &forwardPosition, buffer, sourceTokens);
    atEnd = RelationalOperatorMachine(&basePosition, &forwardPosition, buffer, sourceTokens);
    
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
      AddToTokenLinked(sourceTokens,id,LEXERR, LONG_STRING);
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
  uint8_t * tempBuff = malloc(40);  

  int buffIndex = 0;
  int numType = 0;

  // check that the next character is a digit
  while(isdigit(buffer[*fPosition]) != 0)
  { 
    tempBuff[buffIndex] = buffer[*fPosition];
    
    *fPosition = *fPosition + 1;
    buffIndex = *fPosition - *bPosition;
    numType = 1;
  }
  
  // check for number after decimal
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
    numType = numType + 1;
  }

  // check for numbers after exponentials
  if((buffer[*fPosition] == 'E'))
  {
    tempBuff[buffIndex] = buffer[*fPosition];
    *fPosition = *fPosition + 1;
    buffIndex = *fPosition - *bPosition;

    // check if there is a plus or a minus after 
    if((buffer[*fPosition] == '+') || (buffer[*fPosition] == '-'))
    {
      tempBuff[buffIndex] = buffer[*fPosition];

      *fPosition = *fPosition + 1;
      buffIndex = *fPosition - *bPosition;
    }
    
    // look for numbers after exponential
    while(isdigit(buffer[*fPosition]) != 0)
    {
      tempBuff[buffIndex] = buffer[*fPosition];

      *fPosition = *fPosition + 1;
      buffIndex = *fPosition - *bPosition;
    }
  
    numType = numType + 1;
  }


  //printf("%s\n",tempBuff);

  // switch statement to handle any errors and add
  // tokens for numbers found
  switch(numType)
  {
    case 0 :
    break;

    case 1:
      IntegerMachine(tempBuff, sourceTokens);
    break;

    case 2:
      ShortRealMachine(tempBuff, sourceTokens);
    break;

    case 3:
      LongRealMachine(tempBuff, sourceTokens);
    break;

    default:
    break;
  }

  *bPosition = *fPosition;
  
  if(buffer[*fPosition] == '\n') {
    return 0;
  } else {
    return 1;
  }
}

void IntegerMachine(uint8_t * tempBuff, tokenNode *sourceTokens)
{
  int type = 0;
  int attribute = 0;
  int errors = 0;
  
  // add token for leading zero error
  if(strlen(tempBuff) > 1 && (tempBuff[0] == '0'))
  {
    AddToTokenLinked(sourceTokens,tempBuff,LEXERR,LEADING_ZERO);
    errors = errors + 1;
  }

  // add token for too long of an integer
  if(strlen(tempBuff) > 10)
  {
    AddToTokenLinked(sourceTokens,tempBuff,LEXERR, LONG_INT);
    errors = errors + 1;
  }
  
  // add integer if no errors
  if(errors == 0)
  {
    AddToTokenLinked(sourceTokens,tempBuff,INTEGER, VALUE);
  }
  
}

void ShortRealMachine(uint8_t * tempBuff, tokenNode *sourceTokens)
{
  int type = 0;
  int attribute = 0;
  
  int index = 0;
  int beforeDecimal = 0;
  int afterDecimal = 0;
  int errors = 0;

  while(isdigit(tempBuff[index]) != 0)
  {
    index = index + 1;
    beforeDecimal = beforeDecimal + 1;
  }
  
  index = index + 1;
  
  while(isdigit(tempBuff[index]) != 0)
  {
    index = index + 1;
    afterDecimal = afterDecimal + 1;
  }

  if(beforeDecimal > 5)
  {
    AddToTokenLinked(sourceTokens,tempBuff,LEXERR,REAL_BEFORE_DECIMAL_TOO_LONG);
    errors = errors + 1;
  }
  
  if(afterDecimal > 5)
  {
    AddToTokenLinked(sourceTokens,tempBuff,LEXERR,REAL_AFTER_DECIMAL_TOO_LONG);
    errors = errors + 1;
  }
  
  if(errors == 0)
  {
    AddToTokenLinked(sourceTokens,tempBuff,REAL,VALUE);
  }
}

void LongRealMachine(uint8_t * tempBuff, tokenNode *sourceTokens)
{
  int index = 0;
  int beforeDecimal = 0;
  int afterDecimal = 0;
  int afterExponent = 0;
  int errors = 0;

  while(isdigit(tempBuff[index]) != 0)
  {
    index = index + 1;
    beforeDecimal = beforeDecimal + 1;
  }
  
  index = index + 1;
  
  while(isdigit(tempBuff[index]) != 0)
  {
    index = index + 1;
    afterDecimal = afterDecimal + 1;
  }
  
  index = index + 1;
  
  if(tempBuff[index] == '+' || tempBuff[index] == '-')
  {
    index = index + 1;
  }
  
  while(isdigit(tempBuff[index]) != 0)
  {
    index = index + 1;
    afterExponent = afterExponent + 1;
  }
  
  if(beforeDecimal > 5)
  {
    AddToTokenLinked(sourceTokens,tempBuff,LEXERR,REAL_BEFORE_DECIMAL_TOO_LONG);
    errors = errors + 1;
  }
  
  if(afterDecimal > 5)
  {
    AddToTokenLinked(sourceTokens,tempBuff,LEXERR,REAL_AFTER_DECIMAL_TOO_LONG);
    errors = errors + 1;
  }
  
  if(afterExponent > 2)
  {
    AddToTokenLinked(sourceTokens, tempBuff, LEXERR, EXPONENT_TOO_LONG);
    errors = errors + 1;
  }
  
  if(errors == 0)
  {
    AddToTokenLinked(sourceTokens,tempBuff,REAL,VALUE);
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

char * NumberToString(int Number)
{
  if(Number == 0) return "NULL";
  if(Number == 1) return "IF";
  if(Number == 2) return "THEN"; 
  if(Number == 3) return "PROGRAM";
  if(Number == 4) return "VAR";
  if(Number == 5) return "ARRAY";
  if(Number == 6) return "OF";
  if(Number == 7) return "INT";
  if(Number == 8) return "REAL";
  if(Number == 9) return "PROCEDURE";
  if(Number == 10) return "BEGIN";
  if(Number == 11) return "END";
  if(Number == 12) return "ELSE";
  if(Number == 13) return "WHILE";
  if(Number == 14) return "DO";
  if(Number == 15) return "OR";
  if(Number == 16) return "DIV";
  if(Number == 17) return "MOD";
  if(Number == 18) return "AND";
  if(Number == 19) return "NOT";
  if(Number == 20) return "ID";
  if(Number == 21) return "CALL";
  if(Number == 22) return "VALUE";

  if(Number == 30) return "LEXERR";

  if(Number == 31) return "LONG_STRING";
  if(Number == 32) return "LEADING_ZERO";
  if(Number == 33) return "TRAILING_ZERO";
  if(Number == 34) return "INT_TOO_LONG";
  if(Number == 35) return "REAL_BEFORE_DECIMAL_TOO_LONG";
  if(Number == 36) return "REAL_AFTER_DECIMAL_TOO_LONG";
  if(Number == 37) return "LONG_INT";
  if(Number == 38) return "ENDING_ZEROS";
  if(Number == 39) return "EXPONENT_TOO_LONG";

  if(Number == 71) return "ADD_SYMBOL";
  if(Number == 72) return "SUB_SYMBOL";
  if(Number == 73) return "MUL_SYMBOL";
  if(Number == 74) return "DIV_SYMBOL";
  if(Number == 75) return "COLON";
  if(Number == 76) return "OPEN_BRACKET";
  if(Number == 77) return "CLOSED_BRACKET";
  if(Number == 78) return "PERIOD";
  if(Number == 79) return "SEMICOLON";
  if(Number == 80) return "OPEN_PARENTHESES";
  if(Number == 81) return "CLOSED_PARENTHESES";
  if(Number == 82) return "COMMA";
  if(Number == 83) return "DOUBLE_PERIOD";

  if(Number == 120) return "GT";
  if(Number == 121) return "LT";
  if(Number == 122) return "GTE";
  if(Number == 123) return "LTE";
  if(Number == 124) return "EQU";
  if(Number == 125) return "NE";

  if(Number == 160) return "RELOP";
  if(Number == 161) return "ADDOP";
  if(Number == 162) return "MULOP";
  if(Number == 163) return "ASSIGNOP";
}
