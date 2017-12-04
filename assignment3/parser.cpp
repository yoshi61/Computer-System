#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <sstream>
#include <iterator>
#include "tokeniser.h"
#include "symbols.h"
#include "csdocument.h"

using namespace std ;

// the last token read - start with ?
static string token = "?" ;
static string tokenclass = "?" ;
static string tokenvalue = "?" ;
static cs_node token_node ;   // a document node for the last token read
static string value_holder = "" ;
static string className = "" ;




static bool check_variable(string identifier);





// useful parsing functions
static void nextToken()
{
	value_holder = tokenvalue ;
    token = next_token() ;
    tokenclass = token_class() ;
    tokenvalue = token_value() ;
}

// if we have the expected token - move to next token otherwise give up!
void mustbe(string expected)
{
    if ( expected != token )
    {
        //cout << "Error: found token \"" << token << "\" but expected \"" << expected << "\"" << endl ;
        exit(-1) ;
    }
    nextToken() ;
}

// if we have the expected token - move to next token and return true
bool have(string expected)
{
    if ( expected != token ) return false ;
    nextToken() ;
    return true ;
}


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
static symbol_table_int subroutine_name_table = create_symbol_table_int() ;
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

// this function adds an identifier to the argument_table
static void declare_argument(string identifier, int variable_counter)
{
	if ( !argument_table->insert(identifier,variable_counter) ){} 
	symbols->insert(identifier, 0) ;
}


// lookup the identifier, it is an error if it was not previously declared
static void lookup_argument(string identifier)
{
    if ( argument_table->lookup(identifier) == -1 )
    {
        cout << "Found undeclared argument:  " << identifier <<  endl ;
        exit(-1) ;
    }
}

static bool check_argument(string identifier)
{
	if ( argument_table->lookup(identifier) == -1 )
    {
        return false ;
    }
    else
    {
    	return true ;
    }
}


//////////////
static void declare_subroutine(string identifier)
{
	subroutine_name_table->insert(identifier, 0) ;
}

static void lookup_subroutine(string identifier)
{
    if ( subroutine_name_table->lookup(identifier) == -1 )
    {
        declare_subroutine(identifier) ;
    }
}

static bool check_subroutine(string identifier)
{
	if ( subroutine_name_table->lookup(identifier) == -1 )
    {
        return false ;
    }
    else
    {
    	return true ;
    }
}

cs_node parseClass() ;
cs_node parseClassVarDecs() ;
cs_node parseStaticVarDec() ;
cs_node parseFieldVarDec() ;
cs_node parseType() ;
cs_node parseVtype() ;
cs_node parseSubroutineDecs() ;
cs_node parseConstructor() ;
cs_node parseFunction() ;
cs_node parseMethod() ;
cs_node parseParameterList() ;
cs_node parseSubroutineBody() ;
cs_node parseVarDecs() ;
cs_node parseVarDec() ;
cs_node parseClassName() ;
cs_node parseSubroutineName() ;
cs_node parseVarName() ;
cs_node parseStatements() ;
cs_node parseStatement() ;
cs_node parseLetStatement() ;
cs_node parseIfStatement() ;
cs_node parseWhileStatement() ;
cs_node parseDoStatement() ;
cs_node parseReturnStatement() ;
cs_node parseOffset() ;
cs_node parseExpression() ;
cs_node parseTerm() ;
cs_node parseArrayIndex() ;
cs_node parseSubroutineCall() ;
cs_node parseSubroutineCall(cs_node varName) ;
cs_node parseExpressionList() ;
cs_node parseInfixOp() ;
cs_node parseUnaryOp() ;
cs_node parseKeywordConstant() ;

// now implement the parsing functions
cs_node parseClass()
{
	mustbe("class") ;
	cs_node newClass = cs_new_node("class") ;
	mustbe("identifier") ;
	className = value_holder ;
	cs_node className_node = cs_new_node_text("className", className) ;
	cs_append_child(newClass, className_node) ;
	mustbe("{") ;
	cs_append_child(newClass, parseClassVarDecs()) ;
	cs_append_child(newClass, parseSubroutineDecs()) ;
	mustbe("}") ;
	token = "?";

    return newClass ;
}


cs_node parseClassVarDecs()
{
	cs_node classVarDecs = cs_new_node("classVarDecs") ;
	cs_node varDec = cs_new_node("varDec") ;
	while(have("static") || have("field"))
	{
		if(value_holder == "static")
		{
			varDec = parseStaticVarDec() ; 
			cs_append_child(classVarDecs, varDec) ;
			while(have(","))
			{
				cs_node more_varDec = cs_new_node("varDec") ;
				cs_node varName ;
				mustbe("identifier") ;
				declare_variable(value_holder, static_variable_counter) ;
				varName = cs_new_node_text("varName", value_holder) ;
				cs_node Offset = cs_new_node_text("Offset", int_to_string(static_variable_counter)) ;
				static_variable_counter++ ;
				cs_append_child(more_varDec, varName) ;
				cs_append_child(more_varDec, cs_deep_copy( cs_get_child(varDec,1) ) ) ;
				cs_append_child(more_varDec, cs_deep_copy( cs_get_child(varDec,2) ) );
				cs_append_child(more_varDec, Offset) ;

				cs_append_child(classVarDecs, more_varDec) ;
			}
		}
		else
		{
			varDec = parseFieldVarDec() ;
			cs_append_child(classVarDecs, varDec) ;
			while(have(","))
			{
				cs_node more_varDec = cs_new_node("varDec") ;
				cs_node varName ;
				mustbe("identifier") ;
				declare_variable(value_holder, field_variable_counter) ;
				varName = cs_new_node_text("varName", value_holder) ;
				cs_node Offset = cs_new_node_text("Offset", int_to_string(field_variable_counter)) ;
				field_variable_counter++ ;
				cs_append_child(more_varDec, varName) ;
				cs_append_child(more_varDec, cs_deep_copy( cs_get_child(varDec,1) ) ) ;
				cs_append_child(more_varDec, cs_deep_copy( cs_get_child(varDec,2) ) );
				cs_append_child(more_varDec, Offset) ;

				cs_append_child(classVarDecs, more_varDec) ;
			}
		}
		mustbe(";") ;
	}
	static_variable_counter = 0 ;
	field_variable_counter = 0 ;
	return classVarDecs ;
}



cs_node parseStaticVarDec()
{
	cs_node varDec = cs_new_node("varDec") ;
	cs_node segment = cs_new_node_text("Segment", "static") ;
	cs_node type = parseType() ;
	cs_node varName ;
	mustbe("identifier") ;
	declare_variable(value_holder, static_variable_counter) ;
	varName = cs_new_node_text("varName", value_holder) ;
	cs_node Offset = cs_new_node_text("Offset", int_to_string(static_variable_counter)) ;
	static_variable_counter++ ;
	cs_append_child(varDec, varName) ;
	cs_append_child(varDec, type) ;
	cs_append_child(varDec, segment) ;
	cs_append_child(varDec, Offset) ;

	return varDec ;

}

cs_node parseFieldVarDec()
{
	cs_node varDec = cs_new_node("varDec") ;
	cs_node segment = cs_new_node_text("Segment", "this") ;
	cs_node type = parseType() ;
	cs_node varName ;
	mustbe("identifier") ;
	declare_variable(value_holder, field_variable_counter) ;
	varName = cs_new_node_text("varName", value_holder) ;
	cs_node Offset = cs_new_node_text("Offset", int_to_string(field_variable_counter)) ;
	field_variable_counter++ ;
	cs_append_child(varDec, varName) ;
	cs_append_child(varDec, type) ;
	cs_append_child(varDec, segment) ;
	cs_append_child(varDec, Offset) ;
	return varDec ;
}

cs_node parseType()
{
	cs_node type ;
    if ( have("int") )
    {
    	type = cs_new_node_text("type", "int") ;
    }  
    else if ( have("char") )
    {
    	type = cs_new_node_text("type", "char") ;
    }
    else if( have("boolean") )
    {
    	type = cs_new_node_text("type", "boolean") ;
    }
    else
    {
	    if(tokenvalue == className)
	    {
	    	type = cs_new_node_text("type", className) ;
	    	nextToken() ;
	    }
	    else if(tokenvalue == "Array")
	    {
	    	type = cs_new_node_text("type", "Array") ;
	    	nextToken() ;
	    }
	    else
	    {
	    	type = cs_new_node_text("type", tokenvalue) ;
	    	nextToken() ;
	    }
    }
    return type;
}

cs_node parseVtype()
{
	cs_node type ;
    if ( have("int") )
    {
    	type = cs_new_node_text("type", "int") ;
    }  
    else if ( have("char") )
    {
    	type = cs_new_node_text("type", "char") ;
    }
    else if ( have("boolean") )
    {
    	type = cs_new_node_text("type", "boolean") ;
    }
    else if ( have("void") )
    {
    	type = cs_new_node_text("type", "void") ;
    }
    else
    {
	    if(tokenvalue == className)
	    {
	    	type = cs_new_node_text("type", className) ;
	    	nextToken() ;
	    }
	    else if(tokenvalue == "Array")
	    {
	    	type = cs_new_node_text("type", "Array") ;
	    	nextToken() ;
	    }
	    else
	    {
	    	type = cs_new_node_text("type", tokenvalue) ;
	    	nextToken() ;
	    }
    }

    return type;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

cs_node parseSubroutineDecs()
{
	cs_node subroutineDecs = cs_new_node("subroutineDecs") ;
	cs_node constructor ;
	cs_node function ;
	cs_node method ;
	while(have("constructor") || have("function") || have("method"))
	{
		if(value_holder == "constructor")
		{
			constructor = parseConstructor() ;
			cs_append_child(subroutineDecs, constructor) ;
			
		}
		else if(value_holder == "function")
		{
			function = parseFunction() ;
			cs_append_child(subroutineDecs, function) ;
		}
		else
		{
			method = parseMethod() ;
			cs_append_child(subroutineDecs, method) ;
		}
	}
	return subroutineDecs ;
}

cs_node parseConstructor()
{
	cs_node constructor = cs_new_node("constructor") ;
	cs_node type ;
	mustbe("identifier") ;
    if(value_holder == className)
    {
    	type = cs_new_node_text("type", className) ;
    }
    else
    {
    	cout << "unknown type " << value_holder << endl;
    	exit(-1) ;
    }
	mustbe("identifier") ;
	declare_subroutine(value_holder) ;
	cs_node subroutineName = cs_new_node_text("subroutineName" , value_holder) ;
	mustbe("(") ;
	cs_node parameterList = cs_new_node("parameterList") ;
	if(tokenvalue != ")")
	{
		parameterList = parseParameterList() ;
	}
	mustbe(")")	;
	cs_node subroutineBody = parseSubroutineBody() ;
	cs_append_child(constructor, type) ; 
	cs_append_child(constructor, subroutineName) ;
	cs_append_child(constructor, parameterList) ;
	cs_append_child(constructor, subroutineBody) ;
	argument_counter = 0 ;
	return constructor ;
}

cs_node parseFunction()
{
	argument_counter = 0 ;
	cs_node function = cs_new_node("function") ;
	cs_node type = parseVtype() ;
	mustbe("identifier") ;
	declare_subroutine(value_holder) ;
	cs_node subroutineName = cs_new_node_text("subroutineName" , value_holder) ;
	mustbe("(") ;
	cs_node parameterList = cs_new_node("parameterList") ;
	if(tokenvalue != ")")
	{
		parameterList = parseParameterList() ;
	}
	mustbe(")")	;
	cs_node subroutineBody = parseSubroutineBody() ;
	cs_append_child(function, type) ; 
	cs_append_child(function, subroutineName) ;
	cs_append_child(function, parameterList) ;
	cs_append_child(function, subroutineBody) ;
	argument_counter = 0 ;
	return function ;
}


cs_node parseMethod()
{
	argument_counter = 1 ;
	cs_node method = cs_new_node("method") ;
	cs_node type = parseVtype() ;
	mustbe("identifier") ;
	declare_subroutine(value_holder) ;
	cs_node subroutineName = cs_new_node_text("subroutineName" , value_holder) ;
	mustbe("(") ;
	cs_node parameterList = cs_new_node("parameterList") ;
	if(tokenvalue != ")")
	{
		parameterList = parseParameterList() ;
	}
	mustbe(")")	;
	cs_node subroutineBody = parseSubroutineBody() ;
	cs_append_child(method, type) ; 
	cs_append_child(method, subroutineName) ;
	cs_append_child(method, parameterList) ;
	cs_append_child(method, subroutineBody) ;
	argument_counter = 0 ;
	return method ;
}


cs_node parseParameterList()
{
	cs_node parameterList = cs_new_node("parameterList") ;
	moreArgument:	
	cs_node varDec = cs_new_node("varDec") ;
	cs_node segment = cs_new_node_text("Segment", "argument") ;
	cs_node varName ;
	cs_node type ;
	cs_node Offset ;
	type = parseType() ;
	mustbe("identifier") ;
	declare_argument(value_holder, argument_counter) ;
	varName = cs_new_node_text("varName", value_holder) ;
	Offset = cs_new_node_text("Offset", int_to_string(argument_counter)) ;
	argument_counter++ ;

	cs_append_child(varDec, varName) ;
	cs_append_child(varDec, type) ;
	cs_append_child(varDec, segment) ;
	cs_append_child(varDec, Offset) ;
	cs_append_child(parameterList, varDec) ;

	if(have(","))
	{
		goto moreArgument ;
	}
	return parameterList ;
}



cs_node parseSubroutineBody()
{
	cs_node subroutineBody = cs_new_node("subroutineBody") ;
	mustbe("{") ;
	cs_node varDecs = parseVarDecs() ;
	cs_node statements = parseStatements() ;
	mustbe("}") ;
	cs_append_child(subroutineBody, varDecs) ;
	cs_append_child(subroutineBody, statements) ;
	return subroutineBody ;
}

cs_node parseVarDecs()
{
	cs_node varDecs = cs_new_node("varDecs") ;
	cs_node varDec ;
	while(have("var"))
	{
		varDec = parseVarDec() ; 
		cs_append_child(varDecs, varDec) ;
		while(have(","))
		{
			cs_node more_varDec = cs_new_node("varDec") ;
			cs_node varName ;
			mustbe("identifier") ;
			declare_variable(value_holder, local_variable_counter) ;
			varName = cs_new_node_text("varName", value_holder) ;
			cs_node Offset = cs_new_node_text("Offset", int_to_string(local_variable_counter)) ;
			local_variable_counter++ ;
			cs_append_child(more_varDec, varName) ;
			cs_append_child(more_varDec, cs_deep_copy( cs_get_child(varDec,1) ) ) ;
			cs_append_child(more_varDec, cs_deep_copy( cs_get_child(varDec,2) ) );
			cs_append_child(more_varDec, Offset) ;

			cs_append_child(varDecs, more_varDec) ;
		}
		mustbe(";");
	}
	local_variable_counter = 0 ;
	return varDecs ;
}

cs_node parseVarDec()
{
	cs_node varDec = cs_new_node("varDec") ;
	cs_node segment = cs_new_node_text("Segment", "local") ;
	cs_node type = parseType() ;
	cs_node varName ;
	mustbe("identifier") ;
	declare_variable(value_holder, local_variable_counter) ;
	varName = cs_new_node_text("varName", value_holder) ;
	cs_node Offset = cs_new_node_text("Offset", int_to_string(local_variable_counter)) ;
	local_variable_counter++ ;

	cs_append_child(varDec, varName) ;
	cs_append_child(varDec, type) ;
	cs_append_child(varDec, segment) ;
	cs_append_child(varDec, Offset) ;

	return varDec ;
}

cs_node parseStatements()
{
	cs_node statements = cs_new_node("statements") ;
	while ( token != "}" && token != "?" )
    {
    	cs_node statement;
        statement = parseStatement() ;
        cs_append_child(statements, statement) ;
    }
    return statements ;
}

cs_node parseStatement()
{
	cs_node statement = cs_new_node("statement");
	if ( have("while") ) 
	{
		cs_node whileStatement ;
		whileStatement = parseWhileStatement() ; 
		cs_append_child(statement, whileStatement) ;
	}
	else if ( have("if") ) 
	{
		cs_node ifStatement ;
		ifStatement = parseIfStatement() ; 
		cs_append_child(statement, ifStatement) ;
	}
	else if ( have("let") ) 
	{
		cs_node letStatement ;
		letStatement = parseLetStatement() ; 
		cs_append_child(statement, letStatement) ;
	}
	else if ( have("do") ) 
	{
		cs_node doStatement ;
		doStatement = parseDoStatement() ;
		cs_append_child(statement, doStatement) ; 
	}	
	else
	{
		mustbe("return") ;
		cs_node returnStatement ;
		returnStatement = parseReturnStatement() ;
		cs_append_child(statement, returnStatement) ;
	}
    return statement ;
}



cs_node parseWhileStatement()
{
	cs_node whileStatement = cs_new_node("whileStatement") ;
    mustbe("(") ;
    cs_node expression = parseExpression() ;
    mustbe(")") ;
    mustbe("{") ;
    cs_node statements = parseStatements() ;
    mustbe("}") ;
    cs_append_child(whileStatement, expression) ;
    cs_append_child(whileStatement, statements) ;
    return whileStatement ;
}

cs_node parseIfStatement()
{
	cs_node ifStatement = cs_new_node("ifStatement") ;
    mustbe("(") ;
    cs_node expression = parseExpression() ;
    mustbe(")") ;
    cs_append_child(ifStatement, expression) ;
    mustbe("{") ;
    cs_node statements = parseStatements() ;
    mustbe("}") ;
    cs_append_child(ifStatement, statements) ;
    while ( have("else") )
    {
    	mustbe("{") ;
        cs_node statements = parseStatements() ;
        mustbe("}") ;
        cs_append_child(ifStatement, statements) ;

    }
    return ifStatement ;
}

cs_node parseLetStatement()
{
    // first create a new document node for a letStatement
    cs_node letStatement = cs_new_node("letStatement") ;

    // save token value - it must be an identifier
   

    // if we get here it really was an identifier
    // check if it has been declared
    lookup_variable(tokenvalue) ;

    mustbe("identifier") ;
	// make a new document node for the identifier
	cs_node varName = cs_new_node_text("varName",value_holder) ;

    if(have("["))
    {
    	cs_node arrayIndex = cs_new_node("arrayIndex") ;
    	cs_node expression = parseExpression() ;
    	cs_append_child(arrayIndex, varName) ;
    	cs_append_child(arrayIndex, expression) ;
    	cs_append_child(letStatement, arrayIndex) ;
    	mustbe("]") ;
    }
    else
    {
	    // then we make the identifier the first child node of the letStatement node
	    cs_append_child(letStatement, varName) ;
    }
    

    mustbe("=") ;
    // the call to parseExpression will construct a document tree representation of the expression
    cs_node expression = parseExpression() ;
    mustbe(";") ;

    // add the expression's document tree as the second child of the letstatement
    cs_append_child(letStatement, expression) ;

    // finally return the letStatement node
    return letStatement ;
}

cs_node parseDoStatement()
{
	cs_node doStatement = cs_new_node("doStatement") ;
	if(check_variable(tokenvalue))
	{
		cs_node varName = cs_new_node_text("varName" , tokenvalue) ;
		mustbe("identifier") ;
		cs_append_child(doStatement, varName) ;
		mustbe(".") ;
	}
	else if(tokenvalue == className|| checkJackClass(tokenvalue))
	{
		cs_node className_node = cs_new_node_text("className" , tokenvalue) ;
		mustbe("identifier") ;
		cs_append_child(doStatement, className_node) ;
		mustbe(".") ;
	}
	else
	{
		cs_node keywordConstant = cs_new_node_text("keywordConstant", "this") ;
		cs_append_child(doStatement, keywordConstant);
	}
	mustbe("identifier") ;
	lookup_subroutine(value_holder) ;
	cs_node subroutineName = cs_new_node_text("subroutineName" , value_holder) ;
	cs_append_child(doStatement, subroutineName) ;
	mustbe("(") ;
	cs_node expressionList =  parseExpressionList() ;
	cs_append_child(doStatement, expressionList) ;
	mustbe(")") ;
	mustbe(";") ;
	return doStatement ;
}

cs_node parseReturnStatement()
{
	cs_node returnStatement = cs_new_node("returnStatement") ;
	if(tokenvalue != ";")
	{
		cs_node expression = parseExpression() ;
		cs_append_child(returnStatement, expression) ;
	}
	mustbe(";") ;
	return returnStatement ;
}


cs_node parseExpression()
{
	cs_node expression = cs_new_node("expression") ;
	cs_node term = parseTerm() ;
	if(check_infixOp(tokenvalue))
	{
		cs_node infix = cs_new_node("infix") ;
		cs_append_child(infix, term) ;
		while( check_infixOp(tokenvalue) )
	    {
	    	cs_node infixOp = cs_new_node_text("infixOp" , tokenvalue) ;
	    	cs_append_child(infix, infixOp) ;
	        nextToken() ;
	        cs_node term1 = parseTerm() ;
	        cs_append_child(infix, term1) ;
	    }
	    cs_append_child(expression, infix) ;
	}
	else
	{
		cs_append_child(expression, term) ;
	}
    return expression ;
}

cs_node parseExpressionList()
{
	cs_node expressionList = cs_new_node("expressionList") ;
	if(token != ")")
	{
		cs_node expression = parseExpression() ;
		cs_append_child(expressionList, expression) ;
		while(have(","))
		{
			expression = parseExpression() ;
			cs_append_child(expressionList, expression) ;
		}
	}
	return expressionList ;
}

cs_node parseTerm()
{
	cs_node term = cs_new_node("term") ;
    if ( have("integerConstant") )
    {
        cs_node integerConstant = cs_new_node_text("integerConstant" , value_holder) ;
        cs_append_child(term, integerConstant) ;
    }
    else if ( have("stringConstant") )
    {
        cs_node stringConstant = cs_new_node_text("stringConstant" , value_holder) ;
        cs_append_child(term, stringConstant) ;
    }
    else if ( check_keyword_constant(tokenvalue) )
    {
        nextToken() ;
        cs_node keywordConstant = cs_new_node_text("keywordConstant" , value_holder) ;
        cs_append_child(term, keywordConstant) ;
    }
    else if ( have("(") )
    {
        cs_node expression = cs_new_node("expression") ;
        expression = parseExpression() ;
        mustbe(")") ;
        cs_append_child(term, expression) ;
    }
    else if ( check_unaryOp(tokenvalue) )
    {
    	cs_node unary = cs_new_node("unary") ;
    	nextToken() ;
        cs_node unaryOp = cs_new_node_text("unaryOp", value_holder) ;
        cs_append_child(unary, unaryOp) ;
        cs_node more_term = cs_new_node("term") ;
        more_term = parseTerm() ;
        cs_append_child(unary, more_term) ;
        cs_append_child(term, unary) ;
    }
    else if ( check_variable(tokenvalue) || tokenvalue == className || check_argument(tokenvalue) || checkJackClass(tokenvalue))
    {
        nextToken() ;
        cs_node varName = cs_new_node_text("varName" , value_holder) ;

        if(value_holder == className || checkJackClass(value_holder))
        {
        	varName = cs_new_node_text("className" , value_holder) ;
        }
        if(have("["))
        {
        	cs_node arrayIndex = cs_new_node("arrayIndex") ;
	    	cs_node expression = parseExpression() ;
	    	cs_append_child(arrayIndex, varName) ;

	    	cs_append_child(arrayIndex, expression) ;
	    	cs_append_child(term, arrayIndex) ;
	    	mustbe("]") ;
        }
        else if(have("."))
        {
        	cs_node subroutineCall = cs_new_node("subroutineCall") ;
        	subroutineCall = parseSubroutineCall(varName) ;
        	cs_append_child(term, subroutineCall) ;
        }
        else
        {
        	cs_append_child(term, varName) ;
        }
    }
    else
    {
    	cs_node subroutineCall = cs_new_node("subroutineCall") ;
       	subroutineCall = parseSubroutineCall() ;
        cs_append_child(term, subroutineCall) ;
    }

    return term;
}


cs_node parseSubroutineCall()
{
	cs_node subroutineCall = cs_new_node("subroutineCall") ;
	if(!check_subroutine(tokenvalue))
	{
		if(tokenvalue == className)
		{
			cs_node className_node = cs_new_node_text("className", tokenvalue) ;
			cs_append_child(subroutineCall, className_node) ;
			nextToken() ;
		}
		else
		{
			if(!check_argument(tokenvalue) && !check_variable(tokenvalue))
			{
				declare_subroutine(tokenvalue) ;
			}
			cs_node varName = cs_new_node("varName") ;
			cs_append_child(subroutineCall, varName) ;
			nextToken() ;
		}
		mustbe(".") ;
	}
	cs_node keywordConstant = cs_new_node_text("keywordConstant", "this") ;
	cs_append_child(subroutineCall, keywordConstant) ;
	mustbe("identifier") ;
	if(!check_subroutine(value_holder))
	{
		declare_subroutine(value_holder) ;
	}
	cs_node subroutineName = cs_new_node_text("subroutineName", value_holder) ;
	cs_append_child(subroutineCall, subroutineName) ;
	mustbe("(") ;
	cs_node expressionList = cs_new_node("expressionList") ;
	expressionList = parseExpressionList() ;
	cs_append_child(subroutineCall, expressionList) ;
	mustbe(")") ;
	return subroutineCall ;
}

cs_node parseSubroutineCall(cs_node varName)
{
	cs_node subroutineCall = cs_new_node("subroutineCall") ;
	cs_append_child(subroutineCall, varName) ;
	mustbe("identifier") ;
	if(!check_subroutine(value_holder))
	{
		declare_subroutine(value_holder) ;
	}
	cs_node subroutineName = cs_new_node_text("subroutineName", value_holder) ;
	cs_append_child(subroutineCall, subroutineName) ;
	mustbe("(") ;
	cs_node expressionList = cs_new_node("expressionList") ;
	expressionList = parseExpressionList() ;
	cs_append_child(subroutineCall, expressionList) ;
	mustbe(")") ;
	return subroutineCall ;
}

// the function jack_parser() will be called by the main program
// the document tree constructed must be returned as the function result
cs_node jack_parser()
{
    // create a tokeniser and read first token to initialise it
    init_tokeniser() ;
    nextToken() ;

    // parse a Program to get a new document tree
    cs_node root = parseClass() ;

    // check for end of file
    mustbe("?") ;

    // return root of document tree we parsed
    return root ;
}









