#ifndef LIB_TOKENISER_H
#define LIB_TOKENISER_H

#include <string>

// interface to the precompiled tokeniser
extern void init_tokeniser() ;                 // initialise the tokensier
extern std::string next_token() ;              // read the next token
extern std::string token_value() ;             // token value for last token read
extern std::string token_class() ;             // token class for last token read
extern std::string token_match(std::string) ;  // convert token into a token class, "?" if unrecognised
extern std::string tokeniser_context() ;       // a 2 or 3 line string showing context of last character read

// EXAMPLE OF USE
// before you can use the tokeniser you must initialise it using the init_tokeniser function
// this must not be called more than once
//    init_tokeniser() ;

// to read the next token from the input call the next_token function
// this will read standard input up to and including the first character after the next token
// the result is the token class
// it must not be called until after init_tokeniser has been called
//    mystring = next_token() ;

// to find out what characters make up the token last returned by next_token you need to call the function token_value
// it must not be called unless next_token has been called at least once
//    mystring = token_value() ;

// to find out the class of the token last returned by next_token you need to call the function token_class
// it must not be called unless next_token has been called at least once
//    mystring = token_class() ;

// IMPORTANT
// the tokeniser can only read standard input once
// if you want to read the input more than once you will need to 
// remember the tokens and their values in a suitable data structure

// if you want to see which token class an arbitrary string matches call token_match
// this will return the relevant token or "?"
//    token_match("if") == "keyword"

// if you want to write out an error message and give the current position in the input
// you can call tokeniser_context() and then write out the message
//    cerr << tokeniser_context() << "syntax error" << endl ;

#endif //LIB_TOKENISER_H
