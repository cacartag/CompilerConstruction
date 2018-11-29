#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "LexicalAnalyzer.h"

tokenNode tok;

tokenNode getToken(){
  tokenNode tok;

  if(sourceTokens != NULL)
  {
    tok = sourceTokens;
    sourceTokens = sourceTokens->next;
  } else {
    tok = sourceTokens;
  }
  
  //printf("retrieved %s\n", tok->lexeme);

  return tok;
}


void prgrm()
{
 switch( tok->type )
  {

    case 3: // terminal is prgrm
      match("prgrm");
      checkAddGreenNode(tok->lexeme, PGM);
      match("id");
      match("(");
      id_list();
      match(")");
      match(";");
      prgrmp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of prgrm $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {3,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void prgrmp()
{
 switch( tok->type )
  {

    case 4: // terminal is var
      decls();
      prgrmpp();
    break;

    case 9: // terminal is procdr
      subprgrm_decls();
      cmpnd_stmnt();
      match(".");
    break;

    case 10: // terminal is begin
      cmpnd_stmnt();
      match(".");
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of var procdr begin $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {4,9,10,200};

      while( checkSynch(synchSet, tok->type, 4) )
      {
        tok = getToken();
      }
  }
}

void prgrmpp()
{
 switch( tok->type )
  {

    case 9: // terminal is procdr
      subprgrm_decls();
      cmpnd_stmnt();
      match(".");
    break;

    case 10: // terminal is begin
      cmpnd_stmnt();
      match(".");
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of procdr begin $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {9,10,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void id_list()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      checkAddBlueNode(tok->lexeme,PGMPARAM);
      match("id");
      id_listp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void id_listp()
{
 switch( tok->type )
  {

    case 82: // terminal is ,
      match(",");
      checkAddBlueNode(tok->lexeme,PGMPARAM);
      match("id");
      id_listp();
    break;

    case 81 : // terminal is ), epsilon do nothing
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ) , $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {81,82,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void decls()
{
 switch( tok->type )
  {

    case 4: // terminal is var
      match("var");
      char * idTemp = tok->lexeme;
      printf("%s\n",idTemp);
      match("id");
      match(":");
      checkAddBlueNode(idTemp,type());
      match(";");
      declsp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of var $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {4,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void declsp()
{
 switch( tok->type )
  {

    case 4: // terminal is var
      match("var");
      char * idTemp = tok->lexeme;
      printf("%s\n",idTemp);
      match("id");
      match(":");
      checkAddBlueNode(idTemp,type());
      match(";");
      declsp();
    break;

    case 9 : // terminal is procdr, epsilon do nothing
    break;

    case 10 : // terminal is begin, epsilon do nothing
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of var procdr begin $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {4,9,10,200};

      while( checkSynch(synchSet, tok->type, 4) )
      {
        tok = getToken();
      }
  }
}

int type()
{
 switch( tok->type )
  {

    case 5: // terminal is arr
      match("arr");
      match("[");
      match("num");
      match("..");
      match("num");
      match("]");
      match("of");
      int tempType = stndrd_type();
      if(tempType == ERR)
      {
          return ERR;
      } else if (tempType == INTEGER)
      {
          return AINT;
      } else if (tempType == REAL)
      {
          return AREAL;
      }
    break;

    case 7: // terminal is int
      return stndrd_type();
    break;

    case 8: // terminal is real
      return stndrd_type();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of arr int real $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {5,7,8,200};

      while( checkSynch(synchSet, tok->type, 4) )
      {
        tok = getToken();
      }
  }
}

int stndrd_type()
{
 switch( tok->type )
  {

    case 7: // terminal is int
      match("int");
      return INTEGER;
    break;

    case 8: // terminal is real
      match("real");
      return REAL;
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of int real $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {7,8,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void subprgrm_decls()
{
 switch( tok->type )
  {

    case 9: // terminal is procdr
      subprgrm_decl();
      match(";");
      subprgrm_declsp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of procdr $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {9,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void subprgrm_declsp()
{
 switch( tok->type )
  {

    case 9: // terminal is procdr
      subprgrm_decl();
      match(";");
      subprgrm_declsp();
    break;

    case 10 : // terminal is begin, epsilon do nothing
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of procdr begin $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {9,10,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void subprgrm_decl()
{
 switch( tok->type )
  {

    case 9: // terminal is procdr
      subprgrm_head();
      subprgrm_declp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of procdr $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {9,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void subprgrm_declp()
{
 switch( tok->type )
  {

    case 4: // terminal is var
      decls();
      subprgrm_declpp();
    break;

    case 9: // terminal is procdr
      subprgrm_decls();
      cmpnd_stmnt();
    break;

    case 10: // terminal is begin
      cmpnd_stmnt();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of var procdr begin $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {4,9,10,200};

      while( checkSynch(synchSet, tok->type, 4) )
      {
        tok = getToken();
      }
  }
}

void subprgrm_declpp()
{
 switch( tok->type )
  {

    case 9: // terminal is procdr
      subprgrm_decls();
      cmpnd_stmnt();
    break;

    case 10: // terminal is begin
      cmpnd_stmnt();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of procdr begin $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {9,10,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void subprgrm_head()
{
 switch( tok->type )
  {

    case 9: // terminal is procdr
      match("procdr");
      checkAddGreenNode(tok->lexeme, PROC);
      match("id");
      subprgrm_headp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of procdr $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {9,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void subprgrm_headp()
{
 switch( tok->type )
  {

    case 80: // terminal is (
      args();
      match(";");
    break;

    case 79: // terminal is ;
      match(";");
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ( ; $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {80,79,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void args()
{
 switch( tok->type )
  {

    case 80: // terminal is (
      match("(");
      param_lst();
      match(")");
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ( $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {80,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void param_lst()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      ;
      char * idTemp = tok->lexeme;
      match("id");
      match(":");
      checkAddBlueNode(idTemp,type());
      param_lstp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void param_lstp()
{
 switch( tok->type )
  {

    case 79: // terminal is ;
      match(";");
      char * idTemp = tok->lexeme;
      match("id");
      match(":");
      checkAddBlueNode(idTemp,type());
      param_lstp();
    break;

    case 81 : // terminal is ), epsilon do nothing
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ) ; $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {81,79,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void cmpnd_stmnt()
{
 switch( tok->type )
  {

    case 10: // terminal is begin
      //printf("reached compound statement\n");
      match("begin");
      cmpnd_stmntp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of begin $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {10,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void cmpnd_stmntp()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      opt_stmnt();
      match("end");
    break;

    case 10: // terminal is begin
      opt_stmnt();
      match("end");
    break;

    case 11: // terminal is end
      match("end");
    break;

    case 1: // terminal is if
      opt_stmnt();
      match("end");
    break;

    case 13: // terminal is while
      opt_stmnt();
      match("end");
    break;

    case 21: // terminal is call
      opt_stmnt();
      match("end");
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id begin end if while call $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,10,11,1,13,21,200};

      while( checkSynch(synchSet, tok->type, 7) )
      {
        tok = getToken();
      }
  }
}

void opt_stmnt()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      stmnt_lst();
    break;

    case 10: // terminal is begin
      stmnt_lst();
    break;

    case 1: // terminal is if
      stmnt_lst();
    break;

    case 13: // terminal is while
      stmnt_lst();
    break;

    case 21: // terminal is call
      stmnt_lst();
    break;

    default:
      ;
      printf("reached inside of opt_stmnt\n");
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id begin if while call $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,10,1,13,21,200};

      while( checkSynch(synchSet, tok->type, 6) )
      {
        tok = getToken();
      }
  }
}

void stmnt_lst()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      stmnt();
      stmnt_lstp();
    break;

    case 10: // terminal is begin
      stmnt();
      stmnt_lstp();
    break;

    case 1: // terminal is if
      stmnt();
      stmnt_lstp();
    break;

    case 13: // terminal is while
      stmnt();
      stmnt_lstp();
    break;

    case 21: // terminal is call
      stmnt();
      stmnt_lstp();
    break;

    default:
      ;
      printf("Inside statement list\n");
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id begin if while call $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,10,1,13,21,200};

      while( checkSynch(synchSet, tok->type, 6) )
      {
        tok = getToken();
      }
  }
}

void stmnt_lstp()
{
 switch( tok->type )
  {

    case 79: // terminal is ;
      match(";");
      stmnt();
      stmnt_lstp();
    break;

    case 11 : // terminal is end, epsilon do nothing
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ; end $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {79,11,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void stmnt()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      //printf("%s\n", tok->lexeme);
      //printf("inside terminal id\n");
      variable();
      match("assignop");
      //printf("%s\n", tok->lexeme);
      express();
    break;

    case 10: // terminal is begin
      //printf("first\n");
      cmpnd_stmnt();
    break;

    case 1: // terminal is if
      //printf("Reached inside statement\n");
      match("if");
      //printf("processed if \n");
      express();
      //printf("processed express\n");
      //printf("current lexeme %s\n",tok->lexeme);
      //printf("%s\n",tok->lexeme);
      //printf("before matching then\n");
      match("then");
      //printf("processed then\n");
      //printf("before processing then statement\n");
      stmnt();
      stmntp();
    break;

    case 13: // terminal is while
      //printf("third\n");
      match("while");
      express();
      match("do");
      stmnt();
    break;

    case 21: // terminal is call
      //printf("second\n");
      procdr_stmnt();
    break;

    default:
      ;
      //printf("did not match any statement\n");
      //printf("%s\n",tok->lexeme);
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id begin if while call $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,10,1,13,21,200};

      while( checkSynch(synchSet, tok->type, 6) )
      {
        tok = getToken();
      }
  }
}

void stmntp()
{
  switch( tok->type )
  {
    case 12: // terminal is else
      match("else");
      stmnt();
    break; 
    
    case 79:
    break;
    
    case 11:
    break;
   
    default:
    
    ;
    
  }
}

void variable()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      match("id");
      //printf("variable: %s\n",tok->lexeme);
      variablep();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void variablep()
{
 switch( tok->type )
  {

    case 84: // terminal is [
      match("[");
      express();
      match("]");
    break;

    case 163 : // terminal is assignop, epsilon do nothing
    //printf("broke for assignop\n");
    //printf("%s\n", tok->lexeme);
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of [ assignop $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {84,163,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void procdr_stmnt()
{
 switch( tok->type )
  {

    case 21: // terminal is call
      match("call");
      match("id");
      procdr_stmntp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of call $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {21,200};

      while( checkSynch(synchSet, tok->type, 2) )
      {
        tok = getToken();
      }
  }
}

void procdr_stmntp()
{
 switch( tok->type )
  {

    case 80: // terminal is (
      match("(");
      express_lst();
      match(")");
    break;

    case 78 : // terminal is ., epsilon do nothing
    break;

    case 12 : // terminal is else, epsilon do nothing
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ( . else $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {80,78,12,200};

      while( checkSynch(synchSet, tok->type, 4) )
      {
        tok = getToken();
      }
  }
}

void express_lst()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      express();
      express_lstp();
    break;

    case 80: // terminal is (
      express();
      express_lstp();
    break;

    case INTEGER: // terminal is num, changing to INTEGER
      express();
      express_lstp();
    break;

    case 19: // terminal is not
      express();
      express_lstp();
    break;

    case 161: // terminal is +
      express();
      express_lstp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id ( num not + - $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,80,23,19,161,161,200};

      while( checkSynch(synchSet, tok->type, 7) )
      {
        tok = getToken();
      }
  }
}

void express_lstp()
{
 switch( tok->type )
  {

    case 82: // terminal is ,
      match(",");
      express();
      express_lst();
    break;

    case 81 : // terminal is ), epsilon do nothing
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ) , $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {81,82,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void express()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      simp_express();
      expressp();
    break;

    case 80: // terminal is (
      simp_express();
      expressp();
    break;

    case INTEGER: // terminal is num, changing to INTEGER
      //printf("reached in num\n");
      simp_express();
      //printf("after simp_express, %s\n",tok->lexeme);
      expressp();
    break;

    case 19: // terminal is not
      simp_express();
      expressp();
    break;

    case 161: // terminal is +
      simp_express();
      expressp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id ( num not + - $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,80,23,19,161,161,200};

      while( checkSynch(synchSet, tok->type, 7) )
      {

        tok = getToken();
      }
  }
}

void expressp()
{
 switch( tok->type )
  {

    case 160: // terminal is relop
      //printf("evaluating for relop\n");
      //printf("current lexeme is %s\n", tok->lexeme);
      match("relop");
      //printf("current lexeme is %s\n", tok->lexeme);
      simp_express();
    break;

    case 81 : // terminal is ), epsilon do nothing
    break;

    case 79 : // terminal is ;, epsilon do nothing
    break;

    case 82 : // terminal is ,, epsilon do nothing
    break;

    case 85 : // terminal is ], epsilon do nothing
    break;

    case 11 : // terminal is end, epsilon do nothing
    break;

    case 2 : // terminal is then, epsilon do nothing
    break;

    case 14 : // terminal is do, epsilon do nothing
    break;
    
    case 12 : // terminal is else, epsilon do nothing
    //printf("broke for else\n");
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ) ; , ] end then do relop $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {81,79,82,85,11,2,14,160,200};

      while( checkSynch(synchSet, tok->type, 9) )
      {
        tok = getToken();
      }
  }
}

void simp_express()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      term();
      simp_expressp();
    break;

    case 80: // terminal is (
      term();
      simp_expressp();
    break;

    case INTEGER: // terminal is num, changing to INTEGER
      //printf("inside simp_express\n");
      term();
      //printf("after term, %s\n",tok->lexeme);
      simp_expressp();
    break;

    case 19: // terminal is not
      term();
      simp_expressp();
    break;

    case 161: // terminal is +
      sgn();
      term();
      simp_expressp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id ( num not + - $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,80,23,19,161,161,200};

      while( checkSynch(synchSet, tok->type, 7) )
      {
        tok = getToken();
      }
  }
}

void simp_expressp()
{
 switch( tok->type )
  {

    case 161: // terminal is addop
      match("addop");
      term();
      simp_expressp();
    break;

    case 81 : // terminal is ), epsilon do nothing
    break;

    case 79 : // terminal is ;, epsilon do nothing
    break;

    case 82 : // terminal is ,, epsilon do nothing
    break;

    case 85 : // terminal is ], epsilon do nothing
    break;

    case 11 : // terminal is end, epsilon do nothing
    break;

    case 2 : // terminal is then, epsilon do nothing
    break;

    case 12 : // terminal is else, epsilon do nothing
    //printf("broke for else\n");
    break;

    case 14 : // terminal is do, epsilon do nothing
    break;

    case 160 : // terminal is relop, epsilon do nothing
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ) ; , ] end then else do relop addop $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {81,79,82,85,11,2,12,14,160,161,200};

      while( checkSynch(synchSet, tok->type, 11) )
      {
        tok = getToken();
      }
  }
}

void term()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      factor();
      termp();
    break;

    case 80: // terminal is (
      factor();
      termp();
    break;

    case INTEGER: // terminal is num, changing to INTEGER
      //printf("inside term\n");
      factor();
      //printf("after factor, %s\n", tok->lexeme);
      termp();
    break;

    case 19: // terminal is not
      factor();
      termp();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id ( num not $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,80,23,19,200};

      while( checkSynch(synchSet, tok->type, 5) )
      {
        tok = getToken();
      }
  }
}

void termp()
{
 switch( tok->type )
  {

    case 162: // terminal is mulop
      match("mulop");
      factor();
      termp();
    break;

    case 81 : // terminal is ), epsilon do nothing
    break;

    case 79 : // terminal is ;, epsilon do nothing
    break;

    case 82 : // terminal is ,, epsilon do nothing
    break;

    case 85 : // terminal is ], epsilon do nothing
    break;

    case 11 : // terminal is end, epsilon do nothing
    break;

    case 2 : // terminal is then, epsilon do nothing
    break;

    case 12 : // terminal is else, epsilon do nothing
    //printf("broke for else\n");
    break;

    case 14 : // terminal is do, epsilon do nothing
    break;

    case 160 : // terminal is relop, epsilon do nothing
    break;

    case 161 : // terminal is addop, epsilon do nothing
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ) ; , ] end then else do relop addop mulop $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {81,79,82,85,11,2,12,14,160,161,162,200};

      while( checkSynch(synchSet, tok->type, 12) )
      {
        tok = getToken();
      }
  }
}

void factor()
{
 switch( tok->type )
  {

    case 20: // terminal is id
      match("id");
      factorp();
    break;

    case 80: // terminal is (
      match("(");
      express();
      match(")");
    break;

    case INTEGER: // terminal is num, changing to int
      match("num");
      //printf("%s\n", tok->lexeme);
    break;

    case 19: // terminal is not
      match("not");
      factor();
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of id ( num not $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {20,80,23,19,200};

      while( checkSynch(synchSet, tok->type, 5) )
      {
        tok = getToken();
      }
  }
}

void factorp()
{
 switch( tok->type )
  {

    case 84: // terminal is [
      match("[");
      express();
      match("]");
    break;

    case 81 : // terminal is ), epsilon do nothing
    break;

    case 79 : // terminal is ;, epsilon do nothing
    break;

    case 82 : // terminal is ,, epsilon do nothing
    break;

    case 85 : // terminal is ], epsilon do nothing
    break;

    case 11 : // terminal is end, epsilon do nothing
    break;

    case 163 : // terminal is assignop, epsilon do nothing
    break;

    case 2 : // terminal is then, epsilon do nothing
    break;

    case 12 : // terminal is else, epsilon do nothing
    break;

    case 14 : // terminal is do, epsilon do nothing
    break;

    case 160 : // terminal is relop, epsilon do nothing
    break;

    case 161 : // terminal is addop, epsilon do nothing
    break;

    case 162 : // terminal is mulop, epsilon do nothing
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of ) ; , [ ] end assignop then else do relop addop mulop $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {81,79,82,84,85,11,163,2,12,14,160,161,162,200};

      while( checkSynch(synchSet, tok->type, 14) )
      {
        tok = getToken();
      }
  }
}

void sgn()
{
 switch( tok->attribute->attr )
  {

    case 71: //terminal is +
      match("+");
    break;

    case 72: //terminal is -
      match("-");
    break;

    default:
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting one of + - $, Received: %s\n", tok->lexeme);
      listingPrintf(synTempStr);

      int synchSet[] = {161,161,200};

      while( checkSynch(synchSet, tok->type, 3) )
      {
        tok = getToken();
      }
  }
}

void match(const char * t)
{
    
  if(!strcmp(t,"id"))
  {
    if(tok->type == ID)
    {
      tok = getToken();
    }
    else
    {
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting %s, Received %s \n", t, tok->lexeme);
      listingPrintf(synTempStr);
    }
  }
  else if(!strcmp(t,"num"))
  {
    if(tok->type == INTEGER)
    {
      tok = getToken();
    }
    else
    {
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting %s, Received %s \n", t, tok->lexeme);
      listingPrintf(synTempStr);
    }
  }
  else if (!strcmp(t,"assignop") || !strcmp(t,"relop") || !strcmp(t,"addop") || !strcmp(t,"mulop"))
  {
    if(!strcmp(t,"assignop") && (tok->type == ASSIGNOP))
    {
      tok = getToken();
    } else if(!strcmp(t,"relop") && (tok->type == RELOP))
    {
      tok = getToken();
    } else if(!strcmp(t,"addop") && (tok->type == ADDOP))
    {
      tok = getToken();
    } else if(!strcmp(t,"mulop") && (tok->type == MULOP))
    {
      tok = getToken();
    } else {
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting %s, Received %s \n", t, tok->lexeme);
      listingPrintf(synTempStr);
    }

  }
  else if ( !( strcmp((const char *)(tok->lexeme), t) ) && ( strcmp(t, "$\0") ) )
  {
    tok = getToken();
  }
  else if ( !( strcmp((const char *)(tok->lexeme), t) ) && !( strcmp(t, "$\0") ) )
  {
      ;
      char synTempStr[100];
      sprintf(synTempStr, "Successfully parsed !\n ");
      listingPrintf(synTempStr);
      
  }
  else if ( strcmp((const char *)(tok->lexeme), t) )
  {
      ;
      char synTempStr[100];
      sprintf(synTempStr,"Syntax Error: Expecting %s, Received %s \n", t, tok->lexeme);
      listingPrintf(synTempStr);
      tok = getToken();
  }
}

// returns 0 if found in synchset
int checkSynch(int * synchSet, int tokenType, int length)
{
    int found = 1;

    for(int index = 0; index < length; index++)
    {
      if(synchSet[index] == tokenType)
      {
          found = 0;
      }
    }

    return found;
}

void parse()
{
    syntaxErrors = (syntax)malloc(sizeof(struct syntaxError));
    syntaxHead = syntaxErrors;
    
    initializeInfrastructure();
    
    tok = getToken();
    prgrm();
    match("$");

    syntax tempSyntax = syntaxHead;
    
     printInfrastructure();
    //while(tempSyntax->next != NULL)
    //{
    //  tempSyntax = tempSyntax->next;
    //  printf("%i %s %s",tempSyntax->line, tempSyntax->lexemeTest, tempSyntax->syntaxErr);
    //}

}

void listingPrintf(char * synTempStr)
{
    syntax tempSyntax = (syntax)malloc(sizeof(struct syntaxError));
    tempSyntax->line = tok->line;
    tempSyntax->lexemeTest = malloc(strlen(tok->lexeme));
    tempSyntax->syntaxErr = malloc(100);
    strcpy(tempSyntax->lexemeTest, tok->lexeme);
    strcpy(tempSyntax->syntaxErr, synTempStr);
    tempSyntax->next = NULL;
    
    syntaxErrors->next = tempSyntax;
    syntaxErrors = syntaxErrors->next;
}

