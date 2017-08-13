TARGET=lemonPy
EXPR_PARSER_SRC=expr_parser.cpp
EXPR_LEXER_SRC=expr_lexer.cpp
C_SRCFILES=$(EXPR_PARSER_SRC) $(EXPR_LEXER_SRC) ast.cpp main.cpp
OBJ_FILES=${C_SRCFILES:.cpp=.o}

.PHONY: clean

$(TARGET): $(OBJ_FILES)
	g++ -o $@ $^

$(EXPR_LEXER_SRC): expr.l
	flex -o $@ $^

$(EXPR_PARSER_SRC): expr.y
	./lemon -o$@ -dexpr_tokens.h -Tlempar.c $<

%o.: %.cpp tokens.h
	g++ -c -o $@ $<

run: $(TARGET
	./$(TARGET) TextInputs/input1.txt