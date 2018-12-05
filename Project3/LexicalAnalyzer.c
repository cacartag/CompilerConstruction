/*
  Name: LexicalAnalyzer.c
  Engineer: Christopher Cartagena
  Description: Serves to parse file looking for tokens
  also checks basic requirements for tokens and outputs errors
  if not met
  
*/

#include "LexicalAnalyzer.h"

uint32_t currentLine = 1;

int main(int argc, char * argv[])
{
  FILE * pFile;
  uint8_t sourceLine[73];


  tokenNode reservedHead;
  sourceTokens = (tokenNode)(malloc(sizeof(struct token))); 
  sourceTokens->next = NULL;  
  
  symbolTable = (tokenNode)(malloc(sizeof(struct token)));
  symbolTable->next = NULL;
  symbolTableHead = symbolTable;
  
  RetrieveReservedWords(&reservedHead);
  
  // removes the next character line at the end of the string
  tokenNode tempReservedHead = reservedHead;
  while(tempReservedHead->next != NULL)
  {
    if (tempReservedHead->lexeme[strlen(tempReservedHead->lexeme)-1] == '\n')
    {
      // needed to change this to 1 in order not to mess up reserved word
      tempReservedHead->lexeme[strlen(tempReservedHead->lexeme)-1] = '\0';
    }

    tempReservedHead = tempReservedHead->next; 
  }

  // start by opening up file with program
  if(argc == 1)
  {
    pFile = fopen("program1","r+");
  } else {
    pFile = fopen(argv[1],"r+");
  }

  while(fgets(sourceLine,72,pFile) != NULL)
  {
    AnalyzeLine(&reservedHead, &sourceTokens, sourceLine);
    currentLine = currentLine + 1;
  }

  OutputTokens(sourceTokens);
  tokenNode tempHead = sourceTokens;
  sourceTokens = sourceTokens->next;
  parse();
  OutputListings(tempHead, pFile);  
  
  return 0;
} 

// processes buffer containing a single line of code
int AnalyzeLine(tokenNode *reservedHead, tokenNode *sourceTokens, uint8_t * buffer)
{
  // base position keeps track of starting position
  // forward position moves to keep track of current processed character
  // atEnd keeps track of if the end of the buffer has been reached
  // stuck keeps track of whether anything new has been processed
  int basePosition = 0;
  int forwardPosition = 0;
  int atEnd = 1;
  int stuck = 0;

  // while loop goes through each finite statemachine for processing
  // until the end of the buffer is reached or none of the machines can process
  // the current values.
  while((atEnd == 1) && (buffer[basePosition] != 13) && (buffer[basePosition] != 10) && (buffer[basePosition] != 0))
  {

    tokenNode headSourceTokens = *sourceTokens;

    stuck = basePosition;

    atEnd = WhiteSpaceMachine(&basePosition, &forwardPosition, buffer);
    atEnd = IdResolutionMachine(&basePosition, &forwardPosition, buffer, reservedHead, &headSourceTokens);
    atEnd = CatchAllMachine(&basePosition, &forwardPosition, buffer, reservedHead, sourceTokens);
    atEnd = RetrieveAnyTypeNumber(&basePosition, &forwardPosition, buffer, sourceTokens);
    atEnd = RelationalOperatorMachine(&basePosition, &forwardPosition, buffer, sourceTokens);
    
    // change this to catch other symbols
    if((basePosition - stuck) <= 0)
    {
      atEnd = UnidentifiedSymbol(&basePosition, buffer, sourceTokens);
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
      // check if size is greater than 10 to check for lexical error
      AddToTokenLinked(sourceTokens,id,LEXERR, LONG_STRING);
    } else {
      // check if it's a reserved word
      CheckReservedList(id, reservedHead, &type, &attribute);
      AddToTokenLinked(sourceTokens, id, type, attribute);
      
      if (type == ID)
      {
        // printf("ID is %s\n",id);
        AddToSymbolTable(&symbolTable, id, type, attribute);
      }
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
  //printf("Lexeme is: %s\n", tempBuff);
    if((tempBuff[0] == ':') && (buffer[*fPosition] == '='))
    {
      tempBuff[1] = buffer[*fPosition];
      tempBuff[2] = '\0';
      *fPosition = *fPosition + 1;
      CheckReservedList(tempBuff, reservedHead, &type, &attribute);
    }
     
    if((tempBuff[0] == '.') && (buffer[*fPosition]) == '.')
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
      type = RELOP;
      attribute = LTE;
    } else if(buffer[*fPosition] == '>') {
      tempBuff[1] = buffer[*fPosition];
      *fPosition = *fPosition + 1;
      type = RELOP;
      attribute = NE;
    } else {
      type = RELOP;
      attribute = LT;
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
      type = RELOP;
      attribute = GTE;
    } else {
      type = RELOP;
      attribute = GT;
    }
 
    tempBuff[*fPosition - *bPosition] = '\0';

  } else if(buffer[*fPosition] == '=') 
  {
    // check for equality operator
    tempBuff[0] = buffer[*fPosition];  

    *fPosition = *fPosition + 1;
 
    tempBuff[1] = '\0';
    
    type = RELOP;
    attribute = EQU;
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

// Retrieves all numbers allowed by grammar,
// this includes integers, shorts reals, and long reals
// it then tokenizes the result into linked list
int RetrieveAnyTypeNumber(int *bPosition, int *fPosition, uint8_t * buffer, tokenNode *sourceTokens)
{
  *fPosition = *bPosition;  

  int type = 0;
  int attribute = 0;
  uint8_t * tempBuff = malloc(40);  

  int buffIndex = 0;
  int numType = 0;
  int integer = 0;
  int shortReal = 0;
  int longReal = 0;

  if(isdigit(buffer[*fPosition]) != 0)
  {
    // check that the next character is a digit
    while(isdigit(buffer[*fPosition]) != 0)
    {
      integer = integer + 1;
      tempBuff[buffIndex] = buffer[*fPosition];
      
      *fPosition = *fPosition + 1;
      buffIndex = *fPosition - *bPosition;
      numType = 1;
      
      
    }
    //printf("%s\n", tempBuff);
    // end of digits
    
    // check for number after decimal
    if((buffer[*fPosition] == '.') && (isdigit(buffer[*fPosition + 1]) != 0))
    {
        shortReal = shortReal + 2;
        tempBuff[buffIndex] = buffer[*fPosition];
        *fPosition = *fPosition + 1;
        buffIndex = *fPosition - *bPosition;
      
        while(isdigit(buffer[*fPosition]) != 0)
        {
          tempBuff[buffIndex] = buffer[*fPosition];
      
          *fPosition = *fPosition + 1;
          buffIndex = *fPosition - *bPosition;
          shortReal = shortReal + 1;
        }
        
        numType =  2;
      
      //printf("%s\n", tempBuff);
      // end of short reals
      //
      //// check for numbers after exponentials
      if((buffer[*fPosition] == 'E') && ((buffer[*fPosition + 1] == '+') || (buffer[*fPosition + 1] == '-')) && (isdigit(buffer[*fPosition + 2]) != 0))
      {
        tempBuff[buffIndex] = buffer[*fPosition];
        *fPosition = *fPosition + 1;
        buffIndex = *fPosition - *bPosition;
      
        tempBuff[buffIndex] = buffer[*fPosition];
      
        *fPosition = *fPosition + 1;
        buffIndex = *fPosition - *bPosition;
        
        // look for numbers after exponential=
        while(isdigit(buffer[*fPosition]) != 0)
        {
          longReal = longReal + 1;
            
          tempBuff[buffIndex] = buffer[*fPosition];
        
          *fPosition = *fPosition + 1;
          buffIndex = *fPosition - *bPosition;
          numType = 3; 
        }
      }
        
      if((buffer[*fPosition] == 'E') && (isdigit(buffer[*fPosition + 1]) != 0))
      {
        tempBuff[buffIndex] = buffer[*fPosition];
        *fPosition = *fPosition + 1;
        buffIndex = *fPosition - *bPosition;
      
        tempBuff[buffIndex] = buffer[*fPosition];
      
        *fPosition = *fPosition + 1;
        buffIndex = *fPosition - *bPosition;
        
        // look for numbers after exponential
        while(isdigit(buffer[*fPosition]) != 0)
        {
          longReal = longReal + 1;
            
          tempBuff[buffIndex] = buffer[*fPosition];
        
          *fPosition = *fPosition + 1;
          buffIndex = *fPosition - *bPosition;
          numType = 3; 
        }          
      }
    }


    // end of long real
    
    // switch statement to handle any errors and add
    // tokens for numbers found
    switch(numType)
    {
      case 0 :
      break;
    
      case 1:
        tempBuff[integer + 1] = '\0';
        IntegerMachine(tempBuff, sourceTokens);
      break;
    
      case 2:
        tempBuff[shortReal + integer + 1] = '\0';
        ShortRealMachine(tempBuff, sourceTokens);
      break;
    
      case 3:
        tempBuff[longReal + shortReal + integer + 1] = '\0';
        LongRealMachine(tempBuff, sourceTokens);
      break;
    
      default:
      break;
    }
  }
  
  *bPosition = *fPosition;
  
  if(buffer[*fPosition] == '\n') {
    return 0;
  } else {
    return 1;
  }
}

// retrieves symbol that was unable to be processed by other machines
// and tokenizes it as unidentified,  does only one symbol at a time
int UnidentifiedSymbol(int *bPosition, uint8_t * buffer, tokenNode *sourceTokens)
{
  uint8_t * tempBuff = malloc(2);
  
  tempBuff[0] = buffer[*bPosition];
  tempBuff[1] = '\0';
  
  AddToTokenLinked(sourceTokens,tempBuff,LEXERR,UNIDENTIFIED_SYMBOL);
  
  *bPosition = *bPosition + 1;
    
  if(buffer[*bPosition] == '\n') {
    return 0;
  } else {
    return 1;
  }
}

// parses integers passed to it,
void IntegerMachine(uint8_t * tempBuff, tokenNode *sourceTokens)
{
  int type = 0;
  int attribute = 0;
  int errors = 0;
  
  // add token for leading zero error
  if(LeadingZeroCheck(tempBuff))
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

// tokenizes and parses short reals for any errors, if none found
// then value passed
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
  
  char * subbuff = malloc(beforeDecimal + 1);
  memcpy(subbuff, &tempBuff[0], beforeDecimal);
  subbuff[beforeDecimal] = '\0';

  if(LeadingZeroCheck(subbuff))
  {
    AddToTokenLinked(sourceTokens, tempBuff, LEXERR, LEADING_ZERO);
    errors = errors + 1;
  }
  
  if(TrailingZeroCheck(tempBuff[strlen(tempBuff) - 1]))
  {
    AddToTokenLinked(sourceTokens, tempBuff, LEXERR, TRAILING_ZERO);  
    errors = errors + 1;
  }
  
  if(errors == 0)
  {
    AddToTokenLinked(sourceTokens,tempBuff,REAL,VALUE);
  }
}

// tokenizes and parses long reals for any errors, if none found
// then value tokenized
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
  
  char * subbuff = malloc(afterExponent + 1);
  memcpy(subbuff, &tempBuff[index - afterExponent], afterExponent);
  subbuff[afterExponent] = '\0';
  
  if(atoi(subbuff) == 0)
  {
    AddToTokenLinked(sourceTokens, tempBuff, LEXERR, EXPONENT_IS_ZERO);
    errors = errors + 1; 
  }
  
  subbuff = malloc(beforeDecimal + 1);
  memcpy(subbuff, &tempBuff[0], beforeDecimal);
  subbuff[beforeDecimal] = '\0';
  
  if(LeadingZeroCheck(subbuff))
  {
    AddToTokenLinked(sourceTokens, tempBuff, LEXERR, LEADING_ZERO);
    errors = errors + 1;
  }
  
  
  if(TrailingZeroCheck(tempBuff[beforeDecimal+afterDecimal]))
  {
    AddToTokenLinked(sourceTokens, tempBuff, LEXERR, TRAILING_ZERO);  
    errors = errors + 1;
  }
  
  if(afterExponent == 0)
  {
    AddToTokenLinked(sourceTokens, tempBuff, LEXERR, NO_EXPONENT_SPECIFIED);
    errors = errors + 1;
  }
  
  if(errors == 0)
  {
    AddToTokenLinked(sourceTokens,tempBuff,REAL,VALUE);
  }
  
}

// retrieves reserved words from "Reserved.txt", file needs 
// to be formatted where 3 lines is equal to one reserved token
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
    //printf("Lexeme got: %s", reservedForward->lexeme);

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

// adds new tokens to linked list
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

int InSymbolTable(uint8_t * lexeme)
{    
  int found = 0;
  tokenNode tempHead = symbolTableHead;
  

  while (tempHead != NULL)
  {
    if(!strcmp(tempHead->lexeme, lexeme))
    {
      found = 1;
    }
    tempHead = tempHead->next;
  }
  
  return found;
}

// adds new tokens to linked list
void AddToSymbolTable(tokenNode *sourceTokens, uint8_t * lexeme, uint32_t type, uint32_t attribute)
{
  if(!InSymbolTable(lexeme))
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
}

// checks reserved list if lexeme exists
// if true returns 1, along with the type and attribute values
uint32_t CheckReservedList(char * lexeme, tokenNode *reservedHead, uint32_t *type, uint32_t *attribute)
{
  //if(strcmp(lexeme, "(") == 0 )
  //{
  //  printf("Lexeme is: %s\n", lexeme);     
  //}
    
  tokenNode forwardHead = *reservedHead;

  while(forwardHead->next != NULL)
  {

    //printf("Lexeme: %s Reserved: %s \n", lexeme, forwardHead->lexeme);
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

// converts macro numbers to string with description
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

  if(Number == 30) return "Lexical Error";

  if(Number == 31) return "LONG_STRING";
  if(Number == 32) return "LEADING_ZERO";
  if(Number == 33) return "TRAILING_ZERO";
  if(Number == 34) return "INT_TOO_LONG";
  if(Number == 35) return "REAL_BEFORE_DECIMAL_TOO_LONG";
  if(Number == 36) return "REAL_AFTER_DECIMAL_TOO_LONG";
  if(Number == 37) return "LONG_INT";
  if(Number == 38) return "ENDING_ZEROS";
  if(Number == 39) return "EXPONENT_TOO_LONG";
  if(Number == 40) return "UNIDENTIFIED_SYMBOL";
  if(Number == 41) return "EXPONENT_IS_ZERO";
  if(Number == 42) return "NO_EXPONENT_SPECIFIED";

  if(Number == 71) return "ADD_SYMBOL";
  if(Number == 72) return "SUB_SYMBOL";
  if(Number == 73) return "MUL_SYMBOL";
  if(Number == 74) return "DIV_SYMBOL";
  if(Number == 75) return "COLON";
  if(Number == 76) return "OPEN_CURLY_BRACKET";
  if(Number == 77) return "CLOSED_CURLY_BRACKET";
  if(Number == 78) return "PERIOD";
  if(Number == 79) return "SEMICOLON";
  if(Number == 80) return "OPEN_PARENTHESES";
  if(Number == 81) return "CLOSED_PARENTHESES";
  if(Number == 82) return "COMMA";
  if(Number == 83) return "DOUBLE_PERIOD";
  if(Number == 84) return "OPEN_BRACKET";
  if(Number == 85) return "CLOSED_BRACKET";

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
  if(Number == 200) return "EOF";
  
  if(Number == 250) return "BOOL";
  if(Number == 251) return "PGM";
  if(Number == 252) return "PGMPARAM";
  if(Number == 253) return "AINT";
  if(Number == 254) return "AREAL";
  if(Number == 255) return "ERR";
  if(Number == 256) return "PROC";
  if(Number == 257) return "PROCINT";
  if(Number == 258) return "PROCREAL";
  
  if(Number == 300) return "GREEN_NODE";
  if(Number == 301) return "BLUE_NODE";
  
  return "NO_MATCH";
}

int TrailingZeroCheck(char tempBuff)
{
    if(atoi(&tempBuff) == 0)
    {
        return 1;
    }
    
    return 0;
}

int LeadingZeroCheck(char * tempBuff)
{
    if(strlen(tempBuff) > 1 && (tempBuff[0] == '0'))
    {
        return 1;
    }
    
    return 0;
}

void OutputTokens(tokenNode tempSourceTokens)
{    
  FILE * pFile = fopen("TokenOutput.txt","w+");

  if(tempSourceTokens->next != NULL)
  {
    tempSourceTokens = tempSourceTokens->next;
  }

  fprintf(pFile,"%-5s %-15s %-23s %-10s\n", "Line","Lexeme", "Token Type", "Attribute");
  while(tempSourceTokens->next != NULL)
  {
    if(tempSourceTokens->attribute->attr == 0 && (tempSourceTokens->type != ID))
    {
      fprintf(pFile,"%-5d %-15s %-3d %-20s %-10d %s\n", tempSourceTokens->line, tempSourceTokens->lexeme, tempSourceTokens->type, NumberToString(tempSourceTokens->type), tempSourceTokens->attribute->attr, "NULL");
    } else if(tempSourceTokens->type == ID)
    {
      fprintf(pFile,"%-5d %-15s %-3d %-20s %-10X %s\n", tempSourceTokens->line, tempSourceTokens->lexeme, tempSourceTokens->type, NumberToString(tempSourceTokens->type), &(tempSourceTokens->attribute->attr), "NULL");        
    }
    else
    {
      fprintf(pFile,"%-5d %-15s %-3d %-20s %-10d %s\n", tempSourceTokens->line, tempSourceTokens->lexeme, tempSourceTokens->type, NumberToString(tempSourceTokens->type), tempSourceTokens->attribute->attr, NumberToString(tempSourceTokens->attribute->attr));        
    }
    tempSourceTokens = tempSourceTokens->next; 
    
  }

  if(tempSourceTokens->attribute->attr == 0)
  {
    fprintf(pFile,"%-5d %-15s %-3d %-20s %-10d %s\n", tempSourceTokens->line, tempSourceTokens->lexeme, tempSourceTokens->type, NumberToString(tempSourceTokens->type), tempSourceTokens->attribute->attr, "NULL");
  } else {
    fprintf(pFile,"%-5d %-15s %-3d %-20s %-10d %s\n", tempSourceTokens->line, tempSourceTokens->lexeme, tempSourceTokens->type, NumberToString(tempSourceTokens->type), tempSourceTokens->attribute->attr, NumberToString(tempSourceTokens->attribute->attr));        
  }
  
  fclose(pFile);
}

void OutputListings(tokenNode sourceTokens, FILE * pFile)
{
  char * sourceLine = malloc(73);
  //printf("second\n");
  FILE * tempFile = fopen("ListingOutput.txt","w+");
  //printf("third\n");
  int tempLine = 1;
  tokenNode tempSourceTokens = sourceTokens;
      
  rewind(pFile);
  
  //printf("first flag\n");

  while(fgets(sourceLine,72,pFile) != NULL)
  {
    fprintf(tempFile,"%i    %s",tempLine,sourceLine);
    PrintLexicalErrors(tempSourceTokens, tempLine, tempFile);
    PrintSyntaxErrors(tempLine, tempFile);
    PrintSemanticErrors(tempLine, tempFile);
    
    tempLine = tempLine + 1;
  }

}

void PrintSyntaxErrors(int tempLine, FILE * pFile)
{
  syntax tempHead = syntaxHead;
  
  while(tempHead->next != NULL)
  {
    tempHead = tempHead->next;
    
    if(tempLine == tempHead->line)
    {
      fprintf(pFile,"    %s", tempHead->syntaxErr);
    }
   
  }
}

void PrintSemanticErrors(int tempLine, FILE * pFile)
{
  semantic tempHead = semanticHead;
  
  while(tempHead->next != NULL)
  {
    tempHead = tempHead->next;
    
    if(tempLine == tempHead->line)
    {
      fprintf(pFile,"    %s", tempHead->semanticErr);
    }
   
  }
}

void PrintLexicalErrors(tokenNode tempSourceTokens, int tempLine, FILE * pFile)
{
  if(tempSourceTokens->next != NULL)
  {
    tempSourceTokens = tempSourceTokens->next;
  }

  while(tempSourceTokens->next != NULL && (tempSourceTokens->line <= tempLine) )
  {
    if((tempSourceTokens->type == LEXERR) && (tempSourceTokens->line == tempLine))
    {
      fprintf(pFile,"\n    %s: %s: %s\n",NumberToString(tempSourceTokens->type),NumberToString(tempSourceTokens->attribute->attr),tempSourceTokens->lexeme);
    }
    
    tempSourceTokens = tempSourceTokens->next;  
  }
}