#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct token
{
  uint32_t line;
  uint8_t * lexeme;
  uint32_t type;
  union attrib * attribute;
  struct token * next;
};

typedef struct token *tok;

void prgrm();
void prgrmp();
void prgrmpp();
void id_list();
void id_listp();
void decls();
void declsp();
void type();
void stndrd_type();
void subprgrm_decls();
void subprgrm_declsp();
void subprgrm_decl();
void subprgrm_declp();
void subprgrm_declpp();
void subprgrm_head();
void subprgrm_headp();
void args();
void param_lst();
void param_lstp();
void cmpnd_stmnt();
void cmpnd_stmntp();
void opt_stmnt();
void stmnt_lst();
void stmnt_lstp();
void stmnt();
void variable();
void variablep();
void procdr_stmnt();
void procdr_stmntp();
void express_lst();
void express_lstp();
void express();
void expressp();
void simp_express();
void simp_expressp();
void term();
void termp();
void factor();
void factorp();
void sgn();
void match();
void prgrm()
{
 switch( tok.type )
  {

    case 3: // terminal is prgrm
      match("prgrm");
      match("id");
      match("(");
      id_list();
      match(")");
      match(";");
      prgrmp();
    break;


  }
}

void prgrmp()
{
 switch( tok.type )
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


  }
}

void prgrmpp()
{
 switch( tok.type )
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


  }
}

void id_list()
{
 switch( tok.type )
  {

    case 20: // terminal is id
      match("id");
      id_listp();
    break;


  }
}

void id_listp()
{
 switch( tok.type )
  {

    case 82: // terminal is ,
      match(",");
      match("id");
      id_listp();
    break;

    case 81 : // terminal is ) 
    break;


  }
}

void decls()
{
 switch( tok.type )
  {

    case 4: // terminal is var
      match("var");
      match("id");
      match(":");
      type();
      match(";");
      declsp();
    break;


  }
}

void declsp()
{
 switch( tok.type )
  {

    case 4: // terminal is var
      match("var");
      match("id");
      match(":");
      type();
      match(";");
      declsp();
    break;

    case 9 : // terminal is procdr 
    break;

    case 10 : // terminal is begin 
    break;


  }
}

void type()
{
 switch( tok.type )
  {

    case 5: // terminal is arr
      match("arr");
      match("[");
      match("num");
      match("..");
      match("num");
      match("]");
      match("of");
      stndrd_type();
    break;

    case 7: // terminal is int
      stndrd_type();
    break;

    case 8: // terminal is real
      stndrd_type();
    break;


  }
}

void stndrd_type()
{
 switch( tok.type )
  {

    case 7: // terminal is int
      match("int");
    break;

    case 8: // terminal is real
      match("real");
    break;


  }
}

void subprgrm_decls()
{
 switch( tok.type )
  {

    case 9: // terminal is procdr
      subprgrm_decl();
      match(";");
      subprgrm_declsp();
    break;


  }
}

void subprgrm_declsp()
{
 switch( tok.type )
  {

    case 9: // terminal is procdr
      subprgrm_decl();
      match(";");
      subprgrm_declsp();
    break;

    case 10 : // terminal is begin 
    break;


  }
}

void subprgrm_decl()
{
 switch( tok.type )
  {

    case 9: // terminal is procdr
      subprgrm_decl();
      subprgrm_declp();
    break;


  }
}

void subprgrm_declp()
{
 switch( tok.type )
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


  }
}

void subprgrm_declpp()
{
 switch( tok.type )
  {

    case 9: // terminal is procdr
      subprgrm_decls();
      cmpnd_stmnt();
    break;

    case 10: // terminal is begin
      cmpnd_stmnt();
    break;


  }
}

void subprgrm_head()
{
 switch( tok.type )
  {

    case 9: // terminal is procdr
      match("procdr");
      match("id");
      subprgrm_headp();
    break;


  }
}

void subprgrm_headp()
{
 switch( tok.type )
  {

    case 80: // terminal is (
      args();
      match(";");
    break;

    case 79: // terminal is ;
      match(";");
    break;


  }
}

void args()
{
 switch( tok.type )
  {

    case 80: // terminal is (
      match("(");
      param_lst();
      match(")");
    break;


  }
}

void param_lst()
{
 switch( tok.type )
  {

    case 20: // terminal is id
      match("id");
      match(":");
      type();
      param_lstp();
    break;


  }
}

void param_lstp()
{
 switch( tok.type )
  {

    case 79: // terminal is ;
      match(";");
      match("id");
      match(":");
      type();
      param_lstp();
    break;

    case 81 : // terminal is ) 
    break;


  }
}

void cmpnd_stmnt()
{
 switch( tok.type )
  {

    case 10: // terminal is begin
      match("begin");
      cmpnd_stmntp();
    break;


  }
}

void cmpnd_stmntp()
{
 switch( tok.type )
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


  }
}

void opt_stmnt()
{
 switch( tok.type )
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


  }
}

void stmnt_lst()
{
 switch( tok.type )
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


  }
}

void stmnt_lstp()
{
 switch( tok.type )
  {

    case 79: // terminal is ;
      match(";");
      stmnt();
      stmnt_lst();
    break;

    case 11 : // terminal is end 
    break;


  }
}

void stmnt()
{
 switch( tok.type )
  {

    case 20: // terminal is id
      variable();
      match("assignop");
      express();
    break;

    case 10: // terminal is begin
      cmpnd_stmnt();
    break;

    case 1: // terminal is if
      match("if");
      express();
      match("then");
      stmnt();
      match("else");
      stmnt();
    break;

    case 13: // terminal is while
      match("while");
      express();
      match("do");
      stmnt();
    break;

    case 21: // terminal is call
      procdr_stmnt();
    break;


  }
}

void variable()
{
 switch( tok.type )
  {

    case 20: // terminal is id
      match("id");
      variablep();
    break;


  }
}

void variablep()
{
 switch( tok.type )
  {

    case 84: // terminal is [
      match("[");
      express();
      match("]");
    break;

    case -1 : // terminal is assignop 
    break;


  }
}

void procdr_stmnt()
{
 switch( tok.type )
  {

    case 21: // terminal is call
      match("call");
      match("id");
      procdr_stmntp();
    break;


  }
}

void procdr_stmntp()
{
 switch( tok.type )
  {

    case 80: // terminal is (
      match("(");
      express_lst();
      match(")");
    break;

    case 78 : // terminal is . 
    break;

    case 12 : // terminal is else 
    break;


  }
}

void express_lst()
{
 switch( tok.type )
  {

    case 20: // terminal is id
      express();
      express_lstp();
    break;

    case 80: // terminal is (
      express();
      express_lstp();
    break;

    case 23: // terminal is num
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

    case 161: // terminal is -
      express();
      express_lstp();
    break;


  }
}

void express_lstp()
{
 switch( tok.type )
  {

    case 82: // terminal is ,
      match(",");
      express();
      express_lst();
    break;

    case 81 : // terminal is ) 
    break;


  }
}

void express()
{
 switch( tok.type )
  {

    case 20: // terminal is id
      simp_express();
      expressp();
    break;

    case 80: // terminal is (
      simp_express();
      expressp();
    break;

    case 23: // terminal is num
      simp_express();
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

    case 161: // terminal is -
      simp_express();
      expressp();
    break;


  }
}

void expressp()
{
 switch( tok.type )
  {

    case -1: // terminal is relop
      match("relop");
      simp_express();
    break;

    case 81 : // terminal is ) 
    break;

    case 79 : // terminal is ; 
    break;

    case 82 : // terminal is , 
    break;

    case 85 : // terminal is ] 
    break;

    case 11 : // terminal is end 
    break;

    case 2 : // terminal is then 
    break;

    case 14 : // terminal is do 
    break;


  }
}

void simp_express()
{
 switch( tok.type )
  {

    case 20: // terminal is id
      term();
      simp_express();
    break;

    case 80: // terminal is (
      term();
      simp_express();
    break;

    case 23: // terminal is num
      term();
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

    case 161: // terminal is -
      sgn();
      term();
      simp_expressp();
    break;


  }
}

void simp_expressp()
{
 switch( tok.type )
  {

    case -1: // terminal is addop
      match("addop");
      term();
      simp_expressp();
    break;

    case 81 : // terminal is ) 
    break;

    case 79 : // terminal is ; 
    break;

    case 82 : // terminal is , 
    break;

    case 85 : // terminal is ] 
    break;

    case 11 : // terminal is end 
    break;

    case 2 : // terminal is then 
    break;

    case 12 : // terminal is else 
    break;

    case 14 : // terminal is do 
    break;

    case -1 : // terminal is relop 
    break;


  }
}

void term()
{
 switch( tok.type )
  {

    case 20: // terminal is id
      factor();
      termp();
    break;

    case 80: // terminal is (
      factor();
      termp();
    break;

    case 23: // terminal is num
      factor();
      termp();
    break;

    case 19: // terminal is not
      factor();
      termp();
    break;


  }
}

void termp()
{
 switch( tok.type )
  {

    case -1: // terminal is mulop
      match("mulop");
      factor();
      termp();
    break;

    case 81 : // terminal is ) 
    break;

    case 79 : // terminal is ; 
    break;

    case 82 : // terminal is , 
    break;

    case 85 : // terminal is ] 
    break;

    case 11 : // terminal is end 
    break;

    case 2 : // terminal is then 
    break;

    case 12 : // terminal is else 
    break;

    case 14 : // terminal is do 
    break;

    case -1 : // terminal is relop 
    break;

    case -1 : // terminal is addop 
    break;


  }
}

void factor()
{
 switch( tok.type )
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

    case 23: // terminal is num
      match("num");
    break;

    case 19: // terminal is not
      match("not");
      factor();
    break;


  }
}

void factorp()
{
 switch( tok.type )
  {

    case 84: // terminal is [
      match("[");
      express();
      match("]");
    break;

    case 81 : // terminal is ) 
    break;

    case 79 : // terminal is ; 
    break;

    case 82 : // terminal is , 
    break;

    case 85 : // terminal is ] 
    break;

    case 11 : // terminal is end 
    break;

    case -1 : // terminal is assignop 
    break;

    case 2 : // terminal is then 
    break;

    case 12 : // terminal is else 
    break;

    case 14 : // terminal is do 
    break;

    case -1 : // terminal is relop 
    break;

    case -1 : // terminal is addop 
    break;

    case -1 : // terminal is mulop 
    break;


  }
}

void sgn()
{
 switch( tok.type )
  {

    case 161: // terminal is +
      match("+");
    break;

    case 161: // terminal is -
      match("-");
    break;


  }
}

void match(char * mat)
{
}



int main()
{
  printf("working now\n");
}

