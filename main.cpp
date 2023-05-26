#include "func.h"

int main(){
    string infix = "(10+(2^3-2))/(2*2)";
    string Logicinfix = "(~p|q)<->((s&t)->u)";

    //Arithmetic
    cout << Infix2Postfix(infix) << endl; //to postfix
    cout << Infix2Prefix(infix) << endl; //to prefix
    cout << PostfixPrefixCalculator(Infix2Postfix(infix)) << endl; //calculate postfix
    cout << PostfixPrefixCalculator(Infix2Prefix(infix)) << endl; //calculate prefix

    //Logic functions
    cout << LogicInfix2Postfix(Logicinfix) << endl; //to postfix
    cout << LogicInfix2Prefix(Logicinfix) << endl; //to prefix
    cout << LogicPostfixPrefixCalculator(LogicInfix2Postfix(Logicinfix), "p q s t u 1 1 0 1 0") << endl; //calculate postfix
    cout << LogicPostfixPrefixCalculator(LogicInfix2Prefix(Logicinfix), "p q s t u 1 1 0 1 0") << endl; //calculate prefix

    return 0;
}
