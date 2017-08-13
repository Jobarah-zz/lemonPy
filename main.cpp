#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "tokens.h"
#include "TokenStruct.h"

extern FILE * yyin;
int push_file(char *);

void * ParserAlloc(void *(*allocProc)(size_t));
void * Parser(void *, int, Token *);
void * ParserFree(void *, void (*freeProc)(void *));

int yylex();
extern Token *current_token;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input file>[]\n", argv[0]);
        return 1;
    }

    if (push_file(argv[1]) != 0)
        return 1;

    void *parser = ParserAlloc(malloc);
    int token = yylex();

    while (token != 0)
    {
        if(token == TK_EOL){
            while( (token = yylex()) == TK_EOL );
            if(token == 0) break;

            if(token == RW_ELSE){
                Parser(parser, RW_ELSE, 0);
            }else{
                Parser(parser, TK_EOL, 0);
                Parser(parser, token, current_token);
            }
        }else{
            Parser(parser, token, current_token);
        }
        token = yylex();
    }

    Parser(parser, 0, 0);

    ParserFree(parser, free);
}
