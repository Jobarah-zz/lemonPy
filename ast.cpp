#include "ast.h"

map<string, int> vars;

#define BIN 0
#define DEC 1
#define HEX 2

int convertBin(int decimalnum);
void printConv(int number, int convType);

int AddExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();

	return v1 + v2;
}

int SubExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();

	return v1 - v2;
}

int MulExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();

	return v1 * v2;
}

int DivExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();

	return v1 / v2;
}

int VarExpr::eval(){
	return vars.at(index);
}

int EqualExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();

	return v1 == v2;
}

int NotEqualExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();

	return v1 != v2;
}

int GreaterThanExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();

	return v1 > v2;
}

int GreaterThanEqualExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();

	return v1 >= v2;
}

int LessThanExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();

	return v1 < v2;
}

int LessThanEqualExpr::eval(){
	int v1 = expr1->eval();
	int v2 = expr2->eval();

	return v1 <= v2;
}

void AssignStatement::exec(){
	int value = expr->eval();
	//if not in map insert else replace
	vars[this->varIndex] = value;
}

void PrintStatement::exec(){
	int value = expr->eval();
	printConv(value, format);
}

void BlockStatement::exec(){
	for(list<Statement*>::iterator i = statementList.begin(); i != statementList.end(); i++) {
		(*i)->exec();
	}
}

void IfStatement::exec() {
	if(this->expr->eval()) {
		if_statement->exec();
	} else {
		if(this->else_statement) {
			else_statement->exec();
		}
	}
}