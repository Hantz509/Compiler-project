#include "parser.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <map> // For variable storage
#include "lexer.h"

using namespace std;

// Helper function to check if a string is a number
bool isNumber(const string& token) {
    for (char c : token) {
        if (!isdigit(c)) return false;
    }
    return true;
}

map<string, int> variables; // To store variables
// Helper function to evaluate arithmetic comparisons
bool evaluateCondition(const string& left, const string& op, const string& right) {
    int l, r;

    // Check if 'left' is a variable or a number
    if (variables.find(left) != variables.end()) {
        l = variables[left]; // Fetch variable value
    } else if (isNumber(left)) {
        l = stoi(left); // Convert number
    } else {
        throw runtime_error("Error: Invalid left operand in condition '" + left + "'");
    }

    // Check if 'right' is a variable or a number
    if (variables.find(right) != variables.end()) {
        r = variables[right]; // Fetch variable value
    } else if (isNumber(right)) {
        r = stoi(right); // Convert number
    } else {
        throw runtime_error("Error: Invalid right operand in condition '" + right + "'");
    }

    // Perform the comparison
    if (op == ">") return l > r;
    if (op == "<") return l < r;
    if (op == "==") return l == r;
    if (op == "!=") return l != r;
    if (op == ">=") return l >= r;
    if (op == "<=") return l <= r;

    throw runtime_error("Error: Invalid operator '" + op + "'");
}



// Helper function to evaluate arithmetic expressions
int evaluateExpression(const vector<string>& tokens, size_t start) {
    int result = 0;
    char operation = '+';

    for (size_t i = start; i < tokens.size(); ++i) {
        const string& token = tokens[i];

        int value = 0;

        if (isNumber(token)) { 
            // Token is a number
            value = stoi(token);
        } else if (variables.find(token) != variables.end()) {
            // Token is a variable
            value = variables[token];
        } else if (token == "+" || token == "-" || token == "*" || token == "/") {
            operation = token[0];
            continue; // Move to the next token
        } else {
            throw runtime_error("Error: Invalid token in expression '" + token + "'");
        }

        // Apply the operation
        if (operation == '+') result += value;
        else if (operation == '-') result -= value;
        else if (operation == '*') result *= value;
        else if (operation == '/') result /= value;
    }

    return result;
}

 



void parse(const vector<string>& tokens) {
    if (tokens.empty()) return;

    if (tokens[0] == "print") {
            if (tokens.size() > 1) { // Ensure there's an argument to print
                try {
                    if (tokens[1][0] == '"') {
                        // Case 1: Print a string literal (enclosed in quotes)
                        cout << tokens[1].substr(1, tokens[1].size() - 2) << endl; // Remove quotes
                    } 
                    else if (variables.find(tokens[1]) != variables.end()) {
                        // Case 2: Print a variable value if it exists in the variables map
                        cout << variables[tokens[1]] << endl;
                    } 
                    else {
                        // Case 3: Evaluate and print an arithmetic expression
                        int result = evaluateExpression(tokens, 1);
                        cout << result << endl;
                    }
                } 
                catch (const exception& e) {
                    // Catch any runtime errors during evaluation
                    throw runtime_error("Error in print statement: " + string(e.what()));
                }
            } 
            else {
                // If 'print' has no argument, throw an error
                throw runtime_error("Error: Missing argument for 'print'.");
            }
    }


    else if (tokens.size() >= 3 && tokens[1] == "=") {
    // Handle variable assignment
    string varName = tokens[0];
    vector<string> expression(tokens.begin() + 2, tokens.end());
    
    try {
        int value = evaluateExpression(expression, 0);
        variables[varName] = value;
    } catch (const exception& e) {
        throw runtime_error("Error in assignment: " + string(e.what()));
    }
}




   else if (tokens[0] == "if") {
    // Ensure there are enough tokens for condition and branches
    auto thenPos = find(tokens.begin(), tokens.end(), "then");
    auto elsePos = find(tokens.begin(), tokens.end(), "else");

    if (thenPos != tokens.end()) {
        // Ensure 'else' comes after 'then'
        if (elsePos != tokens.end() && elsePos < thenPos) {
            throw runtime_error("Error: 'else' appears before 'then' in 'if' statement.");
        }

        // Extract the condition tokens
        vector<string> conditionTokens(tokens.begin() + 1, thenPos);

        // Ensure the conditio