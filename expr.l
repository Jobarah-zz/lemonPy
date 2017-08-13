%option noyywrap
%option yylineno

%x comment
%x include_quotes
%x include_lt
%x eol_state
%x comment_in_eol

%{
	#include <stdio.h>
	#include <iostream>
	#include "tokens.h"
	#include "TokenStruct.h"

	using namespace std;

	typedef struct include_stack{
		struct include_stack *prev;
		FILE *f;
		int lineno;
		char *filename;
		YY_BUFFER_STATE bs;
	} include_stack_t;

	include_stack_t *inc_stack = NULL;
	char *yyfilename;
	int push_file(char *filename);
	int pop_file();

	Token *current_token;
%}

%%
[ \t]		{/* Nada */}
^"#"[ \t]*"include"[ \t]*\"	{ BEGIN(include_quotes); }
^"#"[ \t]*"include"[ \t]*"<"	{ BEGIN(include_lt); }
"/*"		{ BEGIN(comment); }
"//".*\n	{ return TK_EOL; }
"+"			{ return OP_ADD; }
"-"			{ return OP_SUB; }
"*"			{ return OP_MUL; }
"/"			{ return OP_DIV; }
"{"			{ return TK_LEFT_CB; }
"}"			{ return TK_RIGHT_CB; }
"("			{ return TK_LEFT_PAR; }
")"			{ return TK_RIGHT_PAR; }
","			{ return TK_COMMA; }
[0-9]+		{ current_token = new Token(NULL, atoi(yytext)); return TK_NUMBER; }
">"			{ return OP_GT; }
"<"			{ return OP_LT; }
">="		{ return OP_GT_EQUAL; }
"<="		{ return OP_LT_EQUAL; }
"!="		{ return OP_NOT_EQUAL; }
"=="		{ return OP_EQUAL; }
"="			{ return OP_ASSIGN; }
"if"		{ return RW_IF; }
"else"		{ return RW_ELSE; }
"while"		{ return RW_WHILE; }
"print"		{ return RW_PRINT; }
"bin"		{ return RW_BIN; }
"dec"		{ return RW_DEC; }
"hex"		{ return RW_HEX; }
[_a-zA-Z][a-zA-Z0-9_]* { current_token = new Token (new string(yytext), -1); return TK_VAR; }
"\n"		{ return TK_EOL; }
<INITIAL><<EOF>>		{ if (pop_file() != 0) yyterminate(); }
.			{ printf("Unexpected Token at line: %d\n", yylineno); yyterminate(); }


<include_quotes>[^\"\n]+ {

	{
		int c;
		while ((c = yyinput())){
			if (c == '\"')
				break;
			if (c == '\n'){
				printf("Line %d: Include was never closed\n", yylineno); return 0;
			}
		}
		yylineno++;
		if (push_file(yytext) != 0){
			yyterminate();
		}
		BEGIN(INITIAL);
	}
}

<include_lt>[^">"\n]+ {

	{
		int c;
		while ((c = yyinput())){
			if (c == '>')
				break;
			if (c == '\n'){
				printf("Line %d: Include was never closed\n", yylineno); return 0;
			}
		}
		yylineno++;
		if (push_file(yytext) != 0){
			yyterminate();
		}
		BEGIN(INITIAL);
	}
}

<comment>"*/"		{ BEGIN(INITIAL); }
<comment>.|\n		{ /* Nothing */ }
<comment><<EOF>>	{ printf("Line %d: Block comment was never closed\n", yylineno); return 0; }

<eol_state>\n {  }
<eol_state>"//".\n {  }
<eol_state>[ \t] {/* Nada */}
<eol_state>"/*" { BEGIN(comment_in_eol); }
<eol_state>"else" { BEGIN(INITIAL); return RW_ELSE; }
<eol_state>. { BEGIN(INITIAL); unput(yytext[0]); return TK_EOL; }

<comment_in_eol>"*/" { BEGIN(eol_state); }
<comment_in_eol>.|\n {  }
<comment_in_eol><<EOF>> { printf("Line %d: Block coment not ended\n", yylineno); return 0; }


%%

int push_file(char *filename) {
	FILE *f = fopen(filename, "r");

	if (f == NULL){
		fprintf(stderr, "Cannot open file %s\n", filename);
		return 1;
	}
	include_stack_t *is =(include_stack_t*) malloc(sizeof(include_stack_t));
	if (is == NULL){
		fprintf(stderr, "Out of memory.\n");
		return 1;
	}
	is->f = f;
	is->lineno = yylineno;;
	is->filename = yyfilename;
	is->bs = yy_create_buffer(f, YY_BUF_SIZE);
	is->prev = inc_stack;
	inc_stack = is;
	yyfilename = filename;
	yylineno = 1;
	yy_switch_to_buffer(is->bs);
	return 0;
}

int pop_file() {
	include_stack_t *prev_is = inc_stack->prev;
	fclose(inc_stack->f);
	yy_delete_buffer(inc_stack->bs);

	yylineno = inc_stack->lineno;
	yyfilename = inc_stack->filename;

	free(inc_stack);

	if (prev_is == NULL) return 1;
	yy_switch_to_buffer(prev_is->bs);
	inc_stack = prev_is;
	return 0;
}