#include "func.h"

int getLength(string expr){
    int i = 0;
    while(expr[i] != '\0') i++;

    return i;
};

bool isOp(char x){
    char nonnumArr[7] = {'(', ')', '+', '-', '*', '/', '^'};

    for(int i = 0; i < 7; i++){
        if(x == nonnumArr[i]) return true;
    }

    return false;
};

bool isLogOp(char x){
    char nonnumArr[8] = {'(', ')', '&', '|', '-', '<', '>', '~'};

    for(int i = 0; i < 8; i++){
        if(x == nonnumArr[i]) return true;
    }

    return false;
};

int OprtPriority(char oprt){
    if(oprt == '+' || oprt =='-') return 1;
    else if(oprt == '*' || oprt =='/') return 2;
    else if(oprt == '^') return 3;
    
    return 0;
}

//in post fix scan (l->r), '-' stands for '->', and '<' stands for '<->'
//however, in prefix scan (r->l), '>' can also stands for both '->' and '<->'
int LogOprtPriority(char oprt){
    if(oprt == '<' || oprt == '>') return 1;
    else if(oprt == '-') return 2;
    else if(oprt == '|' || oprt == '&') return 3;
    else if(oprt == '~') return 4;

    return 0;
}

//convert char to int
double to_Db(char x){
    return x - '0';
}

//convert string to bool
bool to_Bool(string s){
    if(s == "1") return true;
    else return false;
}

//double with more than 4 numbers after decimal point will be rounded down to only 4
string roundTo4(double num){
    ostringstream s4;
    string strhold;
    s4 << setprecision(4) << fixed << num;
    strhold = s4.str();

    int l = getLength(strhold);
    if(strhold[l - 1] == '0'){
        ostringstream s3;
        s3 << setprecision(3) << fixed << num;
        strhold = s3.str();
        if(strhold[l - 2] == '0'){
            ostringstream s2;
            s2 << setprecision(2) << fixed << num;
            strhold = s2.str();
            if(strhold[l - 3] == '0'){
                ostringstream s1;
                s1 << setprecision(1) << fixed << num;
                strhold = s1.str();
                if(strhold[l - 4] == '0'){
                    ostringstream s0;
                    s0 << setprecision(0) << fixed << num;
                    strhold = s0.str();
                }
            }
        }
    }
    
    return strhold;
}

//Arithmetic

//Rules to convert:
//scan from left to right
//put operands into postfix upon met
//incoming opertor has lower or equal priority to top then print until top is lower, else push to stack
//if ), pop until (
string Infix2Postfix(string infix){
    string postfix = "";

    stack<char> OprtStack; //Stack to store operators

    int i = 0;
    while(infix[i] != '\0') {
        if(!isOp(infix[i]) && infix[i] != ' ') {
            postfix += infix[i];
        } else if(infix[i]=='(') {
            OprtStack.push(infix[i]);
        } else if(infix[i]==')') {
            while(OprtStack.top()!='('){
                postfix += " ";
                postfix += OprtStack.top();
                OprtStack.pop();
            }
            OprtStack.pop();
        } else {
            while (!OprtStack.empty() && OprtPriority(infix[i]) <= OprtPriority(OprtStack.top())) {
                postfix+=" ";
                postfix += OprtStack.top();
                OprtStack.pop();
            }

            postfix+=" ";
            OprtStack.push(infix[i]);
        }
        i++;
    }

    while(!OprtStack.empty()) {
        postfix+=" ";
        postfix += OprtStack.top();
        OprtStack.pop();
    }

    return postfix;
};

//Rules to convert:
//scan from right to left
//put operands into prefix upon met
//incoming opertor has lower priority than top then print until top is lower, else push to stack
//if (, pop until )
//reverse output
string Infix2Prefix(string infix){
    string prefix = "";
    string invprefix = ""; //inverse version of prefix

    stack<char> OprtStack;

    int stringLength = getLength(infix);

    //scan from right to left
    for(int i = stringLength - 1; i > -1; i--){
       if(!isOp(infix[i]) && infix[i] != ' ') {
            invprefix += infix[i];
        } else if(infix[i]==')') {
            OprtStack.push(infix[i]);
        } else if(infix[i]=='(') {
            while(OprtStack.top()!=')'){
                invprefix += " ";
                invprefix += OprtStack.top();
                OprtStack.pop();
            }
            OprtStack.pop();
        } else {
            while (!OprtStack.empty() && OprtPriority(infix[i]) < OprtPriority(OprtStack.top())) {
                invprefix+=" ";
                invprefix += OprtStack.top();
                OprtStack.pop();
            }

            invprefix+=" ";
            OprtStack.push(infix[i]);
        }
    }

    while(!OprtStack.empty()) {
        invprefix+=" ";
        invprefix += OprtStack.top();
        OprtStack.pop();
    }

    //inverse to get prefix
    int invlength = getLength(invprefix);

    for(int j = invlength - 1; j > -1; j--){
        prefix += invprefix[j];
    }

    return prefix;
};

//Rules:
//2 cases:
/*
Postfix: always starts with an operand
Prefix: always starts with an operator
*/
/*
- calculate the operands using special algorithm (this algorithm is different for each case)
- push operands into stack
- when encounter an operator, pop 2 from stack, the order is different for each case, calc them and push the result back to stack
- the last value in stack will be rounded and converted to string using stringstream and a special function
- return the result string
*/
string PostfixPrefixCalculator(string input){

    int inputLength = getLength(input);
    string res = "";
    double num;

    stack<double> OprdStack;

    if(!isOp(input[0])){ //postfix case
        for(int i = 0; i < inputLength; i++){
            if(input[i] == ' ') {
                OprdStack.push(num);
                num = 0;
            } else if(!isOp(input[i])){
                num = num * 10 + to_Db(input[i]);
            } else {
                //assign value to the second number
                double hold2 = OprdStack.top();
                OprdStack.pop();

                //assign value to the first number
                double hold1 = OprdStack.top();
                OprdStack.pop();
                
                double result;

                if(input[i] == '+'){
                    result = hold1 + hold2;
                } else if(input[i] == '-'){
                    result = hold1 - hold2;
                } else if(input[i] == '*'){
                    result = hold1 * hold2;
                } else if(input[i] == '/'){
                    result = hold1 / hold2;
                } else {
                    result = pow(hold1, hold2);
                }

                OprdStack.push(result);
                i++; // skip a space
            }
        }

    } else { //prefix case

        int grade = 1;

        //calculation phase
        for(int i = inputLength - 1; i > -1; i--){
            if(input[i] == ' ') {
                OprdStack.push(num);
                num = 0;
                grade = 1;
            } else if(input[i] != ' ' && !isOp(input[i])){
                num = num + to_Db(input[i]) * grade;
                grade = grade * 10;
            } else {
                //assign value to the first number
                double hold1 = OprdStack.top();
                OprdStack.pop();

                //assign value to the second number
                double hold2 = OprdStack.top();
                OprdStack.pop();

                double result;

                if(input[i] == '+'){
                    result = hold1 + hold2;
                } else if(input[i] == '-'){
                    result = hold1 - hold2;
                } else if(input[i] == '*'){
                    result = hold1 * hold2;
                } else if(input[i] == '/'){
                    result = hold1 / hold2;
                } else {
                    result = pow(hold1, hold2);
                }

                OprdStack.push(result);
                i--; // skip a space
            }
        }
    }
    
    res = roundTo4(OprdStack.top());

    return res;
};

//Logical

//Rules to convert:
//scan from left to right
//put variables into postfix upon met
//incoming opertor has lower or equal priority to top then print until top is lower, else push to stack
//'-' stands for '->', while '<' stands '<->'
//if ), pop until (
string LogicInfix2Postfix(string infix){
    string postfix = "";

    stack<char> OprtStack; //Stack to store operators

    int i = 0;
    while(infix[i] != '\0') {
        if(!isLogOp(infix[i])) {
            if(infix[i] != ' ') postfix += infix[i];
        } else if(infix[i]=='(') {
            OprtStack.push(infix[i]);
        } else if(infix[i]==')') {
            while(OprtStack.top()!='('){
                if(OprtStack.top() == '-') postfix = postfix + OprtStack.top() + '>';
                else if(OprtStack.top() == '<') postfix = postfix + OprtStack.top() + '-' + '>';
                else postfix += OprtStack.top();

                OprtStack.pop();
            }
            OprtStack.pop();
        } else {
            while (!OprtStack.empty() && LogOprtPriority(infix[i]) <= LogOprtPriority(OprtStack.top())) {
                if(OprtStack.top() == '-') postfix = postfix + OprtStack.top() + '>';
                else if(OprtStack.top() == '<') postfix = postfix + OprtStack.top() + '-' + '>';
                else postfix += OprtStack.top();

                OprtStack.pop();
            }

            OprtStack.push(infix[i]);
            if(infix[i] == '-') i += 1;
            else if(infix[i] == '<') i += 2;
        }
        i++;
    }

    while(!OprtStack.empty()) {
        if(OprtStack.top() == '-') postfix = postfix + OprtStack.top() + '>';
        else if(OprtStack.top() == '<') postfix = postfix + OprtStack.top() + '-' + '>';
        else postfix += OprtStack.top();
        
        OprtStack.pop();
    }

    return postfix;
};

//Rules to convert:
//scan from right to left
//put variables into inverted prefix upon met
//incoming opertor has lower priority than top then print until top is lower, else push to stack
//when encounter '>' check if the character before it 2 digits is '<' (which means '<->') or not ('->')
//if (, pop until )
//reverse inverted prefix to get output
string LogicInfix2Prefix(string infix){
    string prefix = "";
    string invprefix = ""; //inverse version of prefix

    stack<char> OprtStack;

    int stringLength = getLength(infix);

    //scan from right to left
    for(int i = stringLength - 1; i > -1; i--){
       if(!isLogOp(infix[i])) {
            if(infix[i] != ' ') invprefix += infix[i];
        } else if(infix[i]==')') {
            OprtStack.push(infix[i]);
        } else if(infix[i]=='(') {
            while(OprtStack.top()!=')'){
                if(OprtStack.top() == '-') invprefix = invprefix + '>' + OprtStack.top();
                else if(OprtStack.top() == '<') invprefix = invprefix + '>'+ '-' + OprtStack.top();
                else invprefix += OprtStack.top();
        
                OprtStack.pop();
            }
            OprtStack.pop();
        } else if(infix[i]=='>') {
            if(infix[i-2] == '<'){
                while (!OprtStack.empty() && LogOprtPriority('<') < LogOprtPriority(OprtStack.top())) {
                    if(OprtStack.top() == '-') invprefix = invprefix + '>' + OprtStack.top();
                    else if(OprtStack.top() == '<') invprefix = invprefix + '>'+ '-' + OprtStack.top();
                    else invprefix += OprtStack.top();
        
                    OprtStack.pop();
                }

                OprtStack.push('<');
                i -= 2;
            } else {
                while (!OprtStack.empty() && LogOprtPriority('-') < LogOprtPriority(OprtStack.top())) {
                    if(OprtStack.top() == '-') invprefix = invprefix + '>' + OprtStack.top();
                    else if(OprtStack.top() == '<') invprefix = invprefix + '>'+ '-' + OprtStack.top();
                    else invprefix += OprtStack.top();
        
                    OprtStack.pop();
                }

                OprtStack.push('-');
                i -= 1;
            }
        } else {
            while (!OprtStack.empty() && LogOprtPriority(infix[i]) < LogOprtPriority(OprtStack.top())) {
                if(OprtStack.top() == '-') invprefix = invprefix + '>' + OprtStack.top();
                else if(OprtStack.top() == '<') invprefix = invprefix + '>'+ '-' + OprtStack.top();
                else invprefix += OprtStack.top();
        
                OprtStack.pop();
            }

            OprtStack.push(infix[i]);
        }
    }

    while(!OprtStack.empty()) {
        if(OprtStack.top() == '-') invprefix = invprefix + '>' + OprtStack.top();
        else if(OprtStack.top() == '<') invprefix = invprefix + '>'+ '-' + OprtStack.top();
        else invprefix += OprtStack.top();
        
        OprtStack.pop();
    }

    //inverse to get prefix
    int invlength = getLength(invprefix);

    for(int j = invlength - 1; j > -1; j--){
        prefix += invprefix[j];
    }

    return prefix;
};

//Rules:
//2 cases:
/*
Postfix: always starts with a variable
Prefix: always starts with an operator
*/
/*
- calculate amount of variable
- create an array to look up for logical value of each variable using stringstream
- push value into stack
- when encounter an operator, pop from stack, the order is different for each case, calc them and push the result back to stack
- return the last value in stack
*/
string LogicPostfixPrefixCalculator(string input,string varlue){
    int size = (getLength(varlue) + 1) / 4; //number of variables

    //stringstream to store varlue
    stringstream varluestream; 
    varluestream << varlue;

    //put variables and there value to an array
    //distance between var and val = size
    string vararr[size * 2] = {};
    for(int i = 0; i < size * 2; i++) varluestream >> vararr[i];

    //stack to store variables
    stack<bool> VarStack;

    int inputLength = getLength(input); //length of input string

    if(!isLogOp(input[0])){ //postfix
        for(int i = 0; i < inputLength; i++){
            if(!isLogOp(input[i]) && input[i] != ' ') {
                string hold;
                hold += input[i];
                for(int l = 0; l < size*2; l++){
                    if(hold == vararr[l]){
                        VarStack.push(to_Bool(vararr[l + size]));
                        break;
                    }
                }
            } else if(input[i] == '~') {
                bool newVar = !VarStack.top();
                VarStack.pop();
                VarStack.push(newVar);
            } else if(input[i] == '&') {
                bool hold2 = VarStack.top();
                VarStack.pop();

                bool hold1 = VarStack.top();
                VarStack.pop();

                bool newVar = hold1 && hold2;
                VarStack.push(newVar);
            } else if(input[i] == '|') {
                bool hold2 = VarStack.top();
                VarStack.pop();
                bool hold1 = VarStack.top();
                VarStack.pop();

                bool newVar = hold1 || hold2;
                VarStack.push(newVar);
            } else if(input[i] == '-') {
                bool hold2 = VarStack.top();
                VarStack.pop();
                bool hold1 = VarStack.top();
                VarStack.pop();

                bool newVar;
                if(hold1 == 0) newVar = 1;
                else if(hold1 == 1 && hold2 == 1) newVar = 1;
                else newVar = 0;

                VarStack.push(newVar);

                i+=1; //skip ">"
            } else if(input[i] == '<') {
                bool hold2 = VarStack.top();
                VarStack.pop();
                bool hold1 = VarStack.top();
                VarStack.pop();

                bool newVar;
                if(hold1 == hold2) newVar = 1;
                else newVar = 0;
                
                VarStack.push(newVar);
                i += 2; //skip "->"
            }
        }
    } else { //prefix
        for(int i = inputLength - 1; i > -1; i--){
            if(!isLogOp(input[i]) && input[i] != ' ') {
                string hold;
                hold += input[i];
                for(int l = 0; l < size*2; l++){
                    if(hold == vararr[l]){
                        VarStack.push(to_Bool(vararr[l + size]));
                        break;
                    }
                }
            } else if(input[i] == '~') {
                bool newVar = !VarStack.top();
                VarStack.pop();
                VarStack.push(newVar);
            } else if(input[i] == '&') {
                bool hold1 = VarStack.top();
                VarStack.pop();

                bool hold2 = VarStack.top();
                VarStack.pop();

                bool newVar = hold1 && hold2;
                VarStack.push(newVar);
            } else if(input[i] == '|') {
                bool hold1 = VarStack.top();
                VarStack.pop();
                bool hold2 = VarStack.top();
                VarStack.pop();

                bool newVar = hold1 || hold2;
                VarStack.push(newVar);
            } else if(input[i] == '>') {
                bool hold1 = VarStack.top();
                VarStack.pop();
                bool hold2 = VarStack.top();
                VarStack.pop();
                bool newVar;
                if(input[i-2] == '<'){
                    if(hold1 == hold2) newVar = 1;
                    else newVar = 0;
    
                    i -= 2; //skip "<-"
                } else {
                    if(hold1 == 0) newVar = 1;
                    else if(hold1 == 1 && hold2 == 1) newVar = 1;
                    else newVar = 0;

                    i -= 1; //skip "-"
                }

                VarStack.push(newVar);
            }
        }
    }

    if(VarStack.top()) return "TRUE";
    else return "FALSE";
}