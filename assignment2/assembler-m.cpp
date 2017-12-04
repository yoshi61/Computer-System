// convert Hack assembly into binary
#include <iostream>
#include "tokeniser.h"
#include "symbols.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sstream>
#include <iterator>

// to simplify the code
using namespace std ;

//this function convert string type number to binary
string toBinary(string num)
{
   	long int n = 0;
   	stringstream stoi1(num);
   	stoi1 >> n;
    string result;
    while(n!=0) 
   	{
   		result=(n%2==0 ?"0":"1")+result; 
    	n/=2;
    }
    if(result.size() < 15)
    {
    	while(result.size() < 15)
    	{
    		result = "0" + result;
    	}
    }
    else if(result.size() > 15)
    {
    	result.erase(result.begin(),result.end()-15);
    }
    return result;
}

// standard input is the contents of a Hack assembly file
void assembler_m()
{
	//table for jmp part of C-instruction
	symbol_table_string JMP_table = create_symbol_table_string();
	if ( !JMP_table->insert("NULL","000") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JGT", "001") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JEQ", "010") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JGE", "011") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JLT", "100") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JNE", "101") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JLE", "110") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JMP", "111") ) cout << "Error!" << endl;

	//table for dest part of C-instruction
	symbol_table_string dest_table = create_symbol_table_string();
	if ( !dest_table->insert("NULL","000") ) cout << "Error!" << endl;
	if ( !dest_table->insert("M","001") ) cout << "Error!" << endl;
	if ( !dest_table->insert("D","010") ) cout << "Error!" << endl;
	if ( !dest_table->insert("MD","011") ) cout << "Error!" << endl;
	if ( !dest_table->insert("A","100") ) cout << "Error!" << endl;
	if ( !dest_table->insert("AM","101") ) cout << "Error!" << endl;
	if ( !dest_table->insert("AD","110") ) cout << "Error!" << endl;
	if ( !dest_table->insert("AMD","111") ) cout << "Error!" << endl;

	//table for comp part of C-instruction
	symbol_table_string comp_table = create_symbol_table_string();
	if ( !comp_table->insert("0","101010") ) cout << "Error!" << endl;
	if ( !comp_table->insert("1","111111") ) cout << "Error!" << endl;
	if ( !comp_table->insert("-1","111010") ) cout << "Error!" << endl;
	if ( !comp_table->insert("D","001100") ) cout << "Error!" << endl;
	if ( !comp_table->insert("A","110000") ) cout << "Error!" << endl;
	if ( !comp_table->insert("!D","001101") ) cout << "Error!" << endl;
	if ( !comp_table->insert("!A","110001") ) cout << "Error!" << endl;
	if ( !comp_table->insert("-D","001111") ) cout << "Error!" << endl;
	if ( !comp_table->insert("-A","110011") ) cout << "Error!" << endl;
	if ( !comp_table->insert("D+1","011111") ) cout << "Error!" << endl;
	if ( !comp_table->insert("A+1","110111") ) cout << "Error!" << endl;
	if ( !comp_table->insert("D-1","001110") ) cout << "Error!" << endl;
	if ( !comp_table->insert("A-1","110010") ) cout << "Error!" << endl;
	if ( !comp_table->insert("D+A","000010") ) cout << "Error!" << endl;
	if ( !comp_table->insert("D-A","010011") ) cout << "Error!" << endl;
	if ( !comp_table->insert("A-D","000111") ) cout << "Error!" << endl;
	if ( !comp_table->insert("D&A","000000") ) cout << "Error!" << endl;
	if ( !comp_table->insert("D|A","010101") ) cout << "Error!" << endl;

	// initialise the tokeniser
	init_tokeniser() ;

	string instrution;
	string token = next_token() ;
	while ( token != "?" )			// stop at EOF, "?" denotes EOF
	{
		instrution = "";		//initialise instruction

		//check if it is an A-instruction by seeing the first token with class address
		if(token == "address")
		{
			instrution = "0" + toBinary(token_value());//A-instruction is 0+value
			cout << instrution << endl;
		}
		
		//check if it is a C-instruction
		else if(token == "dest-comp?" | token == "comp")
		{
			instrution = "111";//C-instruction always start with 111
			string abit = "0";
			string token_value_holder = token_value();
			for(int i = 0; i < token_value().size(); i++)//check A bit is 0 or 1 by checking the comp has M or A
			{
				if(token_value_holder[i] == 'M')//if it has M then change it to A
				{
					abit = "1";
					token_value_holder[i] = 'A';
				}
			}
			if(abit == "0")//A bit is 0
			{
				instrution = instrution + abit + comp_table->lookup(token_value());//add up instruction
				token = next_token();
				if(token == "arrow")
				{
					token = next_token();
					if(token_value() == "NULL" |token_value() == "M"|token_value() == "D"||token_value() == "MD"||token_value() == "A"||token_value() == "AM"||token_value() == "AD"||token_value() == "AMD")
					{
						instrution = instrution + dest_table->lookup(token_value()) + JMP_table->lookup("NULL");//add up instruction
						cout << instrution << endl;
					}
					else
					{
						token = "?";
					}
					
				}
				else if(token == "semi")
				{
					token = next_token();
					if(token == "jump")
					{
						instrution = instrution + dest_table->lookup("NULL") + JMP_table->lookup(token_value());//add up instruction
						cout << instrution << endl;
					}
					else
					{
						token = "?";
					}
				}
			}
			else if(abit == "1")
			{
				instrution = instrution + abit + comp_table->lookup(token_value_holder);//add up instruction
				token = next_token();
				if(token == "arrow")
				{
					token = next_token();
					if(token_value() == "NULL" |token_value() == "M"||token_value() == "D"||token_value() == "MD"||token_value() == "A"||token_value() == "AM"||token_value() == "AD"||token_value() == "AMD")
					{
						instrution = instrution + dest_table->lookup(token_value()) + JMP_table->lookup("NULL");//add up instruction
						cout << instrution << endl;
					}
					else
					{
						token = "?";
					}
					
				}
				else if(token == "semi")
				{
					token = next_token();
					if(token == "jump")
					{
						instrution = instrution + dest_table->lookup("NULL") + JMP_table->lookup(token_value());//add up instruction
						cout << instrution << endl;
					}
					else
					{
						token = "?";
					}
				}
			}
		}
		
		else 		//if syntax is wrong, while loop ends
		{
			break;
		}

		// read the next token
		token = next_token() ;
	}

}
