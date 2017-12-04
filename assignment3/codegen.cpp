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
static int whileCounter = 0;
static int ifCounter = 0;



static bool check_variable(string identifier);



int string_to_int(string str)
{
    int n = 0;
    stringstream stoi1(str);
    stoi1 >> n; 
    return n;   
}

string int_to_string(int num)
{
	stringstream ss;
	ss << num;
	string str = ss.str();
	return str ;
}

bool firstUpper(const string& word) 
{
	return word.size() && std::isupper(word[0]); 
}

static bool checkJackClass(string jack_class_name)
{
	if(jack_class_name == ""||jack_class_name == "Memory"||jack_class_name == "Array"||
		jack_class_name == "Math"||jack_class_name == "String"||jack_class_name == "Output"||
		jack_class_name == "Screen"||jack_class_name == "Keyboard"||jack_class_name == "Sys")
	{
		return true;
	}
	else if(!firstUpper(jack_class_name))
	{
		return false;
	}
	else if(check_variable(jack_class_name))
	{
		return false;
	}
	return true;
}

static bool check_infixOp(string symbol)
{
	if(symbol == "+" || symbol == "-" || symbol == "*" || symbol == "/" || symbol == "&" || symbol == "|" || symbol == "<" || symbol == ">" || symbol == "=" )
	{
		return true ;
	}
	else
	{
		return false ;
	}
}

static bool check_unaryOp(string symbol)
{
	if(symbol == "-" || symbol == "~" )
	{
		return true ;
	}
	else
	{
		return false ;
	}
}
static bool check_keyword_constant(string symbol)
{
	if(symbol == "true" || symbol == "false" || symbol == "null" || symbol == "this")
	{
		return true ;
	}
	else
	{
		return false ;
	}
}
//////////////////////////////symbol table////////////////////////////////////////////////////////////////////////////////////
static int static_variable_counter = 0 ;
static int field_variable_counter = 0 ;
static int local_variable_counter = 0 ;
static int argument_counter = 0 ;
//////////// symbols table management so we can lookup declared  variables
static symbol_table_int symbols = create_symbol_table_int() ;
static symbol_table_int static_variable_table = create_symbol_table_int() ;
static symbol_table_int field_variable_table = create_symbol_table_int() ;
static symbol_table_int argument_table = create_symbol_table_int() ;


// this function adds an identifier to the symbol table
static void declare_variable(string identifier, int variable_counter)
{
	if ( !symbols->insert(identifier,variable_counter) )
	{
		//cout << "Variable:  " << identifier <<  " has already been declared" << endl ;
        //exit(-1) ;
	} 
}


// lookup the identifier, it is an error if it was not previously declared
static void lookup_variable(string identifier)
{
    if ( symbols->lookup(identifier) == -1 )
    {
        //cout << "Found undeclared symbol variable:  " << identifier <<  endl ;
        exit(-1) ;
    }
}

static bool check_variable(string identifier)
{
	if ( symbols->lookup(identifier) == -1 )
    {
        return false ;
    }
    else
    {
    	return true ;
    }
}


static void declare_static_variable(string identifier, int static_variable_counter)
{
	if ( !static_variable_table->insert(identifier,static_variable_counter) )
	{
		//cout << "static:  " << identifier <<  " has already been declared" << endl ;
        //exit(-1) ;
	} 
	static_variable_counter++;
}


// lookup the identifier, it is an error if it was not previously declared
static void lookup_static_variable(string identifier)
{
    if ( static_variable_table->lookup(identifier) == -1 )
    {
        //cout << "Found undeclared symbol static:  " << identifier <<  endl ;
        exit(-1) ;
    }
}

static bool check_static_variable(string identifier)
{
	if ( static_variable_table->lookup(identifier) == -1 )
    {
        return false ;
    }
    else
    {
    	return true ;
    }
}

static void declare_field_variable(string identifier, int field_variable_counter)
{
	if ( !field_variable_table->insert(identifier,field_variable_counter) )
	{
		//cout << "field:  " << identifier <<  " has already been declared" << endl ;
        //exit(-1) ;
	} 
	field_variable_counter++;
}


// lookup the identifier, it is an error if it was not previously declared
static void lookup_field_variable(string identifier)
{
    if ( field_variable_table->lookup(identifier) == -1 )
    {
        //cout << "Found undeclared symbol field:  " << identifier <<  endl ;
        exit(-1) ;
    }
}

static bool check_field_variable(string identifier)
{
	if ( static_variable_table->lookup(identifier) == -1 )
    {
        return false ;
    }
    else
    {
    	return true ;
    }
}

void genClass(cs_node ) ;
void genClassVarDecs(cs_node ) ;
void genStaticVarDec(cs_node ) ;
void genFieldVarDec(cs_node ) ;
void genType(cs_node ) ;
void genVtype(cs_node ) ;
void genSubroutineDecs(cs_node ) ;
void genConstructor(cs_node ) ;
void genFunction(cs_node ) ;
void genMethod(cs_node ) ;
void genParameterList(cs_node ) ;
void genParameter(cs_node ) ;
void genSubroutineBody(cs_node ) ;
void genVarDecs(cs_node ) ;
void genVarDec(cs_node ) ;
void genClassName() ;
void genSubroutineName(cs_node ) ;
void genVarName(cs_node ) ;



void genStatements(cs_node ) ;
void genStatement(cs_node ) ;

void genLetStatement(cs_node ) ;
void genIfStatement(cs_node ) ;
void genWhileStatement(cs_node ) ;
void genDoStatement(cs_node ) ;
void genReturnStatement(cs_node ) ;

void genOffset(cs_node ) ;



void genExpression(cs_node ) ;
void genTerm(cs_node ) ;
void genArrayIndex(cs_node ) ;
void genSubroutineCall(cs_node ) ;
void genSubroutineCall(cs_node ) ;
void genExpressionList(cs_node ) ;
void genInfixOp(cs_node ) ;
void genUnaryOp(cs_node ) ;
void genKeywordConstant(cs_node ) ;

// now implement the parsing functions
void genClass(cs_node t)
{
	className = cs_node_value(cs_get_child(t,0));
	genClassVarDecs(cs_get_child(t,1));
	genSubroutineDecs(cs_get_child(t,2));


}

void genClassName()
{
	write_to_buffer(className) ;
	cout << className;
}


void genClassVarDecs(cs_node classVarDecs)
{
	int num_of_class_var_dec = cs_children(classVarDecs);
	if(num_of_class_var_dec > 0)
	{
		for(int i = 0; i < num_of_class_var_dec; i++)
		{
			genVarDec(cs_get_child(classVarDecs, i));
		}
	}
	
}



void genType(cs_node type)
{

}

void genVtype(cs_node vType)
{

}


void genSubroutineDecs(cs_node subroutineDecs)
{
	int num_of_subroutine_dec = cs_children(subroutineDecs);
	if(num_of_subroutine_dec > 0)
	{
		string keyword = cs_node_name(cs_get_child(subroutineDecs, 0));
		for(int i = 0; i < num_of_subroutine_dec; i++)
		{
			if(keyword=="function")
			{
				genFunction(cs_get_child(subroutineDecs, i));
			}
			else if(keyword=="constructor")
			{
				genConstructor(cs_get_child(subroutineDecs, i));
			}
			else if(keyword=="method")
			{
				genMethod(cs_get_child(subroutineDecs, i));
			}
			else
			{
				cout << "can not find keyword : " << keyword << endl;
				exit(0) ;
			}
		}
	}
}

void genConstructor(cs_node constructor)
{
	write_to_buffer("function " + className + ".");
	cout << "function " + className + ".";


	genType(cs_get_child(constructor, 0));
	genSubroutineName(cs_get_child(constructor, 1));
	genParameterList(cs_get_child(constructor, 2));

	genSubroutineBody(cs_get_child(constructor, 3));
}

void genFunction(cs_node function)
{
	write_to_buffer("function " + className + ".");
	cout << "function " + className + ".";

	genType(cs_get_child(function, 0));
	genSubroutineName(cs_get_child(function, 1));
	genParameterList(cs_get_child(function, 2));

	genSubroutineBody(cs_get_child(function, 3));
}

void genMethod(cs_node method)
{
	write_to_buffer("function " + className + ".");
	cout << "function " + className + ".";

	genType(cs_get_child(method, 0));
	genSubroutineName(cs_get_child(method, 1));
	genParameterList(cs_get_child(method, 2));

	genSubroutineBody(cs_get_child(method, 3));
}

void genSubroutineName(cs_node funcMethCons)
{
	write_to_buffer(cs_node_value(funcMethCons)+" ");
	cout << cs_node_value(funcMethCons)+" ";
}

void genParameterList(cs_node parameterList)
{
	int num_of_parameter = cs_children(parameterList);
	
	if(num_of_parameter > 0)
	{
		for(int i = 0; i < num_of_parameter; i++)
		{
			genParameter(cs_get_child(parameterList, i)) ;

		}
	}
}

void genParameter(cs_node parameter)
{
	genType(cs_get_child(parameter, 0));
	genVarName(cs_get_child(parameter, 1));
}


void genSubroutineBody(cs_node subroutineBody)
{
	//check the number of local variable 
	int num_of_var_dec = cs_children(cs_get_child(subroutineBody, 0)) ;
	//print it out
	write_to_buffer(int_to_string(num_of_var_dec) );
	cout << int_to_string(num_of_var_dec);

	//change line
	write_to_buffer("\n");
	cout<< endl;

	//generate code for varDecs
	genVarDecs(cs_get_child(subroutineBody, 0));
	//generate code for statements
	genStatements(cs_get_child(subroutineBody, 1));
}

void genVarDecs(cs_node varDecs)
{
	int num_of_var_dec = cs_children(varDecs) ;
	//if there are local variables
	if(num_of_var_dec > 0)
	{
		bool have_field = false;
		for(int i = 0; i < num_of_var_dec; i++)
		{
			genVarDec(cs_get_child(varDecs, i));
			if(cs_node_name(cs_get_child(cs_get_child(varDecs, i), 2)) == "field")
			{
				have_field = true;
			}
		}
		/*
		if(have_field)
		{
			write_to_buffer("push constant " + int_to_string(field_variable_counter) + "\n");
			write_to_buffer("call Memory.alloc 1\n");
			write_to_buffer("pop pointer 0\n")
			cout<<  ;
			cout<<  ;
			cout<<  ;
		}
		*/
	}
}

void genVarDec(cs_node varDec)
{
	string variable_name = cs_node_value(cs_get_child(varDec, 0));
	string variable_type = cs_node_value(cs_get_child(varDec, 1));
	string variable_segment = cs_node_value(cs_get_child(varDec, 2));
	string variable_Offset = cs_node_value(cs_get_child(varDec, 3));

	if(variable_segment == "static")
	{
		declare_static_variable(variable_name, string_to_int(variable_Offset));
	}
	else if(variable_segment == "field")
	{
		declare_field_variable(variable_name, string_to_int(variable_Offset));
	}
}

void genStatements(cs_node statements)
{

	int num_of_statement = cs_children(statements);
	if(num_of_statement > 0)
	{
		for(int i = 0; i < num_of_statement; i++)
		{
			genStatement(cs_get_child(statements, i));
		}
	}
}

void genStatement(cs_node statement)
{
	string keyword = cs_node_name(cs_get_child(statement, 0));
	if(keyword == "letStatement")
	{
		genLetStatement(cs_get_child(statement, 0));
	}
	else if(keyword == "ifStatement")
	{
		genIfStatement(cs_get_child(statement, 0));
	}
	else if(keyword == "whileStatement")
	{
		genWhileStatement(cs_get_child(statement, 0));
	}
	else if(keyword == "doStatement")
	{
		genDoStatement(cs_get_child(statement, 0));
	}
	else if(keyword == "returnStatement")
	{
		genReturnStatement(cs_get_child(statement, 0));
	}
	else
	{
		cout << "can not find statement : " << keyword << endl;
		exit(0);
	}
}



void genWhileStatement(cs_node whileStatement)
{
	int whileCounterHolder = whileCounter;

	//creat WHILE_EXP label
	write_to_buffer("label WHILE_EXP" + int_to_string(whileCounter)+ "\n" );
	cout << "label WHILE_EXP" + int_to_string(whileCounter) + "\n";

	//check while condition
	genExpression(cs_get_child(whileStatement, 0));


	//if the condition is true
	write_to_buffer("not\nif-goto WHILE_END" + int_to_string(whileCounter)+ "\n");
	cout << "not\nif-goto WHILE_END" + int_to_string(whileCounter)+ "\n";

	whileCounter++;

	//if condition is false do this
	genStatements(cs_get_child(whileStatement, 1));



	//go back to the top
	write_to_buffer("goto WHILE_EXP" + int_to_string(whileCounterHolder)+ "\n");
	cout << "goto WHILE_EXP" + int_to_string(whileCounterHolder)+ "\n";

	//add label to the end
	write_to_buffer("label WHILE_END" + int_to_string(whileCounterHolder)+ "\n" );
	cout << "label WHILE_END" + int_to_string(whileCounterHolder) + "\n";
}

void genIfStatement(cs_node ifStatement)
{
	int num_of_else = cs_children(ifStatement)-2;

	int ifCounterHolder = ifCounter;

	//check while condition
	genExpression(cs_get_child(ifStatement, 0));


	//if the condition is true
	write_to_buffer("if-goto IF_TRUE" + int_to_string(ifCounter)+ "\n");
	cout << "if-goto IF_TRUE" + int_to_string(ifCounter)+ "\n";
	//if condition is true do this

	
	write_to_buffer("goto IF_FALSE" + int_to_string(ifCounter)+ "\n");
	cout << "goto IF_FALSE" + int_to_string(ifCounter)+ "\n";


	ifCounter++;

	//creat IF_TRUE label
	write_to_buffer("label IF_TRUE" + int_to_string(ifCounterHolder)+ "\n" );
	cout << "label IF_TRUE" + int_to_string(ifCounterHolder) + "\n";

	//if condition is true do this
	genStatements(cs_get_child(ifStatement, 1));

	if(num_of_else == 1)
	{
	//then goto end
	write_to_buffer("goto IF_END" + int_to_string(ifCounterHolder)+ "\n");
	cout << "goto IF_END" + int_to_string(ifCounterHolder)+ "\n";
	}

	//creat IF_FALSE label
	write_to_buffer("label IF_FALSE" + int_to_string(ifCounterHolder)+ "\n" );
	cout << "label IF_FALSE" + int_to_string(ifCounterHolder) + "\n";
	//if have else and if condition is false
	if(num_of_else == 1)
	{
		//if condition is false do this
		genStatements(cs_get_child(ifStatement, 2));
	
		//add label to the end
		write_to_buffer("label IF_END" + int_to_string(ifCounterHolder)+ "\n" );
		cout << "label IF_END" + int_to_string(ifCounterHolder) + "\n";
	}
}

void genLetStatement(cs_node letStatement)
{
	string unknown_variable_name = cs_node_value(cs_get_child(letStatement, 0)) ;
	genExpression(cs_get_child(letStatement, 1));

	// use pop to store the result to a place
	if(check_static_variable(unknown_variable_name))
	{
		write_to_buffer("pop static " + int_to_string(static_variable_table->lookup(unknown_variable_name)) + "\n");
		cout << "pop static " + int_to_string(static_variable_table->lookup(unknown_variable_name)) + "\n" ;
	}
}

void genDoStatement(cs_node  doStatement)
{

}

void genReturnStatement(cs_node returnStatement)
{
	string check_void = cs_node_value(cs_get_child(cs_parent_node(cs_parent_node(cs_parent_node(cs_parent_node(returnStatement)))), 0));

	int num_of_expression = cs_children(returnStatement);
	if(num_of_expression == 1)
	{
		genExpression(cs_get_child(returnStatement, 0));
		write_to_buffer("return\n");
		cout << "return" << endl;
	}
	else if(check_void == "void")
	{
		write_to_buffer("push constant 0\nreturn\n");
		cout << "push constant 0\nreturn" << endl;
	}
	else
	{
		exit(0);
	}
	
}


void genExpression(cs_node expression)
{
	if(cs_node_name(cs_get_child(expression, 0)) == "infix")
	{
		cs_node infix = cs_get_child(expression, 0) ;
		int num_of_infix = cs_children(infix);
		genTerm(cs_get_child(infix, 0));
		for(int i = 1; i < num_of_infix; i+=2)
		{
			genTerm(cs_get_child(infix, (i+1))) ;
			genInfixOp(cs_get_child(infix, i));
		}
	}
	else
	{
		genTerm(cs_get_child(expression, 0));
	}
}

void genInfixOp(cs_node infix)
{
	if(cs_node_value(infix) == "+")
	{
		write_to_buffer("add\n");
		cout << "add\n" ;
	}
	
}

void genExpressionList(cs_node expressionList)
{

}

void genTerm(cs_node term)
{
	if(cs_node_name(cs_get_child(term,0)) == "keywordConstant")
	{
		if(cs_node_value(cs_get_child(term,0)) == "true")
		{
			write_to_buffer("push constant 0\nnot\n");
			cout << "push constant 0\nnot\n";
		}
		else if(cs_node_value(cs_get_child(term,0)) == "false")
		{
			write_to_buffer("push constant 0\n");
			cout << "push constant 0\n";
		}
	}
	else if(cs_node_name(cs_get_child(term,0)) == "varName")
	{
		string unknown_variable_name = cs_node_value(cs_get_child(term,0));
		if(check_static_variable(unknown_variable_name))
		{
			write_to_buffer("push static " + int_to_string(static_variable_table->lookup(unknown_variable_name)) + "\n");
			cout << "push static " + int_to_string(static_variable_table->lookup(unknown_variable_name)) + "\n" ;
		}
		/*
		else if(check_field_variable(unknown_variable_name))
		{
			write_to_buffer("push static " + int_to_string(static_variable_table->lookup(unknown_variable_name)) + "\n");
			cout << "push static " + int_to_string(static_variable_table->lookup(unknown_variable_name)) + "\n" ;
		}
		*/
	}
	else if(cs_node_name(cs_get_child(term,0)) == "integerConstant")
	{
		write_to_buffer("push constant " + cs_node_value(cs_get_child(term,0)) + "\n");
		cout << "push constant " + cs_node_value(cs_get_child(term,0)) + "\n";
	}
	else if(cs_node_name(cs_get_child(term,0)) == "subroutineCall")
	{
		genSubroutineCall(cs_get_child(term,0));
	}
}


void genSubroutineCall(cs_node subroutineCall)
{
	string unknown_variable_name;
	string unknown_subroutine_name;
	int num_of_subroutineCall = cs_children(subroutineCall);

	write_to_buffer("call ");
	cout << "call ";

	if(num_of_subroutineCall == 3)
	{
		unknown_variable_name = cs_node_value(cs_get_child(subroutineCall, 0));
		unknown_subroutine_name = cs_node_value( cs_get_child(subroutineCall, 1));
		write_to_buffer(unknown_variable_name+"."+unknown_subroutine_name+" 0\n");
		cout << unknown_variable_name+"."+unknown_subroutine_name+" 0\n";
	}
}

void genVarName(cs_node varName)
{

}




//parameterList and Array is not done yet
//cout << "kokokokokokokokokokokokokokokokokokokokokkokokokokokookokokokokokokokokokoko " << endl;


void jack_codegen(cs_node t)
{                
	genClass(t);

    //cout << "\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";


}
