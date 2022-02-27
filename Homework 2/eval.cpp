#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool ltePrecedence(char a, char b) {
    if ((a == '-' || a == '+') && (b == '*' && b == '/'))
        return true;
    if ((a == '-' && b == '+') || (a == '+' && b == '-'))
        return true;
    if ((a == '*' && b == '/') || (a == '/' && b == '*'))
        return true;
    return false;
}

int checkParentheses(string& infix, int index, int& endIndex) {
    //expect first char to be '(', set endIndex to the pos of ')'
    int opCount = 0;
    for (int i = index + 1; i < infix.size(); i++) {
        switch (infix[i]) {
            case ' ':
                break;
            case '/':
            case '*':
            case '-':
            case '+':
                opCount--;
                break;
            case '(':
            {
                if (opCount != 0)
                    return 1;
                int miniEnd;
                int result = checkParentheses(infix, i, miniEnd); //recursive check
                if (result == 1)
                    return 1;
                opCount++;
                i = miniEnd;
                break;
            }
            case ')':
                if (opCount != 1)
                    return 1;
                endIndex = i;
                return 0;
            default:
                if (isalpha(infix[i]) && islower(infix[i])) { //operands lowercase letters
                    opCount++;
                } else
                    return 1;
        }
    }
    return 1;
}

int checkSyntax(string& infix) {
    int opCount = 0;
    if (infix.size() == 0)
        return 1;
    for (int i = 0; i < infix.size(); i++) {
        switch (infix[i]) {
            case ' ':
                break;
            case '/':
            case '*':
            case '-':
            case '+':
                opCount--;
                break;
            case '(':
            {
                if (opCount != 0)
                    return 1;
                int endIndex;
                int result = checkParentheses(infix, i, endIndex); //recursive check
                if (result == 1)
                    return 1;
                opCount++;
                i = endIndex;
                break;
            }
            default:
                if (isalpha(infix[i]) && islower(infix[i])) { //operands lowercase letters
                    opCount++;
                } else
                    return 1;
        }
        if (opCount < 0 || opCount > 1)
            return 1;
    }
    if (opCount != 1)
        return 1;
    else
        return 0;
}

int evaluate(string infix, const Map& values, string& postfix, int& result)
{   // Evaluates an integer arithmetic expression
    //   If infix is a syntactically valid infix integer expression whose
    //   only operands are single lower case letters (whether or not they
    //   appear in the values map), then postfix is set to the postfix
    //   form of the expression; otherwise postfix may or may not be
    //   changed, result is unchanged, and the function returns 1.  If
    //   infix is syntactically valid but contains at least one lower
    //   case letter operand that does not appear in the values map, then
    //   result is unchanged and the function returns 2.  If infix is
    //   syntactically valid and all its lower case operand letters
    //   appear in the values map, then if evaluating the expression
    //   (using for each letter in the expression the value in the map
    //   that corresponds to it) attempts to divide by zero, then result
    //   is unchanged and the function returns 3; otherwise, result is
    //   set to the value of the expression and the function returns 0.
    if (checkSyntax(infix) == 1)
        return 1;
    //set postfix
    postfix = "";
    stack<char> opStack;
    for (char ch : infix) {
        switch (ch) {
            case ' ':
                break;
            case '/':
            case '*':
            case '-':
            case '+':
                while (!opStack.empty() && opStack.top() != '(' && ltePrecedence(ch, opStack.top())) {
                    postfix += opStack.top();
                    opStack.pop();
                }
                opStack.push(ch);
                break;
            case '(':
                opStack.push(ch);
                break;
            case ')':
                while (opStack.top() != '(') {
                    postfix += opStack.top();
                    opStack.pop();
                }
                opStack.pop();
                break;
            case 'a' ... 'z':
                postfix += ch;
        }
    }
    
    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }
    //infix is syntactically valid, postfix set
    for (int i = 0; i < postfix.size(); i++) {
        if (isalpha(postfix[i]) && !values.contains(postfix[i]))
            return 2;
    }
    //infix is syntactically valid and all operands are in values map
    stack<int> resultStack;
    for (char ch : postfix) {
        if (isalpha(ch)) {
            int chVal;
            values.get(ch, chVal);
            resultStack.push(chVal);
        } else {
            int operand2 = resultStack.top();
            resultStack.pop();
            int operand1 = resultStack.top();
            resultStack.pop();
            switch (ch) {
                case '/':
                    if (operand2 == 0)
                        return 3; 
                    resultStack.push(operand1 / operand2);
                    break;
                case '*':
                    resultStack.push(operand1 * operand2);
                    break;
                case '-':
                    resultStack.push(operand1 - operand2);
                    break;
                case '+':
                    resultStack.push(operand1 + operand2);
                    break;
            }
        }
    }
    result = resultStack.top();
    resultStack.pop();
    return 0;
}

int main()
{
    char vars[] = { 't', 'v', 'w', 'x'};
    int  vals[] = {  3,5,2,-25};
    Map m;
    for (int k = 0; k != 4; k++)
        m.insert(vars[k], vals[k]);
    string postfix;
    int result;
    int ans = evaluate("w*t+v", m, postfix, result);
    std::cout << ans << "," << postfix << "," << result;
}