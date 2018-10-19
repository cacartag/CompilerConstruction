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
 switch( tok )
  {

    case prgrm:  
      match("prgrm");
      match("id");
      match("(");
      id_list();
      match(")");
      match(";");
      prgrmp();
  }


}

void prgrmp()
{
 switch( tok )
  {

    case var:  
      decls();
      prgrmpp();
    case procdr:  
      subprgrm_decls();
      cmpnd_stmnt();
      match(".");
    case begin:  
      cmpnd_stmnt();
      match(".");
  }


}

void prgrmpp()
{
 switch( tok )
  {

    case procdr:  
      subprgrm_decls();
      cmpnd_stmnt();
      match(".");
    case begin:  
      cmpnd_stmnt();
      match(".");
  }


}

void id_list()
{
 switch( tok )
  {

    case id:  
      match("id");
      id_listp();
  }


}

void id_listp()
{
 switch( tok )
  {

    case ,:  
      match(",");
      match("id");
      id_listp();
  }

  if ()
  {

  }

}

void decls()
{
 switch( tok )
  {

    case var:  
      match("var");
      match("id");
      match(":");
      type();
      match(";");
      declsp();
  }


}

void declsp()
{
 switch( tok )
  {

    case var:  
      match("var");
      match("id");
      match(":");
      type();
      match(";");
      declsp();
  }

  if (|| 
    )
  {

  }

}

void type()
{
 switch( tok )
  {

    case arr:  
      match("arr");
      match("[");
      match("num");
      match("..");
      match("num");
      match("]");
      match("of");
      stndrd_type();
    case int:  
      stndrd_type();
    case real:  
      stndrd_type();
  }


}

void stndrd_type()
{
 switch( tok )
  {

    case int:  
      match("int");
    case real:  
      match("real");
  }


}

void subprgrm_decls()
{
 switch( tok )
  {

    case procdr:  
      subprgrm_decl();
      match(";");
      subprgrm_declsp();
  }


}

void subprgrm_declsp()
{
 switch( tok )
  {

    case procdr:  
      subprgrm_decl();
      match(";");
      subprgrm_declsp();
  }

  if ()
  {

  }

}

void subprgrm_decl()
{
 switch( tok )
  {

    case procdr:  
      subprgrm_decl();
      subprgrm_declp();
  }


}

void subprgrm_declp()
{
 switch( tok )
  {

    case var:  
      decls();
      subprgrm_declpp();
    case procdr:  
      subprgrm_decls();
      cmpnd_stmnt();
    case begin:  
      cmpnd_stmnt();
  }


}

void subprgrm_declpp()
{
 switch( tok )
  {

    case procdr:  
      subprgrm_decls();
      cmpnd_stmnt();
    case begin:  
      cmpnd_stmnt();
  }


}

void subprgrm_head()
{
 switch( tok )
  {

    case procdr:  
      match("procdr");
      match("id");
      subprgrm_headp();
  }


}

void subprgrm_headp()
{
 switch( tok )
  {

    case (:  
      args();
      match(";");
    case ;:  
      match(";");
  }


}

void args()
{
 switch( tok )
  {

    case (:  
      match("(");
      param_lst();
      match(")");
  }


}

void param_lst()
{
 switch( tok )
  {

    case id:  
      match("id");
      match(":");
      type();
      param_lstp();
  }


}

void param_lstp()
{
 switch( tok )
  {

    case ;:  
      match(";");
      match("id");
      match(":");
      type();
      param_lstp();
  }

  if ()
  {

  }

}

void cmpnd_stmnt()
{
 switch( tok )
  {

    case begin:  
      match("begin");
      cmpnd_stmntp();
  }


}

void cmpnd_stmntp()
{
 switch( tok )
  {

    case id:  
      opt_stmnt();
      match("end");
    case begin:  
      opt_stmnt();
      match("end");
    case end:  
      match("end");
    case if:  
      opt_stmnt();
      match("end");
    case while:  
      opt_stmnt();
      match("end");
    case call:  
      opt_stmnt();
      match("end");
  }


}

void opt_stmnt()
{
 switch( tok )
  {

    case id:  
      stmnt_lst();
    case begin:  
      stmnt_lst();
    case if:  
      stmnt_lst();
    case while:  
      stmnt_lst();
    case call:  
      stmnt_lst();
  }


}

void stmnt_lst()
{
 switch( tok )
  {

    case id:  
      stmnt();
      stmnt_lstp();
    case begin:  
      stmnt();
      stmnt_lstp();
    case if:  
      stmnt();
      stmnt_lstp();
    case while:  
      stmnt();
      stmnt_lstp();
    case call:  
      stmnt();
      stmnt_lstp();
  }


}

void stmnt_lstp()
{
 switch( tok )
  {

    case ;:  
      match(";");
      stmnt();
      stmnt_lst();
  }

  if ()
  {

  }

}

void stmnt()
{
 switch( tok )
  {

    case id:  
      variable();
      match("assignop");
      express();
    case begin:  
      cmpnd_stmnt();
    case if:  
      match("if");
      express();
      match("then");
      stmnt();
      match("else");
      stmnt();
    case while:  
      match("while");
      express();
      match("do");
      stmnt();
    case call:  
      procdr_stmnt();
  }


}

void variable()
{
 switch( tok )
  {

    case id:  
      match("id");
      variablep();
  }


}

void variablep()
{
 switch( tok )
  {

    case [:  
      match("[");
      express();
      match("]");
  }

  if ()
  {

  }

}

void procdr_stmnt()
{
 switch( tok )
  {

    case call:  
      match("call");
      match("id");
      procdr_stmntp();
  }


}

void procdr_stmntp()
{
 switch( tok )
  {

    case (:  
      match("(");
      express_lst();
      match(")");
  }

  if (|| 
    )
  {

  }

}

void express_lst()
{
 switch( tok )
  {

    case id:  
      express();
      express_lstp();
    case (:  
      express();
      express_lstp();
    case num:  
      express();
      express_lstp();
    case not:  
      express();
      express_lstp();
    case +:  
      express();
      express_lstp();
    case -:  
      express();
      express_lstp();
  }


}

void express_lstp()
{
 switch( tok )
  {

    case ,:  
      match(",");
      express();
      express_lst();
  }

  if ()
  {

  }

}

void express()
{
 switch( tok )
  {

    case id:  
      simp_express();
      expressp();
    case (:  
      simp_express();
      expressp();
    case num:  
      simp_express();
      expressp();
    case not:  
      simp_express();
      expressp();
    case +:  
      simp_express();
      expressp();
    case -:  
      simp_express();
      expressp();
  }


}

void expressp()
{
 switch( tok )
  {

    case relop:  
      match("relop");
      simp_express();
  }

  if (|| 
    || 
    || 
    || 
    || 
    || 
    )
  {

  }

}

void simp_express()
{
 switch( tok )
  {

    case id:  
      term();
      simp_express();
    case (:  
      term();
      simp_express();
    case num:  
      term();
      simp_expressp();
    case not:  
      term();
      simp_expressp();
    case +:  
      sgn();
      term();
      simp_expressp();
    case -:  
      sgn();
      term();
      simp_expressp();
  }


}

void simp_expressp()
{
 switch( tok )
  {

    case addop:  
      match("addop");
      term();
      simp_expressp();
  }

  if (|| 
    || 
    || 
    || 
    || 
    || 
    || 
    || 
    )
  {

  }

}

void term()
{
 switch( tok )
  {

    case id:  
      factor();
      termp();
    case (:  
      factor();
      termp();
    case num:  
      factor();
      termp();
    case not:  
      factor();
      termp();
  }


}

void termp()
{
 switch( tok )
  {

    case mulop:  
      match("mulop");
      factor();
      termp();
  }

  if (|| 
    || 
    || 
    || 
    || 
    || 
    || 
    || 
    || 
    )
  {

  }

}

void factor()
{
 switch( tok )
  {

    case id:  
      match("id");
      factorp();
    case (:  
      match("(");
      express();
      match(")");
    case num:  
      match("num");
    case not:  
      match("not");
      factor();
  }


}

void factorp()
{
 switch( tok )
  {

    case [:  
      match("[");
      express();
      match("]");
  }

  if (|| 
    || 
    || 
    || 
    || 
    || 
    || 
    || 
    || 
    || 
    || 
    )
  {

  }

}

void sgn()
{
 switch( tok )
  {

    case +:  
      match("+");
    case -:  
      match("-");
  }


}

void match(char * mat)
{
}



int main()
{
  printf("working now\n");
}

