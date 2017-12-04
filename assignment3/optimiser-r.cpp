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

static int tab_counter = 0;


//declare all functions here
cs_node optClass(cs_node ) ;
cs_node optClassVarDecs(cs_node ) ;
cs_node optClassVarDec(cs_node) ;
cs_node optSubroutineDecs(cs_node ) ;
cs_node optSubroutineDec(cs_node ) ;
cs_node optConstructor(cs_node ) ;
cs_node optFunction(cs_node ) ;
cs_node optMethod(cs_node ) ;
cs_node optParameterList(cs_node ) ;
cs_node optParameter(cs_node ) ;
cs_node optSubroutineBody(cs_node ) ;
cs_node optVarDecs(cs_node ) ;
cs_node optVarDec(cs_node ) ;
cs_node optSubroutineName(cs_node ) ;
cs_node optVarName(cs_node ) ;
cs_node optStatements(cs_node ) ;
cs_node optStatement(cs_node ) ;
cs_node optLetStatement(cs_node ) ;
cs_node optIfStatement(cs_node ) ;
cs_node optWhileStatement(cs_node) ;
cs_node optDoStatement(cs_node ) ;
cs_node optReturnStatement(cs_node ) ;
cs_node optOffset(cs_node ) ;
cs_node optExpression(cs_node ) ;
cs_node optTerm(cs_node ) ;
cs_node optArrayIndex(cs_node ) ;
cs_node optSubroutineCall(cs_node ) ;
cs_node optSubroutineCall(cs_node ) ;
cs_node optExpressionList(cs_node ) ;
cs_node optInfixOp(cs_node ) ;
cs_node optUnaryOp(cs_node ) ;
cs_node optKeywordConstant(cs_node ) ;

// now implement the opt functions
cs_node jack_optimiser_r(cs_node t)
{
	cs_node optimised_node = optClass(t);
	return optimised_node ;
}



cs_node optClass(cs_node root)
{
	cs_node optimised_class = cs_copy(root);
	cs_node className = cs_deep_copy(cs_get_child(root, 0));
	cs_append_child(optimised_class, className);
	cs_node classVarDecs = cs_deep_copy(cs_get_child(root, 1));
	cs_append_child(optimised_class, classVarDecs);
	cs_node subroutineDecs = optSubroutineDecs(cs_get_child(root, 2));
	cs_append_child(optimised_class, subroutineDecs) ;
	return optimised_class;
}

cs_node optSubroutineDecs(cs_node subroutineDecs)
{
	int num_of_subroutine_dec = cs_children(subroutineDecs);
	cs_node optimised_subroutineDecs = cs_copy(subroutineDecs);
	if(num_of_subroutine_dec > 0)
	{
		cs_node cons_meth_func;
		for(int i = 0; i < num_of_subroutine_dec; i++)
		{
			cons_meth_func = optSubroutineDec(cs_get_child(subroutineDecs, i));
			cs_append_child(optimised_subroutineDecs, cons_meth_func) ;
		}
	}
	return optimised_subroutineDecs;
}

cs_node optSubroutineDec(cs_node subroutineDec)
{
	cs_node optimised_subroutineDec = cs_copy(subroutineDec);
	cs_node type =  cs_deep_copy(cs_get_child(subroutineDec, 0));
	cs_append_child(optimised_subroutineDec, type);
	cs_node subroutineName = cs_deep_copy(cs_get_child(subroutineDec, 1));
	cs_append_child(optimised_subroutineDec, subroutineName);
	cs_node parameterList = cs_deep_copy(cs_get_child(subroutineDec, 2));
	cs_append_child(optimised_subroutineDec, parameterList);
	cs_node subroutineBody = optSubroutineBody(cs_get_child(subroutineDec, 3));
	cs_append_child(optimised_subroutineDec, subroutineBody);
	return optimised_subroutineDec;
}



cs_node optSubroutineBody(cs_node subroutineBody)
{
	cs_node optimised_subroutineBody = cs_copy(subroutineBody);
	//generate code for varDecs
	cs_node varDecs = cs_deep_copy(cs_get_child(subroutineBody, 0));
	cs_append_child(optimised_subroutineBody, varDecs) ;
	//generate code for statements
	cs_node statements = optStatements(cs_get_child(subroutineBody, 1));
	cs_append_child(optimised_subroutineBody, statements);
	return optimised_subroutineBody;
}

cs_node optStatements(cs_node statements)
{
	cs_node optimised_statements = cs_copy(statements);
	int num_of_statement = cs_children(statements);
	if(num_of_statement > 0)
	{
		for(int i = 0; i < num_of_statement; i++)
		{
			cs_node statement = cs_copy(cs_get_child(statements,i));
			if(!(cs_node_name(cs_get_child(cs_get_child(cs_get_child(cs_get_child(cs_get_child(statements, i), 0), 0), 0),0)) == "keywordConstant" && cs_node_value(cs_get_child(cs_get_child(cs_get_child(cs_get_child(cs_get_child(statements, i), 0), 0), 0),0)) == "false"))
			{
				statement = optStatement(cs_get_child(statements, i));
				cs_append_child(optimised_statements, statement);
			}
		}
	}
	return optimised_statements;
}

cs_node optStatement(cs_node statement)
{
	cs_node optimised_statement = cs_copy(statement);
	cs_node letStatement;
	cs_node ifStatement;
	cs_node whileStatement;
	cs_node doStatement;
	cs_node returnStatement;

	string keyword = cs_node_name(cs_get_child(statement, 0));
	if(keyword == "letStatement")
	{
		letStatement = cs_deep_copy(cs_get_child(statement, 0));
		cs_append_child(optimised_statement, letStatement);
	}
	else if(keyword == "ifStatement")
	{
		//ifStatement = optIfStatement(cs_get_child(statement, 0));
		ifStatement = cs_deep_copy(cs_get_child(statement, 0));
		cs_append_child(optimised_statement, ifStatement);
	}
	else if(keyword == "whileStatement")
	{
		whileStatement = optWhileStatement(cs_get_child(statement, 0));
		cs_append_child(optimised_statement, whileStatement);
	}
	else if(keyword == "doStatement")
	{
		doStatement = cs_deep_copy(cs_get_child(statement, 0));
		cs_append_child(optimised_statement, doStatement);
	}
	else if(keyword == "returnStatement")
	{
		//returnStatement = optReturnStatement(cs_get_child(statement, 0));
		returnStatement = cs_deep_copy(cs_get_child(statement, 0));
		cs_append_child(optimised_statement, returnStatement);
	}
	return optimised_statement;
}

cs_node optWhileStatement(cs_node whileStatement)
{
	cs_node optimised_whileStatement = cs_copy(whileStatement);
	/*
	int num_of_statement = cs_children(cs_parent_node(cs_parent_node(whileStatement)));
	string last_statement = cs_node_name(cs_get_child(cs_get_child(cs_parent_node(cs_parent_node(whileStatement)),num_of_statement-1),0));
	string first_statement = cs_node_name(cs_get_child(cs_get_child(cs_parent_node(cs_parent_node(whileStatement)),0),0));
	*/

	//check while condition
	cs_node expression = cs_deep_copy(cs_get_child(whileStatement, 0));

	cs_append_child(optimised_whileStatement, expression);
	//if condition is false do this
	cs_node statements = optStatements(cs_get_child(whileStatement, 1));
	cs_append_child(optimised_whileStatement, statements);
	return optimised_whileStatement;
}

cs_node optIfStatement(cs_node ifStatement, int statement_number)
{
	int num_of_statement = cs_children(cs_parent_node(cs_parent_node(ifStatement)));
	string last_statement = cs_node_name(cs_get_child(cs_get_child(cs_parent_node(cs_parent_node(ifStatement)),num_of_statement-1),0));

	string first_statement = cs_node_name(cs_get_child(cs_get_child(cs_parent_node(cs_parent_node(ifStatement)),0),0));

	int num_of_else = cs_children(ifStatement)-2;

	//check while condition
	//optExpression(cs_get_child(ifStatement, 0));

	optStatements(cs_get_child(ifStatement, 1));


	if(num_of_else > 0)
	{
		for(int i = 0; i < num_of_else; i++)
		{

			optStatements(cs_get_child(ifStatement, i+2));

		}
	}

}

cs_node optReturnStatement(cs_node returnStatement)
{
	string check_cs_node = cs_node_value(cs_get_child(cs_parent_node(cs_parent_node(cs_parent_node(cs_parent_node(returnStatement)))), 0));


	int num_of_expression = cs_children(returnStatement);
	if(num_of_expression == 1)
	{
		//optExpression(cs_get_child(returnStatement, 0));

	}
	else if(check_cs_node == "cs_node")
	{
		//write_to_buffer("push constant 0\nreturn\n");
		//cout << "push constant 0\nreturn" << endl;
	}
	else
	{
		exit(0);
	}

}
/*
cs_node optExpressionList(cs_node expressionList)
{
	int num_of_expression = cs_children(expressionList);
	if(num_of_expression > 0)
	{
		for(int i = 0; i < num_of_expression; i++)
		{
			optExpression(cs_get_child(expressionList, i));
		}
	}
}
*/
cs_node optTerm(cs_node term)
{
	return term;
}

