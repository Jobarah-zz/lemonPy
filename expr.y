%include {
    #include <iostream>
    #include "TokenStruct.h"
    #include "ast.h"
}

#define BIN 0
#define DEC 1
#define HEX 2

%token_type { Token_t }
%type stmts { Statement * }
%type stmt { Statement * }

input::= stmts(S). { S->exec(); }

stmts(S)::= stmts(S1) eols stmt(s). { S = S1; ((BlockStatement*)S)->addStatement(s); }
stmts(S)::= stmt. { S = new BlockStatement; ((BlockStatement*)S)->addStatement(s); }

eols::= eols TK_EOL.
eols::= TK_EOL.

stmt(S)::= assign_stmt(S1). { S = S1 }
stmt(S)::= print_stmt(S1). { S = S1 }
stmt(S)::= if_stmnt(S1). { S = S1 }

if_stmnt(S)::= KW_IF TK_LEFT_PAR logic_expression(S3) TK_RIGHT_PAR eols optional_block_stmt(S6) eols KW_ELSE eols optional_block_stmt(S10). { S = new IfStatement(S3,S6,S10); }
if_stmnt(S)::= KW_IF TK_LEFT_PAR logic_expression(S3) TK_RIGHT_PAR eols optional_block_stmt(S6). { S = new IfStatement(S3,S6,NULL); }

optional_block_stmt(S)::= TK_LEFT_CURLY_BRAKET optional_eols stmts(S3) optional_eols TK_RIGHT_CURLY_BRAKET. { S = S3; }
optional_block_stmt(S)::= stmt(S1). { S = S1; }

conditional_expr(S)::= expr(S1) relational_ops(S2) expr(S3). { S = S2; ((BinaryExpr*)S)->expr1 = S1; ((BinaryExpr*)S)->expr2 = S3; }

relational_ops(S)::= OP_GT. { S = new GreaterThanExpr(); }
relational_ops(S)::= OP_LT. { S = new LessThanExpr(); }
relational_ops(S)::= OP_GT_EQUAL. { S = new GreaterThanEqualExpr(); }
relational_ops(S)::= OP_LT_EQUAL. { S = new LessThanEqualExpr(); }
relational_ops(S)::= OP_NOT_EQUAL. { S = new NotEqualExpr(); }
relational_ops(S)::= OP_EQUAL. { S = new EqualExpr(); }

assign_stmt(S)::= TK_VAR(S1) OP_ASSIGN expr(S3). { S = new AssignStatement(S1, S3); }

print_stmt(S)::= RW_PRINT expr(S2) TK_COMMA conv_type(S4). {S = new PrintStatement(S2, S4); }

conv_type(S)::= RW_BIN. { S = BIN; }
conv_type(S)::= RW_DEC. { S = DEC; }
conv_type(S)::= RW_HEX. { S = HEX; }

expr(S)::=	expr OP_ADD term. { S = new AddExpr($1, $3); }
expr(S)::= expr OP_SUB term. { S = new SubExpr($1, $3); }
expr(S)::= term. { S = $1; }

term(S)::= term(S1) OP_MUL factor(S3). { S = new MulExpr(S1, S3); }
term(S)::= term(S1) OP_DIV factor(S3). { S = new DivExpr(S1, S3); }
term(S)::= factor(S1). { S = S1; }

factor::= TK_NUMBER(S1). { S = new NumberExpr(S1); }
factor::= TK_VAR(S1). { S = new VarExpr(S1); }
factor::= TK_LEFT_PAR expr(S2) TK_RIGHT_PAR. { S = S2; }