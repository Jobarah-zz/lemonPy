%include {
    #include <iostream>
    #include <assert.h>
    #include "TokenStruct.h"
    #include "ast.h"

    #define BIN 0
    #define DEC 1
    #define HEX 2
}

%token_type { Token * }
%type stmt { Statement * }
%type stmts { Statement * }
%type block_stmt { Statement * }
%type if_stmt { Statement * }
%type while_stmt { Statement * }
%type optional_else { Statement * }
%type print_stmt { Statement * }
%type assign_stmt { Statement * }
%type expr { Expr * }
%type conditional_expr { Expr * }
%type relational_ops { Expr * }
%type term { Expr * }
%type factor { Expr * }
%type conv_type { int }
%name Parser
%start_symbol input

input::= opt_eols stmts(S). { S->exec(); }

stmts(S)::= stmts(S1) eols stmt(s). { S = S1; ((BlockStatement*)S)->addStatement(s); }
stmts(S)::= stmt(s). { S = new BlockStatement; ((BlockStatement*)S)->addStatement(s); }

eols::= eols TK_EOL.
eols::= TK_EOL.

opt_eols::= eols.
opt_eols::= .

stmt(S)::= assign_stmt(S1). { S = S1; }
stmt(S)::= print_stmt(S1). { S = S1; }
stmt(S)::= if_stmt(S1). { S = S1; }
stmt(S)::= while_stmt(S1). { S = S1; }

while_stmt(S)::= RW_WHILE TK_LEFT_PAR conditional_expr(S3) TK_RIGHT_PAR TK_EOL block_stmt(S6). { S = new WhileStatement(S3, S6); }

if_stmt(S)::= RW_IF TK_LEFT_PAR conditional_expr(S3) TK_RIGHT_PAR TK_EOL block_stmt(S6) optional_else(S7). { S = new IfStatement(S3,S6,S7); }

optional_else(S)::= RW_ELSE eols block_stmt(S3). { S = S3; }
optional_else(S)::= . { S = NULL; }

block_stmt(S)::= TK_LEFT_CB opt_eols stmts(S3) opt_eols TK_RIGHT_CB. { S = S3; }
block_stmt(S)::= stmt(S1). { S = S1; }

conditional_expr(S)::= expr(S1) relational_ops(S2) expr(S3). { S = S2; ((BinaryExpr*)S)->expr1 = S1; ((BinaryExpr*)S)->expr2 = S3; }

relational_ops(S)::= OP_GT. { S = new GreaterThanExpr(); }
relational_ops(S)::= OP_LT. { S = new LessThanExpr(); }
relational_ops(S)::= OP_GT_EQUAL. { S = new GreaterThanEqualExpr(); }
relational_ops(S)::= OP_LT_EQUAL. { S = new LessThanEqualExpr(); }
relational_ops(S)::= OP_NOT_EQUAL. { S = new NotEqualExpr(); }
relational_ops(S)::= OP_EQUAL. { S = new EqualExpr(); }

assign_stmt(S)::= TK_VAR(S1) OP_ASSIGN expr(S3). { S = new AssignStatement(S1->name, S3); }

print_stmt(S)::= RW_PRINT expr(S2) TK_COMMA conv_type(S4). {S = new PrintStatement(S2, S4); }

conv_type(S)::= RW_BIN. { S = BIN; }
conv_type(S)::= RW_DEC. { S = DEC; }
conv_type(S)::= RW_HEX. { S = HEX; }

expr(S)::=	expr(S1) OP_ADD term(S3). { S = new AddExpr(S1, S3); }
expr(S)::= expr(S1) OP_SUB term(S3). { S = new SubExpr(S1, S3); }
expr(S)::= term(S1). { S = S1; }

term(S)::= term(S1) OP_MUL factor(S3). { S = new MulExpr(S1, S3); }
term(S)::= term(S1) OP_DIV factor(S3). { S = new DivExpr(S1, S3); }
term(S)::= factor(S1). { S = S1; }

factor(S)::= TK_NUMBER(S1). { S = new NumberExpr(S1->value); }
factor(S)::= TK_VAR(S1). { S = new VarExpr(S1->name); }
factor(S)::= TK_LEFT_PAR expr(S2) TK_RIGHT_PAR. { S = S2; }