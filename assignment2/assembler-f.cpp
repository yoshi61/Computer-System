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
#include <vector>

// to simplify the code
using namespace std ;

//this function convert string type number into binary
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

//this function convert int type number into binary
string toBinary(int n)
{
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
void assembler_f()
{
	//this table stores the jmp part binary fot C-instruction
	symbol_table_string JMP_table = create_symbol_table_string();
	if ( !JMP_table->insert("NULL","000") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JGT", "001") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JEQ", "010") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JGE", "011") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JLT", "100") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JNE", "101") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JLE", "110") ) cout << "Error!" << endl;
	if ( !JMP_table->insert("JMP", "111") ) cout << "Error!" << endl;

	//This table stores the dest part binary for C-instruction
	symbol_table_string dest_table = create_symbol_table_string();
	if ( !dest_table->insert("NULL","000") ) cout << "Error!" << endl;
	if ( !dest_table->insert("M","001") ) cout << "Error!" << endl;
	if ( !dest_table->insert("D","010") ) cout << "Error!" << endl;
	if ( !dest_table->insert("MD","011") ) cout << "Error!" << endl;
	if ( !dest_table->insert("A","100") ) cout << "Error!" << endl;
	if ( !dest_table->insert("AM","101") ) cout << "Error!" << endl;
	if ( !dest_table->insert("AD","110") ) cout << "Error!" << endl;
	if ( !dest_table->insert("AMD","111") ) cout << "Error!" << endl;

	//This table stores the comp part binary for C-instruction
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

	//This table stores the pre-defined variable address and will store user-defined variable address later
	symbol_table_string variable_table = create_symbol_table_string();
	if ( !variable_table->insert("R0","000000000000000") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R1","000000000000001") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R2","000000000000010") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R3","000000000000011") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R4","000000000000100") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R5","000000000000101") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R6","000000000000110") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R7","000000000000111") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R8","000000000001000") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R9","000000000001001") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R10","000000000001010") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R11","000000000001011") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R12","000000000001100") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R13","000000000001101") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R14","000000000001110") ) cout << "Error!" << endl;
	if ( !variable_table->insert("R15","000000000001111") ) cout << "Error!" << endl;
	if ( !variable_table->insert("SP","000000000000000") ) cout << "Error!" << endl;
	if ( !variable_table->insert("LCL","000000000000001") ) cout << "Error!" << endl;
	if ( !variable_table->insert("ARG","000000000000010") ) cout << "Error!" << endl;
	if ( !variable_table->insert("THIS","000000000000011") ) cout << "Error!" << endl;
	if ( !variable_table->insert("THAT","000000000000100") ) cout << "Error!" << endl;
	if ( !variable_table->insert("SCREEN","100000000000000") ) cout << "Error!" << endl;
	if ( !variable_table->insert("KBD","110000000000000") ) cout << "Error!" << endl;

	static int variable_counter = 16; //this records the number of current variables

	symbol_table_string label_table = create_symbol_table_string();

//////////////////////////////The First Lap/////////////////////////////////////

	vector<string> v_token_class;//record the token class read by the tokeniser
	vector<string> v_token_value;//record the token value read by the tokeniser

	int instrution_counter;//count the number of instruction has been read
    init_tokeniser() ;//initialise tokeniser
    string token = next_token() ;

	while ( token != "?" )			// stop at EOF, "?" denotes EOF
	{	
		if(token == "label")		//Find out labels and record them in label table
		{
			if ( !label_table->insert(token_value(), toBinary(instrution_counter)) ) cout << "Error!" << endl;
		}

		else if(token == "address")	//Find A-instruction for instruction_counter to count up
		{
			v_token_class.push_back(token);
			v_token_value.push_back(token_value());
			instrution_counter++;
		}
		else if(token == "arrow")	//Find C-instruction for instruction_counter to count up
		{
			v_token_class.push_back(token);
			v_token_value.push_back(token_value());
			instrution_counter++;
		}
		else if(token == "semi")	//Find C-instruction for instruction_counter to count up
		{
			v_token_class.push_back(token);
			v_token_value.push_back(token_value());
			instrution_counter++;
		}
		else 
		{
			v_token_class.push_back(token);
			v_token_value.push_back(token_value());
		}
		// read the next token
		token = next_token() ;
	}

//////////////////////////////The Second lap/////////////////////////////////////////

	int token_counter = 0;//initialise token counter
    token = v_token_class[token_counter];//read token class
    string token_value = v_token_value[token_counter];//read token value
    string part_of_instrution;//record one single instruction
    vector<string> v_full_instruction; //record the entire code
    int syntax_error=0; //count up syntax errors

	while ( token_counter < v_token_class.size()-2)	// stop at EOF
	{
		part_of_instrution = "";//initialise part of instruction
		
		//Find out A-instruction and check its syntax
		if(token == "address")
		{
			//check if its @NAME
			if(isalpha(token_value[0])|| token_value[0] == '_'|| token_value[0] == '$'|| token_value[0] == ':'|| token_value[0] == '.')
			{
				//look up on the variable table
				if(variable_table->lookup(token_value) != "")
				{
					part_of_instrution = "0" + variable_table->lookup(token_value);
				}

				//look up on label table
				else if(label_table->lookup(token_value) != "")
				{
					part_of_instrution = "0" + label_table->lookup(token_value);
				}

				//not found then add to the variable table
				else
				{
					if ( !variable_table->insert(token_value,toBinary(variable_counter)) ) cout << "Error!" << endl;
					variable_counter++;
					part_of_instrution = "0" + variable_table->lookup(token_value);
				}
			}

			//check if its @number
			else if(isdigit(token_value[0]))
			{
				part_of_instrution = "0" + toBinary(token_value);
			}

			v_full_instruction.push_back(part_of_instrution);
			if(token_counter < v_token_class.size()-1)
						{
							// read the next token
							token_counter++;
							token = v_token_class[token_counter];
							token_value = v_token_value[token_counter];
						}
		}
		
		//Find out C-instruction and check its syntax
		else if(token == "dest-comp?" | token == "comp")
		{
			part_of_instrution = "111";
			string abit = "0";
			string token_value_holder = token_value;
			for(int i = 0; i < token_value.size(); i++)
			{
				if(token_value_holder[i] == 'M')
				{
					abit = "1";
					token_value_holder[i] = 'A';
				}
			}
			if(abit == "0")
			{
				part_of_instrution = part_of_instrution + abit + comp_table->lookup(token_value);//add up instruction
				// read the next token
				token_counter++;
				token = v_token_class[token_counter];
				token_value = v_token_value[token_counter];
				if(token == "arrow")
				{
					// read the next token
					token_counter++;
					token = v_token_class[token_counter];
					token_value = v_token_value[token_counter];
					if(token_value == "NULL" |token_value == "M"|token_value == "D"||token_value == "MD"||token_value == "A"||token_value == "AM"||token_value == "AD"||token_value == "AMD")
					{
						part_of_instrution = part_of_instrution + dest_table->lookup(token_value) + JMP_table->lookup("NULL");//add up instruction

			
						//cout << part_of_instrution << endl;
						v_full_instruction.push_back(part_of_instrution);
						if(token_counter < v_token_class.size()-1)
						{
							// read the next token
							token_counter++;
							token = v_token_class[token_counter];
							token_value = v_token_value[token_counter];
						}
					}
					else
					{
						syntax_error++;
						break;
					}
					
				}
				else if(token == "semi")
				{
					// read the next token
					token_counter++;
					token = v_token_class[token_counter];
					token_value = v_token_value[token_counter];
					if(token == "jump")
					{
						part_of_instrution = part_of_instrution + dest_table->lookup("NULL") + JMP_table->lookup(token_value);//add up instruction

			
						//cout << part_of_instrution << endl;
						v_full_instruction.push_back(part_of_instrution);
						if(token_counter < v_token_class.size()-1)
						{
							// read the next token
							token_counter++;
							token = v_token_class[token_counter];
							token_value = v_token_value[token_counter];
						}
					}
					else
					{
						syntax_error++;
						break;
					}
				}
			}
			else if(abit == "1")
			{
				part_of_instrution = part_of_instrution + abit + comp_table->lookup(token_value_holder);//add up instruction//add up instruction
				// read the next token
				token_counter++;
				token = v_token_class[token_counter];
				token_value = v_token_value[token_counter];
				if(token == "arrow")
				{
					// read the next token
					token_counter++;
					token = v_token_class[token_counter];
					token_value = v_token_value[token_counter];
					if(token_value == "NULL" |token_value == "M"||token_value == "D"||token_value == "MD"||token_value == "A"||token_value == "AM"||token_value == "AD"||token_value == "AMD")
					{
						part_of_instrution = part_of_instrution + dest_table->lookup(token_value) + JMP_table->lookup("NULL");//add up instruction

						v_full_instruction.push_back(part_of_instrution);
						if(token_counter < v_token_class.size()-1)
						{
							// read the next token
							token_counter++;
							token = v_token_class[token_counter];
							token_value = v_token_value[token_counter];
						}
					}
					else
					{
						syntax_error++;
						break;
					}
					
				}
				else if(token == "semi")
				{
					// read the next token
					token_counter++;
					token = v_token_class[token_counter];
					token_value = v_token_value[token_counter];
					if(token == "jump")
					{
						part_of_instrution = part_of_instrution + dest_table->lookup("NULL") + JMP_table->lookup(token_value);//add up instruction

						v_full_instruction.push_back(part_of_instrution);
						if(token_counter < v_token_class.size()-1)
						{
							// read the next token
							token_counter++;
							token = v_token_class[token_counter];
							token_value = v_token_value[token_counter];
						}
					}
					else
					{
						syntax_error++;
						break;
					}
				}
			}
		}
		else
		{
			syntax_error++;
			break;
		}

	}
	if (syntax_error==0)//if any syntax error, then do not output anything
	{
		for(int j=0; j<v_full_instruction.size(); j++)
		{
			cout << v_full_instruction[j] << endl;
		}
	}
}
