// a skeleton of the tokensf class implementation

#include "tokensf.h"
#include <iostream>
#include <ctype.h>

// to shorten the code
using namespace std ;

////////////////////////////////////////////////////////////////////////

// the following global pointer and functions exist to keep the tokensf class
// and its implementation completely hidden from the main function

// pointer to tokeniser object
static tokensf *tokeniser ;

// create and initialise the tokensier - must be called first and exactly once
void init_tokeniser()
{
    tokeniser = new tokensf() ;
}

// next token - calls matching function in tokeniser object
// init_tokeniser() must have been previously called
string next_token()
{
    return tokeniser->next_token() ;
}

// token value - calls matching function in tokeniser object
// init_tokeniser() must have been previously called
string token_value()
{
    return tokeniser->token_value() ;
}


////////////////////////////////////////////////////////////////////////

// the following externally defined function must be used for all input
extern int read_char() ;

// the following externally defined function must be used to lookup keywords
// it returns "" if the string passed in is not a keyword
string lookup_keyword(string) ;


////////////////////////////////////////////////////////////////////////

// the following are the implementations of the tokensf class functions

// construct and initialise a tokensf tokeniser
tokensf::tokensf()
{
    // token value initially ""
    value = "" ;

    // read first character to initialise the tokeniser
    ch = read_char() ;
}

// read next character in the input and record in instance variable ch
void tokensf::nextch()
{
    ch = read_char() ;
}

// return the current token value
string tokensf::token_value()
{
    return value ;
}

// Helper functions can go here - these can simplify writing next_token()
// Some useful builtin helper functions include, isalpha(), isdigit(), isalnum(), etc.


// ****  you must modify the following function *****
// return the next token - return "?" as the token class and token value on error or end of input
string tokensf::next_token()
{
    // you should read input using the nextch() function
    // the last character read is in instance variable ch
    // always read one character past the end of the token being returned

    // this loop reads one character at a time until it reaches end of input
    int checkDecimal = 0;
    int checkAfterZero = 0;
    string valueHolder;
    while ( ch != EOF )
    {
        if(checkDecimal > 0)
        {
            checkDecimal--;
            return "punctuation";
        }
        if(checkAfterZero > 0)
        {
            checkAfterZero--;
            goto Numbers;
        }
///////////////////////////////Check if it is an identifier token/////////////////////////////////////////////////////////////////////////////////////////////
        if(ch == '_' || isalpha(ch))
        {
            value = "";
            while(isdigit(ch) || ch == '_' || isalpha(ch))
            {
                value += ch;
                if(lookup_keyword(value) != "")
                {
                    nextch();
                    return "keyword";
                }
                nextch();
            }
            return "identifier";
        }
///////////////////////////////Check if it is a number token/////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(isdigit(ch))
        {
            value = "";
            if(ch=='0')                     //check if the first number is 0
            {
                value += ch;
                nextch();
                if(ch == '0')
                {
                    return "number";
                }
                else if(isdigit(ch))
                {
                    checkAfterZero++;
                    return "number";
                }
                else if(ch == '.')                //check if there is a decimal after the number or not
                {
                    value += ch;
                    nextch();
                    if(isdigit(ch))       //check if there is a number after the decimal
                    {
                        while(isdigit(ch))
                        {
                        value += ch;
                        nextch();
                        }
                        return "number";
                    }  
                    else                   //not a number, then have to return the "number" and return the decimal as a "punctuation"
                    {
                        checkDecimal++;
                        value = valueHolder;
                        return "number";
                    }  
                }
                else
                {
                    return "number";
                }
            }
            else                            //is 1-9
            {
                value += ch;
                nextch();
                Numbers:
                while(isdigit(ch))
                {
                   value += ch;
                   nextch();
                }
                if(ch=='.')                 
                {
                    valueHolder = value;
                    value += ch;
                    nextch();
                    if(isdigit(ch))
                    {
                        while(isdigit(ch))
                        {
                        value += ch;
                        nextch();
                        }
                        return "number";
                    }    
                    else
                    {
                        checkDecimal++;
                        value = valueHolder;
                        return "number";
                    }
                }
                else
                {

                    return "number";
                }
            }
        }
///////////////////////////////Check if it is an punctuation token////////////////////////////////////////////////////////////////////////////////////////////////////
        else if(ch==';' || ch==':' || ch=='!' || ch==',' || ch=='.' || ch=='=' || ch=='{' || ch=='}' || ch=='(' || ch==')' || ch=='[' || ch==']' || ch=='@')
        {
            value = ch;
            nextch();
            return "punctuation";
        }
///////////////////////////////Check if it is something we can ignor ////////////////////////////////////////////////////////////////////////////////////////////////
        else if(ch=='\t'|| ch=='\n'|| ch==' '|| ch=='\r')
        {
            nextch();
        }
///////////////////////////////  Handling comments can be ignored ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if(ch=='/')
        {
        	nextch();
        	if(ch=='/')
        	{
        		while(ch!='\n')
        		{
        			nextch();
        		}
        	}
        	else if(ch=='*')
        	{
        		value = ch;
        		nextch();
        		value += ch;
        		if(value == "*/")
        		{
        			nextch();
        		}
        		else
        		{
        			while(value != "*/")
        			{
        				value = ch;
        				nextch();
        				value += ch;
        			}
        			nextch();
        		}	
        	}
        	else
        	{
        		value = "?" ;
            	return "?" ;
        	}
        }
///////////////////////////////Undefined character! will return "?"/////////////////////////////////////////////////////////////////////////////////////////////////
        else
        {
            value = "?" ;
            return "?" ;
        }
    }
    value = "?" ;
    return "?" ;
}

