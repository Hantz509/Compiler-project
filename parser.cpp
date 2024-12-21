#include "parser.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <map>
#include "lexer.h"

using namespace std;

// Storage for variables and functions
map<string, int> variables; // Variables
map<string, UserFunction> functions; // Function definitions

// Check if a string is a number
bool isNumber(const string& token) {
    for (char c : token) if (!isdigit(c)) return false;
    return true;
}

// Evaluate conditions
bool evaluateCondition(const string& left, const string& op, const string& right) {
    int l, r;

    // Check left operand
    if (variables.find(left) != variables.end()) {
        l = variables[left];
    } else if (isNumber(left)) {
        l = stoi(left);
    } else {
        throw runtime_error("Error: Invalid left operand '" + left + "'");
    }

    // Check right operand
    if (variables.find(right) != variables.end()) {
        r = variables[right];
    } else if (isNumber(right)) {
        r = stoi(right);
    } else {
        throw runtime_error("Error: Invalid right operand '" + right + "'");
    }

    // Perform comparisons
    if (op == ">") return l > r;
    if (op == "<") return l < r;
    if (op == "==") return l == r;
    if (op == "!=") return l != r;
    if (op == ">=") return l >= r;
    if (op == "<=") return l <= r;

    throw runtime_error("Error: Invalid operator '" + op + "'");
}


// Evaluate arithmetic expressions
int evaluateExpression(const vector<string>& tokens, size_t start) {
    int result = 0;
    char operation = '+';

    for (size_t i = start; i < tokens.size(); ++i) {
        const string& token = tokens[i];
        int value = 0;

        if (isNumber(token)) value = stoi(token);
        else if (variables.find(token) != variables.end()) value = variables[token];
        else if (token == "+" || token == "-" || token == "*" || token == "/") {
            operation = token[0];
            continue;
        } else throw runtime_error("Error: Invalid token '" + token + "'");

        // Apply arithmetic operation
        if (operation == '+') result += value;
        else if (operation == '-') result -= value;
        else if (operation == '*') result *= value;
        else if (operation == '/') result /= value;
    }

    return result;
}

// Handle function calls
void callFunction(const string& funcName, const vector<string>& args) {
    if (functions.find(funcName) == functions.end()) 
        throw runtime_error("Error: Function '" + funcName + "' is not defined.");

    UserFunction func = functions[funcName];

    // Debug: Print function details
    
    for (const string& arg : args) cout << arg << " ";
    cout << endl;

    // Parameter validation
    if (args.size() != func.params.size()) 
        throw runtime_error("Error: Function '" + funcName + "' expects " + 
                            to_string(func.params.size()) + " arguments, got " + to_string(args.size()));

    map<string, int> oldVariables = variables; // Save variables

    // Assign arguments to function parameters
    for (size_t i = 0; i < args.size(); ++i) {
        if (isNumber(args[i])) variables[func.params[i]] = stoi(args[i]);
        else if (variables.find(args[i]) != variables.end()) variables[func.params[i]] = variables[args[i]];
        else throw runtime_error("Error: Invalid argument '" + args[i] + "'");
    }

    

    // Execute function body
    for (const string& line : func.body) {
        auto commandTokens = tokenize(line);
        
    }

    variables = oldVariables; // Restore variables
}


// Parse and execute commands
void parse(const vector<string>& tokens) {
    if (tokens.empty()) return;

    if (tokens[0] == "print") {
        if (tokens.size() > 1) {
            if (tokens[1][0] == '"') cout << tokens[1].substr(1, tokens[1].size() - 2) << endl;
            else if (variables.find(tokens[1]) != variables.end()) cout << variables[tokens[1]] << endl;
            else cout << evaluateExpression(tokens, 1) << endl;
        } else throw runtime_error("Error: Missing argument for 'print'");
    }

    else if (tokens.size() >= 3 && tokens[1] == "=") {
        string varName = tokens[0];
        variables[varName] = evaluateExpression(tokens, 2);
    }

    else if (tokens[0] == "if") {
        auto thenPos = find(tokens.begin(), tokens.end(), "then");
        auto elsePos = find(tokens.begin(), tokens.end(), "else");

        if (thenPos != tokens.end()) {
            vector<string> conditionTokens(tokens.begin() + 1, thenPos);
            if (conditionTokens.size() == 3) {
                bool condition = evaluateCondition(conditionTokens[0], conditionTokens[1], conditionTokens[2]);
                if (condition) parse(vector<string>(thenPos + 1, (elsePos != tokens.end()) ? elsePos : tokens.end()));
                else if (elsePos != tokens.end()) parse(vector<string>(elsePos + 1, tokens.end()));
            } else throw runtime_error("Error: Invalid condition syntax in 'if' statement");
        } else throw runtime_error("Error: Missing 'then' in 'if' statement");
    }

    else if (tokens[0] == "while") {
        // Ensure correct syntax: while <condition> do
        if (tokens.size() >= 5 && tokens[4] == "do") {
            string varName = tokens[1];
            string operatorSymbol = tokens[2];
            string rightOperand = tokens[3];

            vector<string> loopBody;
            string line;

             
            while (getline(cin, line)) {
                // Trim trailing spaces (optional)
                line.erase(line.find_last_not_of(" \t\n\r") + 1);
                if (line == "end") break; // Properly detect 'end' to stop input
                loopBody.push_back(line);
            }

            // Run the loop with dynamic evaluation of the condition
            while (evaluateCondition(to_string(variables[varName]), operatorSymbol, rightOperand)) {
                for (const string& command : loopBody) {
                    auto commandTokens = tokenize(command);
                    parse(commandTokens);
                }
            }
        } else {
            throw runtime_error("Error: Invalid while loop syntax.");
        }
    }



    else if (tokens[0] == "func") {
        if (tokens.size() < 2) {
            throw runtime_error("Error: Function name missing after 'func'.");
        }

        string funcName = tokens[1];
        vector<string> params(tokens.begin() + 2, tokens.end());
        vector<string> body;
        string line;
        while (getline(cin, line) && line != "end") body.push_back(line);
        functions[funcName] = {params, body};
    }

    else if (tokens[0] == "call") {
        string funcName = tokens[1];
        vector<string> args(tokens.begin() + 2, tokens.end());
        callFunction(funcName, args);
    }

    else if (tokens[0] == "for") {
        if (tokens.size() >= 8 && tokens[2] == "=" && tokens[4] == "to" && tokens[6] == "do") {
            string varName = tokens[1];
            int start = stoi(tokens[3]);
            int end = stoi(tokens[5]);

            for (int i = start; i <= end; ++i) {
                variables[varName] = i;
                parse(vector<string>(tokens.begin() + 7, tokens.end()));
            }
        } else throw runtime_error("Error: Invalid for loop syntax");
    }

    else throw runtime_error("Error: Unknown command '" + tokens[0] + "'");
}