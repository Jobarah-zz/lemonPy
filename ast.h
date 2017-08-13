#ifndef _AST_H
#define _AST_H

#include <list>
#include <map>
#include <stdio.h>
#include <iostream>


using namespace std;


class Expr {
public:
	Expr() {}
	virtual int eval() = 0;
};

class BinaryExpr: public Expr{
protected:
	BinaryExpr(Expr *expr1, Expr *expr2){
		this->expr1 = expr1;
		this->expr2 = expr2;
	}
public:
	Expr *expr1, *expr2;
};

class GreaterThanExpr: public BinaryExpr {
	public:
		GreaterThanExpr() : BinaryExpr(NULL, NULL){}
		GreaterThanExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2){}
		int eval();
};

class EqualExpr: public BinaryExpr {
	public:
		EqualExpr() : BinaryExpr(NULL, NULL){}
		EqualExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2){}
		int eval();
};

class NotEqualExpr: public BinaryExpr {
	public:
		NotEqualExpr() : BinaryExpr(NULL, NULL){}
		NotEqualExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2){}
		int eval();
};

class LessThanExpr: public BinaryExpr {
	public:
		LessThanExpr() : BinaryExpr(NULL, NULL){}
		LessThanExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2){}
		int eval();
};

class GreaterThanEqualExpr: public BinaryExpr {
	public:
		GreaterThanEqualExpr() : BinaryExpr(NULL, NULL){}
		GreaterThanEqualExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2){}
		int eval();
};

class LessThanEqualExpr: public BinaryExpr {
	public:
		LessThanEqualExpr() : BinaryExpr(NULL, NULL){}
		LessThanEqualExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2){}
		int eval();
};

class AddExpr: public BinaryExpr {
public:
	AddExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2){}
	int eval();
};

class SubExpr: public BinaryExpr {
public:
	SubExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2){}
	int eval();
};

class MulExpr: public BinaryExpr {
public:
	MulExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2){}
	int eval();
};

class DivExpr: public BinaryExpr {
public:
	DivExpr(Expr *expr1, Expr *expr2): BinaryExpr(expr1, expr2){}
	int eval();
};

class NumberExpr: public Expr {
public:
	NumberExpr(int value){
		this->value = value;
	}

	int eval(){
		return value;
	}

	int value;
};

class VarExpr: public Expr{
public:
	VarExpr(string* index){
		this->index = *index;
	}
	int eval();
	string index;
};

class Statement{
	public:
		Statement() {}
		virtual void exec() = 0;
};

class AssignStatement: public Statement{
public:
	AssignStatement(string* varIndex, Expr *expr){
		this->varIndex = *varIndex;
		this->expr = expr;
	}

	void exec();

	string varIndex;
	Expr *expr;
};

class PrintStatement: public Statement{
public:
	PrintStatement(Expr *expr, int format){
		this->expr = expr;
		this->format = format;
	}

	void exec();

	Expr *expr;
	int format;
};

class BlockStatement: public Statement{
public:
	BlockStatement(){}

	void addStatement(Statement *statement){
		statementList.push_back(statement);
	}

	void exec();

	list <Statement*> statementList;
};

class IfStatement: public Statement{
	public:
		IfStatement(Expr *expr, Statement *if_statement, Statement *else_statement) {
			this->expr = expr;
			this->if_statement = if_statement;
			this->else_statement = else_statement;
		}

		void exec();
		
		Expr *expr;
		Statement *if_statement, *else_statement;
};

class WhileStatement: public Statement{
	public:
		WhileStatement(Expr *expr, Statement *body) {
			this->expr = expr;
			this->body = body;
		}

		void exec();
		
		Expr *expr;
		Statement *body;
};

#endif