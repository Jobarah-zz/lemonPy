/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 1 "expr.y"

    #include <iostream>
    #include <assert.h>
    #include "TokenStruct.h"
    #include "ast.h"

    #define BIN 0
    #define DEC 1
    #define HEX 2
#line 38 "expr_parser.cpp"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParserTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParserTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParserARG_SDECL     A static variable declaration for the %extra_argument
**    ParserARG_PDECL     A parameter declaration for the %extra_argument
**    ParserARG_STORE     Code to store %extra_argument into yypParser
**    ParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 43
#define YYACTIONTYPE unsigned char
#define ParserTOKENTYPE  Token * 
typedef union {
  int yyinit;
  ParserTOKENTYPE yy0;
  int yy4;
  Expr * yy74;
  Statement * yy80;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParserARG_SDECL
#define ParserARG_PDECL
#define ParserARG_FETCH
#define ParserARG_STORE
#define YYNSTATE             31
#define YYNRULE              34
#define YY_MAX_SHIFT         30
#define YY_MIN_SHIFTREDUCE   58
#define YY_MAX_SHIFTREDUCE   91
#define YY_MIN_REDUCE        92
#define YY_MAX_REDUCE        125
#define YY_ERROR_ACTION      126
#define YY_ACCEPT_ACTION     127
#define YY_NO_ACTION         128
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE

**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (105)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    72,   73,   74,   75,   76,   77,   60,   17,    6,   65,
 /*    10 */    79,   64,   63,   11,   10,   61,   30,   24,   88,  127,
 /*    20 */     3,   70,   87,   67,   65,   86,   64,   63,   27,   70,
 /*    30 */     7,   19,   65,   62,   64,   63,    4,   60,   16,    8,
 /*    40 */    65,   30,   64,   63,    1,    3,   18,   61,   30,   66,
 /*    50 */    90,   59,    9,   27,   65,    7,   64,   63,   30,    2,
 /*    60 */    27,   89,    7,   12,   29,   22,   23,   88,   23,   88,
 /*    70 */    27,   28,    7,    5,   20,   91,   21,   23,   88,   23,
 /*    80 */    88,   94,   26,   92,   62,   23,   88,   80,   81,   82,
 /*    90 */    13,   94,   11,   10,   11,   10,   25,   88,   62,   11,
 /*   100 */    10,   94,   15,   14,   69,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     8,    9,   10,   11,   12,   13,   27,   28,   36,   30,
 /*    10 */    39,   32,   33,   21,   22,    1,    2,   37,   38,   40,
 /*    20 */     6,   27,   38,   29,   30,   38,   32,   33,   14,   27,
 /*    30 */    16,   29,   30,    1,   32,   33,   41,   27,   28,    3,
 /*    40 */    30,    2,   32,   33,   41,    6,    5,    1,    2,   31,
 /*    50 */    14,   27,   15,   14,   30,   16,   32,   33,    2,    1,
 /*    60 */    14,   25,   16,   34,   35,   34,   37,   38,   37,   38,
 /*    70 */    14,    4,   16,    3,   34,    4,   34,   37,   38,   37,
 /*    80 */    38,   42,   34,    0,    1,   37,   38,   18,   19,   20,
 /*    90 */    17,   42,   21,   22,   21,   22,   37,   38,    1,   21,
 /*   100 */    22,   42,   23,   24,    7,
};
#define YY_SHIFT_USE_DFLT (-9)
#define YY_SHIFT_COUNT (30)
#define YY_SHIFT_MIN   (-8)
#define YY_SHIFT_MAX   (97)
static const signed char yy_shift_ofst[] = {
 /*     0 */    56,   14,   39,   56,   46,   36,   36,   36,   36,   36,
 /*    10 */    36,   36,   -8,   69,   36,   36,   97,   83,   32,   41,
 /*    20 */    73,   71,   78,   79,   79,   79,   78,   37,   58,   67,
 /*    30 */    70,
};
#define YY_REDUCE_USE_DFLT (-30)
#define YY_REDUCE_COUNT (19)
#define YY_REDUCE_MIN   (-29)
#define YY_REDUCE_MAX   (59)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -21,   -6,    2,   10,   24,   29,   31,   40,   42,   48,
 /*    10 */   -20,   59,  -28,  -29,  -16,  -13,   -5,   -5,    3,   18,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   126,  126,  126,  126,  126,  126,  126,  126,  126,  126,
 /*    10 */   126,  126,  126,  126,  126,  126,  126,  126,  126,  102,
 /*    20 */   126,  126,  105,  119,  118,  117,  112,  126,  126,  126,
 /*    30 */   126,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParserTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "TK_EOL",        "RW_IF",         "TK_LEFT_PAR", 
  "TK_RIGHT_PAR",  "RW_ELSE",       "TK_LEFT_CB",    "TK_RIGHT_CB", 
  "OP_GT",         "OP_LT",         "OP_GT_EQUAL",   "OP_LT_EQUAL", 
  "OP_NOT_EQUAL",  "OP_EQUAL",      "TK_VAR",        "OP_ASSIGN",   
  "RW_PRINT",      "TK_COMMA",      "RW_BIN",        "RW_DEC",      
  "RW_HEX",        "OP_ADD",        "OP_SUB",        "OP_MUL",      
  "OP_DIV",        "TK_NUMBER",     "error",         "stmt",        
  "stmts",         "block_stmt",    "if_stmnt",      "optional_else",
  "print_stmt",    "assign_stmt",   "expr",          "conditional_expr",
  "relational_ops",  "term",          "factor",        "conv_type",   
  "input",         "eols",        
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "input ::= stmts",
 /*   1 */ "stmts ::= stmts eols stmt",
 /*   2 */ "stmts ::= stmt",
 /*   3 */ "eols ::= eols TK_EOL",
 /*   4 */ "eols ::= TK_EOL",
 /*   5 */ "stmt ::= assign_stmt",
 /*   6 */ "stmt ::= print_stmt",
 /*   7 */ "stmt ::= if_stmnt",
 /*   8 */ "if_stmnt ::= RW_IF TK_LEFT_PAR conditional_expr TK_RIGHT_PAR TK_EOL block_stmt optional_else",
 /*   9 */ "optional_else ::= RW_ELSE eols block_stmt",
 /*  10 */ "optional_else ::=",
 /*  11 */ "block_stmt ::= TK_LEFT_CB stmts TK_RIGHT_CB",
 /*  12 */ "block_stmt ::= stmt",
 /*  13 */ "conditional_expr ::= expr relational_ops expr",
 /*  14 */ "relational_ops ::= OP_GT",
 /*  15 */ "relational_ops ::= OP_LT",
 /*  16 */ "relational_ops ::= OP_GT_EQUAL",
 /*  17 */ "relational_ops ::= OP_LT_EQUAL",
 /*  18 */ "relational_ops ::= OP_NOT_EQUAL",
 /*  19 */ "relational_ops ::= OP_EQUAL",
 /*  20 */ "assign_stmt ::= TK_VAR OP_ASSIGN expr",
 /*  21 */ "print_stmt ::= RW_PRINT expr TK_COMMA conv_type",
 /*  22 */ "conv_type ::= RW_BIN",
 /*  23 */ "conv_type ::= RW_DEC",
 /*  24 */ "conv_type ::= RW_HEX",
 /*  25 */ "expr ::= expr OP_ADD term",
 /*  26 */ "expr ::= expr OP_SUB term",
 /*  27 */ "expr ::= term",
 /*  28 */ "term ::= term OP_MUL factor",
 /*  29 */ "term ::= term OP_DIV factor",
 /*  30 */ "term ::= factor",
 /*  31 */ "factor ::= TK_NUMBER",
 /*  32 */ "factor ::= TK_VAR",
 /*  33 */ "factor ::= TK_LEFT_PAR expr TK_RIGHT_PAR",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParserAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parser and ParserFree.
*/
void *ParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParserARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yyidx>=0 );
  yytos = &pParser->yystack[pParser->yyidx--];
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    if( i==YY_SHIFT_USE_DFLT ) return yy_default[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
      if( iLookAhead>0 ){
#ifdef YYFALLBACK
        YYCODETYPE iFallback;            /* Fallback token */
        if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
               && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
          }
#endif
          assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
          iLookAhead = iFallback;
          continue;
        }
#endif
#ifdef YYWILDCARD
        {
          int j = i - iLookAhead + YYWILDCARD;
          if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
            j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
            j<YY_ACTTAB_COUNT &&
#endif
            yy_lookahead[j]==YYWILDCARD
          ){
#ifndef NDEBUG
            if( yyTraceFILE ){
              fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
                 yyTracePrompt, yyTokenName[iLookAhead],
                 yyTokenName[YYWILDCARD]);
            }
#endif /* NDEBUG */
            return yy_action[j];
          }
        }
#endif /* YYWILDCARD */
      }
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ParserARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  ParserTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 40, 1 },
  { 28, 3 },
  { 28, 1 },
  { 41, 2 },
  { 41, 1 },
  { 27, 1 },
  { 27, 1 },
  { 27, 1 },
  { 30, 7 },
  { 31, 3 },
  { 31, 0 },
  { 29, 3 },
  { 29, 1 },
  { 35, 3 },
  { 36, 1 },
  { 36, 1 },
  { 36, 1 },
  { 36, 1 },
  { 36, 1 },
  { 36, 1 },
  { 33, 3 },
  { 32, 4 },
  { 39, 1 },
  { 39, 1 },
  { 39, 1 },
  { 34, 3 },
  { 34, 3 },
  { 34, 1 },
  { 37, 3 },
  { 37, 3 },
  { 37, 1 },
  { 38, 1 },
  { 38, 1 },
  { 38, 3 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParserARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( yypParser->yyidx>yypParser->yyidxMax ){
      yypParser->yyidxMax = yypParser->yyidx;
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yyidx>=YYSTACKDEPTH-1 ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yyidx>=yypParser->yystksz-1 ){
      yyGrowStack(yypParser);
      if( yypParser->yyidx>=yypParser->yystksz-1 ){
        yyStackOverflow(yypParser);
        return;
      }
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* input ::= stmts */
#line 28 "expr.y"
{ yymsp[0].minor.yy80->exec(); }
#line 825 "expr_parser.cpp"
        break;
      case 1: /* stmts ::= stmts eols stmt */
#line 30 "expr.y"
{ yylhsminor.yy80 = yymsp[-2].minor.yy80; ((BlockStatement*)yylhsminor.yy80)->addStatement(yymsp[0].minor.yy80); }
#line 830 "expr_parser.cpp"
  yymsp[-2].minor.yy80 = yylhsminor.yy80;
        break;
      case 2: /* stmts ::= stmt */
#line 31 "expr.y"
{ yylhsminor.yy80 = new BlockStatement; ((BlockStatement*)yylhsminor.yy80)->addStatement(yymsp[0].minor.yy80); }
#line 836 "expr_parser.cpp"
  yymsp[0].minor.yy80 = yylhsminor.yy80;
        break;
      case 5: /* stmt ::= assign_stmt */
      case 6: /* stmt ::= print_stmt */ yytestcase(yyruleno==6);
      case 7: /* stmt ::= if_stmnt */ yytestcase(yyruleno==7);
      case 12: /* block_stmt ::= stmt */ yytestcase(yyruleno==12);
#line 36 "expr.y"
{ yylhsminor.yy80 = yymsp[0].minor.yy80; }
#line 845 "expr_parser.cpp"
  yymsp[0].minor.yy80 = yylhsminor.yy80;
        break;
      case 8: /* if_stmnt ::= RW_IF TK_LEFT_PAR conditional_expr TK_RIGHT_PAR TK_EOL block_stmt optional_else */
#line 40 "expr.y"
{ yymsp[-6].minor.yy80 = new IfStatement(yymsp[-4].minor.yy74,yymsp[-1].minor.yy80,yymsp[0].minor.yy80); }
#line 851 "expr_parser.cpp"
        break;
      case 9: /* optional_else ::= RW_ELSE eols block_stmt */
#line 42 "expr.y"
{ yymsp[-2].minor.yy80 = yymsp[0].minor.yy80; }
#line 856 "expr_parser.cpp"
        break;
      case 10: /* optional_else ::= */
#line 43 "expr.y"
{ yymsp[1].minor.yy80 = NULL; }
#line 861 "expr_parser.cpp"
        break;
      case 11: /* block_stmt ::= TK_LEFT_CB stmts TK_RIGHT_CB */
#line 45 "expr.y"
{ yymsp[-2].minor.yy80 = yymsp[-1].minor.yy80; }
#line 866 "expr_parser.cpp"
        break;
      case 13: /* conditional_expr ::= expr relational_ops expr */
#line 48 "expr.y"
{ yylhsminor.yy74 = yymsp[-1].minor.yy74; ((BinaryExpr*)yylhsminor.yy74)->expr1 = yymsp[-2].minor.yy74; ((BinaryExpr*)yylhsminor.yy74)->expr2 = yymsp[0].minor.yy74; }
#line 871 "expr_parser.cpp"
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 14: /* relational_ops ::= OP_GT */
#line 50 "expr.y"
{ yymsp[0].minor.yy74 = new GreaterThanExpr(); }
#line 877 "expr_parser.cpp"
        break;
      case 15: /* relational_ops ::= OP_LT */
#line 51 "expr.y"
{ yymsp[0].minor.yy74 = new LessThanExpr(); }
#line 882 "expr_parser.cpp"
        break;
      case 16: /* relational_ops ::= OP_GT_EQUAL */
#line 52 "expr.y"
{ yymsp[0].minor.yy74 = new GreaterThanEqualExpr(); }
#line 887 "expr_parser.cpp"
        break;
      case 17: /* relational_ops ::= OP_LT_EQUAL */
#line 53 "expr.y"
{ yymsp[0].minor.yy74 = new LessThanEqualExpr(); }
#line 892 "expr_parser.cpp"
        break;
      case 18: /* relational_ops ::= OP_NOT_EQUAL */
#line 54 "expr.y"
{ yymsp[0].minor.yy74 = new NotEqualExpr(); }
#line 897 "expr_parser.cpp"
        break;
      case 19: /* relational_ops ::= OP_EQUAL */
#line 55 "expr.y"
{ yymsp[0].minor.yy74 = new EqualExpr(); }
#line 902 "expr_parser.cpp"
        break;
      case 20: /* assign_stmt ::= TK_VAR OP_ASSIGN expr */
#line 57 "expr.y"
{ yylhsminor.yy80 = new AssignStatement(yymsp[-2].minor.yy0->name, yymsp[0].minor.yy74); }
#line 907 "expr_parser.cpp"
  yymsp[-2].minor.yy80 = yylhsminor.yy80;
        break;
      case 21: /* print_stmt ::= RW_PRINT expr TK_COMMA conv_type */
#line 59 "expr.y"
{yymsp[-3].minor.yy80 = new PrintStatement(yymsp[-2].minor.yy74, yymsp[0].minor.yy4); }
#line 913 "expr_parser.cpp"
        break;
      case 22: /* conv_type ::= RW_BIN */
#line 61 "expr.y"
{ yymsp[0].minor.yy4 = BIN; }
#line 918 "expr_parser.cpp"
        break;
      case 23: /* conv_type ::= RW_DEC */
#line 62 "expr.y"
{ yymsp[0].minor.yy4 = DEC; }
#line 923 "expr_parser.cpp"
        break;
      case 24: /* conv_type ::= RW_HEX */
#line 63 "expr.y"
{ yymsp[0].minor.yy4 = HEX; }
#line 928 "expr_parser.cpp"
        break;
      case 25: /* expr ::= expr OP_ADD term */
#line 65 "expr.y"
{ yylhsminor.yy74 = new AddExpr(yymsp[-2].minor.yy74, yymsp[0].minor.yy74); }
#line 933 "expr_parser.cpp"
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 26: /* expr ::= expr OP_SUB term */
#line 66 "expr.y"
{ yylhsminor.yy74 = new SubExpr(yymsp[-2].minor.yy74, yymsp[0].minor.yy74); }
#line 939 "expr_parser.cpp"
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 27: /* expr ::= term */
      case 30: /* term ::= factor */ yytestcase(yyruleno==30);
#line 67 "expr.y"
{ yylhsminor.yy74 = yymsp[0].minor.yy74; }
#line 946 "expr_parser.cpp"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 28: /* term ::= term OP_MUL factor */
#line 69 "expr.y"
{ yylhsminor.yy74 = new MulExpr(yymsp[-2].minor.yy74, yymsp[0].minor.yy74); }
#line 952 "expr_parser.cpp"
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 29: /* term ::= term OP_DIV factor */
#line 70 "expr.y"
{ yylhsminor.yy74 = new DivExpr(yymsp[-2].minor.yy74, yymsp[0].minor.yy74); }
#line 958 "expr_parser.cpp"
  yymsp[-2].minor.yy74 = yylhsminor.yy74;
        break;
      case 31: /* factor ::= TK_NUMBER */
#line 73 "expr.y"
{ yylhsminor.yy74 = new NumberExpr(yymsp[0].minor.yy0->value); }
#line 964 "expr_parser.cpp"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 32: /* factor ::= TK_VAR */
#line 74 "expr.y"
{ yylhsminor.yy74 = new VarExpr(yymsp[0].minor.yy0->name); }
#line 970 "expr_parser.cpp"
  yymsp[0].minor.yy74 = yylhsminor.yy74;
        break;
      case 33: /* factor ::= TK_LEFT_PAR expr TK_RIGHT_PAR */
#line 75 "expr.y"
{ yymsp[-2].minor.yy74 = yymsp[-1].minor.yy74; }
#line 976 "expr_parser.cpp"
        break;
      default:
      /* (3) eols ::= eols TK_EOL */ yytestcase(yyruleno==3);
      /* (4) eols ::= TK_EOL */ yytestcase(yyruleno==4);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno>=0 && yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    yypParser->yyidx -= yysize - 1;
    yymsp -= yysize-1;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yypParser->yyidx -= yysize;
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParserTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParserARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
/************ End %syntax_error code ******************************************/
  ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  ParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParserAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParserTOKENTYPE yyminor       /* The value for the token */
  ParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      yyStackOverflow(yypParser);
      return;
    }
#endif
    yypParser->yyidx = 0;
#ifndef YYNOERRORRECOVERY
    yypParser->yyerrcnt = -1;
#endif
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sInitialize. Empty stack. State 0\n",
              yyTracePrompt);
    }
#endif
  }
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      if( yyact > YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
#ifndef NDEBUG
  if( yyTraceFILE ){
    int i;
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE,"%c%s", i==1 ? '[' : ' ', 
              yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
