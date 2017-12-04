#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <sstream>
#include <iterator>
#include "tokeniser.h"
#include "symbols.h"
#include "csdocument.h"
#include "iobuffer.h"

using namespace std ;

static string className = "" ;
static int tab_counter = 0;

void prettyBuffer(string str1)
{
	write_to_buffer(str1);
	//comment out the following cout when this program is complete
	//cout << str1;
}

void tabPrinter()
{
	for(int i = 0; i < tab_counter; i++)
	{
		prettyBuffer("    ");
	}
}

//declare all functions here
void prettyClass(cs_node ) ;
void prettyClassVarDecs(cs_node ) ;
void prettyClassVarDec(cs_node) ;
void prettyType(cs_node ) ;
void prettyVtype(cs_node ) ;
void prettySubroutineDecs(cs_node ) ;
void prettyConstructor(cs_node ) ;
void prettyFunction(cs_node ) ;
void prettyMethod(cs_node ) ;
void prettyParameterList(cs_node ) ;
void prettyParameter(cs_node ) ;
void prettySubroutineBody(cs_node ) ;
void prettyVarDecs(cs_node ) ;
void prettyVarDec(cs_node ) ;
void prettyClassName() ;
void prettySubroutineName(cs_node ) ;
void prettyVarName(cs_node ) ;
void prettyStatements(cs_node ) ;
void prettyStatement(cs_node , int) ;
void prettyLetStatement(cs_node ) ;
void prettyIfStatement(cs_node , int) ;
void prettyWhileStatement(cs_node , int) ;
void prettyDoStatement(cs_node ) ;
void prettyReturnStatement(cs_node ) ;
void prettyOffset(cs_node ) ;
void prettyExpression(cs_node ) ;
void prettyTerm(cs_node ) ;
void prettyArrayIndex(cs_node ) ;
void prettySubroutineCall(cs_node ) ;
void prettySubroutineCall(cs_node ) ;
void prettyExpressionList(cs_node ) ;
void prettyInfixOp(cs_node ) ;
void prettyUnaryOp(cs_node ) ;
void prettyKeywordConstant(cs_node ) ;

// now implement the pretty functions
void jack_pretty(cs_node t)
{
	prettyClass(t);
	output_buffer();
}


void prettyClass(cs_node t)
{
	prettyBuffer("class ");

	className = cs_node_value(cs_get_child(t,0));

	prettyBuffer(className + "\n");

	prettyBuffer("{\n");

	tab_counter++;

	prettyClassVarDecs(cs_get_child(t,1));
	prettySubroutineDecs(cs_get_child(t,2));

	tab_counter--;
	prettyBuffer("}\n");

}

void prettyClassName()
{

}


void prettyClassVarDecs(cs_node classVarDecs)
{
	int num_of_class_var_dec = cs_children(classVarDecs);
	if(num_of_class_var_dec > 0)
	{
		for(int i = 0; i < num_of_class_var_dec; i++)
		{
			prettyClassVarDec(cs_get_child(classVarDecs, i));
			if(i == num_of_class_var_dec -1)
			{
				prettyBuffer("\n");
			}
		}
	}
	
}



void prettyType(cs_node type)
{
	string unknown_type = cs_node_value(type);
	prettyBuffer(unknown_type+" ");
}

void prettyVtype(cs_node vType)
{
	string unknown_vType = cs_node_value(vType);
	prettyBuffer(unknown_vType+" ");
}


void prettySubroutineDecs(cs_node subroutineDecs)
{
	int num_of_subroutine_dec = cs_children(subroutineDecs);
	if(num_of_subroutine_dec > 0)
	{
		string keyword;
		for(int i = 0; i < num_of_subroutine_dec; i++)
		{
			keyword = cs_node_name(cs_get_child(subroutineDecs, i));
			if(keyword=="function")
			{
				prettyFunction(cs_get_child(subroutineDecs, i));
			}
			else if(keyword=="constructor")
			{
				prettyConstructor(cs_get_child(subroutineDecs, i));
			}
			else if(keyword=="method")
			{
				prettyMethod(cs_get_child(subroutineDecs, i));
			}
			else
			{
				//cout << "can not find keyword : " << keyword << endl;
				exit(0) ;
			}
			if(i != num_of_subroutine_dec - 1)
			{
				prettyBuffer("\n");
			}
		}
	}
}

void prettyConstructor(cs_node constructor)
{
	tabPrinter();
	prettyBuffer("constructor ");
	
	prettyType(cs_get_child(constructor, 0));
	prettySubroutineName(cs_get_child(constructor, 1));

	prettyBuffer("(");	
	prettyParameterList(cs_get_child(constructor, 2));
	prettyBuffer(")\n");

	tabPrinter();
	prettyBuffer("{\n");
	tab_counter++;
	prettySubroutineBody(cs_get_child(constructor, 3));
	tab_counter--;
	tabPrinter();
	prettyBuffer("}\n");
}

void prettyFunction(cs_node function)
{
	tabPrinter();
	prettyBuffer("function ");

	prettyType(cs_get_child(function, 0));
	prettySubroutineName(cs_get_child(function, 1));

	prettyBuffer("(");	
	prettyParameterList(cs_get_child(function, 2));
	prettyBuffer(")\n");

	tabPrinter();
	prettyBuffer("{\n");
	tab_counter++;
	prettySubroutineBody(cs_get_child(function, 3));
	tab_counter--;
	tabPrinter();
	prettyBuffer("}\n");
}

void prettyMethod(cs_node method)
{
	tabPrinter();
	prettyBuffer("method ");

	prettyType(cs_get_child(method, 0));
	prettySubroutineName(cs_get_child(method, 1));

	prettyBuffer("(");
	prettyParameterList(cs_get_child(method, 2));
	prettyBuffer(")\n");

	tabPrinter();
	prettyBuffer("{\n");
	tab_counter++;
	prettySubroutineBody(cs_get_child(method, 3));
	tab_counter--;
	tabPrinter();
	prettyBuffer("}\n");
}

void prettySubroutineName(cs_node funcMethCons)
{
	prettyBuffer(cs_node_value(funcMethCons));
}

void prettyParameterList(cs_node parameterList)
{
	int num_of_parameter = cs_children(parameterList);
	
	if(num_of_parameter > 0)
	{
		for(int i = 0; i < num_of_parameter; i++)
		{
			prettyParameter(cs_get_child(parameterList, i)) ;
			if(i != num_of_parameter - 1)
			{
				prettyBuffer(",");
			}
		}
	}
}

void prettyParameter(cs_node varDec)
{
	prettyType(cs_get_child(varDec, 1));
	prettyVarName(cs_get_child(varDec, 0));
}


void prettySubroutineBody(cs_node subroutineBody)
{
	//check the number of local variable 
	int num_of_var_dec = cs_children(cs_get_child(subroutineBody, 0)) ;

	//generate code for varDecs
	prettyVarDecs(cs_get_child(subroutineBody, 0));
	//generate code for statements
	prettyStatements(cs_get_child(subroutineBody, 1));

}

void prettyVarDecs(cs_node varDecs)
{
	int num_of_var_dec = cs_children(varDecs) ;
	//if there are local variables
	if(num_of_var_dec > 0)
	{
		bool have_field = false;
		for(int i = 0; i < num_of_var_dec; i++)
		{
			prettyVarDec(cs_get_child(varDecs, i));
			if(i == num_of_var_dec -1)
			{
				prettyBuffer("\n");
			}
		}
	}
}

void prettyVarDec(cs_node varDec)
{
	string variable_name = cs_node_value(cs_get_child(varDec, 0));
	string variable_type = cs_node_value(cs_get_child(varDec, 1));
	string variable_segment = cs_node_value(cs_get_child(varDec, 2));
	string variable_Offset = cs_node_value(cs_get_child(varDec, 3));
	tabPrinter();
	prettyBuffer("var "+variable_type+" "+variable_name+" ;");
	prettyBuffer("\n");
}

void prettyClassVarDec(cs_node classVarDec)
{
	string variable_name = cs_node_value(cs_get_child(classVarDec, 0));
	string variable_type = cs_node_value(cs_get_child(classVarDec, 1));
	string variable_segment = cs_node_value(cs_get_child(classVarDec, 2));
	if(variable_segment == "this")
	{
		variable_segment = "field";
	}
	string variable_Offset = cs_node_value(cs_get_child(classVarDec, 3));
	tabPrinter();
	prettyBuffer(variable_segment+" "+variable_type+" "+variable_name+" ;");
	prettyBuffer("\n");
}

void prettyStatements(cs_node statements)
{

	int num_of_statement = cs_children(statements);
	if(num_of_statement > 0)
	{
		for(int i = 0; i < num_of_statement; i++)
		{
			prettyStatement(cs_get_child(statements, i), i);
		}
	}
}

void prettyStatement(cs_node statement, int statement_number)
{
	string keyword = cs_node_name(cs_get_child(statement, 0));
	if(keyword == "letStatement")
	{
		prettyLetStatement(cs_get_child(statement, 0));
	}
	else if(keyword == "ifStatement")
	{
		prettyIfStatement(cs_get_child(statement, 0), statement_number);
	}
	else if(keyword == "whileStatement")
	{
		prettyWhileStatement(cs_get_child(statement, 0), statement_number);
	}
	else if(keyword == "doStatement")
	{
		prettyDoStatement(cs_get_child(statement, 0));
	}
	else if(keyword == "returnStatement")
	{
		prettyReturnStatement(cs_get_child(statement, 0));
	}
}

void prettyWhileStatement(cs_node whileStatement, int statement_number)
{
	int num_of_statement = cs_children(cs_parent_node(cs_parent_node(whileStatement)));
	string last_statement = cs_node_name(cs_get_child(cs_get_child(cs_parent_node(cs_parent_node(whileStatement)),num_of_statement-1),0));
	string first_statement = cs_node_name(cs_get_child(cs_get_child(cs_parent_node(cs_parent_node(whileStatement)),0),0));

	tabPrinter();
	prettyBuffer("while ");
	prettyBuffer("(");
	//check while condition
	prettyExpression(cs_get_child(whileStatement, 0));
	prettyBuffer(")\n");

	tabPrinter();
	prettyBuffer("{\n");
	tab_counter++;
	//if condition is false do this
	prettyStatements(cs_get_child(whileStatement, 1));
	tab_counter--;
	tabPrinter();
	prettyBuffer("}\n");

	
	if(statement_number != num_of_statement-1)
	{
		prettyBuffer("\n");
	}
}

void prettyIfStatement(cs_node ifStatement, int statement_number)
{
	int num_of_statement = cs_children(cs_parent_node(cs_parent_node(ifStatement)));
	string last_statement = cs_node_name(cs_get_child(cs_get_child(cs_parent_node(cs_parent_node(ifStatement)),num_of_statement-1),0));

	string first_statement = cs_node_name(cs_get_child(cs_get_child(cs_parent_node(cs_parent_node(ifStatement)),0),0));

	int num_of_else = cs_children(ifStatement)-2;

	tabPrinter();
	prettyBuffer("if ");
	prettyBuffer("(");

	//check while condition
	prettyExpression(cs_get_child(ifStatement, 0));
	prettyBuffer(")\n");

	tabPrinter();
	prettyBuffer("{\n");
	tab_counter++;
	prettyStatements(cs_get_child(ifStatement, 1));
	tab_counter--;
	tabPrinter();
	prettyBuffer("}\n");


	if(num_of_else > 0)
	{
		for(int i = 0; i < num_of_else; i++)
		{
			tabPrinter();
			prettyBuffer("else\n");

			tabPrinter();
			prettyBuffer("{\n");
			tab_counter++;
			prettyStatements(cs_get_child(ifStatement, i+2));
			tab_counter--;
			tabPrinter();
			prettyBuffer("}\n");
		}
	}

	if(statement_number != num_of_statement-1)
	{
		prettyBuffer("\n");
	}
}

void prettyLetStatement(cs_node letStatement)
{
	if(cs_node_name(cs_get_child(letStatement, 0))=="arrayIndex")
	{
		tabPrinter();
		prettyBuffer("let ");
		prettyArrayIndex(cs_get_child(letStatement, 0));
		prettyBuffer(" = ");
	}
	else
	{
		string unknown_variable_name = cs_node_value(cs_get_child(letStatement, 0)) ;
		tabPrinter();
		prettyBuffer("let "+unknown_variable_name+" = ");
	}

	prettyExpression(cs_get_child(letStatement, 1));
	prettyBuffer(" ;");
	prettyBuffer("\n");
	
}

void prettyDoStatement(cs_node  doStatement)
{
	tabPrinter();
	prettyBuffer("do ");

	string unknown_variable_name;
	string unknown_do_name;
	int num_of_doStatement = cs_children(doStatement);


	if(num_of_doStatement == 3)
	{
		if(cs_node_value(cs_get_child(doStatement, 0))=="this")
		{
			unknown_do_name = cs_node_value( cs_get_child(doStatement, 1));
			prettyBuffer(unknown_do_name);
			prettyBuffer("(");
			prettyExpressionList(cs_get_child(doStatement, 2));
			prettyBuffer(")");
		}
		else
		{
			unknown_variable_name = cs_node_value(cs_get_child(doStatement, 0));
			unknown_do_name = cs_node_value( cs_get_child(doStatement, 1));
			prettyBuffer(unknown_variable_name + "." + unknown_do_name);
			prettyBuffer("(");
			prettyExpressionList(cs_get_child(doStatement, 2));
			prettyBuffer(")");
		}
	}
	else
	{
		unknown_do_name = cs_node_value( cs_get_child(doStatement, 0));
		prettyBuffer(unknown_do_name);
		prettyBuffer("(");
		prettyExpressionList(cs_get_child(doStatement, 2));
		prettyBuffer(")");
	}
	prettyBuffer(" ;");
	prettyBuffer("\n");
}

void prettyReturnStatement(cs_node returnStatement)
{
	string check_void = cs_node_value(cs_get_child(cs_parent_node(cs_parent_node(cs_parent_node(cs_parent_node(returnStatement)))), 0));

	tabPrinter();
	prettyBuffer("return " );

	int num_of_expression = cs_children(returnStatement);
	if(num_of_expression == 1)
	{
		prettyExpression(cs_get_child(returnStatement, 0));
		prettyBuffer(" ");
	}
	else if(check_void == "void")
	{
		//write_to_buffer("push constant 0\nreturn\n");
		//cout << "push constant 0\nreturn" << endl;
	}
	else
	{
		exit(0);
	}
	prettyBuffer(";");
	prettyBuffer("\n");
}


void prettyExpression(cs_node expression)
{
	if(cs_node_name(cs_get_child(expression, 0)) == "infix")
	{
		cs_node infix = cs_get_child(expression, 0) ;
		int num_of_infix = cs_children(infix);
		prettyTerm(cs_get_child(infix, 0));
		for(int i = 1; i < num_of_infix; i+=2)
		{
			prettyInfixOp(cs_get_child(infix, i));
			prettyTerm(cs_get_child(infix, (i+1))) ;
		}
	}
	else
	{
		prettyTerm(cs_get_child(expression, 0));
	}
}

void prettyInfixOp(cs_node infix)
{
	prettyBuffer(" "+cs_node_value(infix)+" ");
}

void prettyExpressionList(cs_node expressionList)
{
	int num_of_expression = cs_children(expressionList);
	if(num_of_expression > 0)
	{
		for(int i = 0; i < num_of_expression; i++)
		{
			prettyExpression(cs_get_child(expressionList, i));
			if(i != num_of_expression -1)
			{
				prettyBuffer(",");
			}
		}
	}
}

void prettyTerm(cs_node term)
{
	if(cs_node_name(cs_get_child(term,0)) == "arrayIndex")
	{
		prettyBuffer(cs_node_value(cs_get_child(cs_get_child(term,0),0))) ;
		prettyBuffer("[");
		prettyExpression(cs_get_child(cs_get_child(term,0),1));
		prettyBuffer("]");
	}
	else if(cs_node_name(cs_get_child(term,0)) == "keywordConstant")
	{
		prettyBuffer(cs_node_value(cs_get_child(term,0)));
	}
	else if(cs_node_name(cs_get_child(term,0)) == "varName")
	{
		prettyBuffer(cs_node_value(cs_get_child(term,0))) ;
	}
	else if(cs_node_name(cs_get_child(term,0)) == "integerConstant")
	{
		prettyBuffer(cs_node_value(cs_get_child(term,0))) ;
	}
	else if(cs_node_name(cs_get_child(term,0)) == "subroutineCall")
	{
		prettySubroutineCall(cs_get_child(term,0));
	}
	else if(cs_node_name(cs_get_child(term,0)) == "stringConstant")
	{
		prettyBuffer("\""+cs_node_value(cs_get_child(term,0))+"\"") ;
	}
	else if(cs_node_name(cs_get_child(term,0)) == "expression")
	{
		prettyBuffer("(");
		prettyExpression(cs_get_child(term,0));
		prettyBuffer(")");
	}
	else if(cs_node_name(cs_get_child(term,0)) == "unary")
	{
		prettyBuffer(cs_node_value(cs_get_child(cs_get_child(term,0),0)));
		prettyTerm(cs_get_child(cs_get_child(term,0),1));
	}
}

void prettyArrayIndex(cs_node arrayIndex)
{
	prettyBuffer(cs_node_value(cs_get_child(arrayIndex,0))) ;
	prettyBuffer("[");
	prettyExpression(cs_get_child(arrayIndex,1));
	prettyBuffer("]");
}


void prettySubroutineCall(cs_node subroutineCall)
{
	string unknown_variable_name;
	string unknown_subroutine_name;
	int num_of_subroutineCall = cs_children(subroutineCall);


	if(num_of_subroutineCall == 3)
	{
		if(cs_node_value(cs_get_child(subroutineCall, 0))=="this")
		{
			unknown_subroutine_name = cs_node_value( cs_get_child(subroutineCall, 1));
			prettyBuffer(unknown_subroutine_name);
			prettyBuffer("(");
			prettyExpressionList(cs_get_child(subroutineCall, 2));
			prettyBuffer(")");
		}
		else
		{
			unknown_variable_name = cs_node_value(cs_get_child(subroutineCall, 0));
			unknown_subroutine_name = cs_node_value( cs_get_child(subroutineCall, 1));
			prettyBuffer(unknown_variable_name + "." + unknown_subroutine_name);
			prettyBuffer("(");
			prettyExpressionList(cs_get_child(subroutineCall, 2));
			prettyBuffer(")");
		}

	}
	else
	{
		unknown_subroutine_name = cs_node_value( cs_get_child(subroutineCall, 0));
		prettyBuffer(unknown_subroutine_name);
		prettyBuffer("(");
		prettyExpressionList(cs_get_child(subroutineCall, 2));
		prettyBuffer(")");
	}
}

void prettyVarName(cs_node varName)
{
	prettyBuffer(cs_node_value(varName));
}
