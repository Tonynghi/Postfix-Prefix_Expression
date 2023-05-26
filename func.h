#ifndef __FUNC_H__
#define __FUNC_H__

#include <iostream>
#include <math.h>
#include<stack>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

//get string length function
int getLength(string expr);

//check if operator functions
bool isOp(char x); //arithmetic
bool isLogOp(char x); // logic

//operator priority functions
int OprtPriority(char oprt); //arithmetic
int LogOprtPriority(char oprt); //logic

//char to double converter
double to_Db(char x);

//string to bool converter
bool to_Bool(string s);

//round a double with precision > 4
string roundTo4(double num);

//Arithmetic
string Infix2Postfix(string infix);
string Infix2Prefix(string infix);
string PostfixPrefixCalculator(string input);

//Logical
string LogicInfix2Postfix(string infix);
string LogicInfix2Prefix(string infix);
string LogicPostfixPrefixCalculator(string input,string varlue);

#endif //__FUNC_H__