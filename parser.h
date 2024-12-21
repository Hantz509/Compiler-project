#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <map>

using namespace std;

// Structure to hold function details
struct UserFunction {
    vector<string> params;       // Parameters of the function
    vector<string> body;         // Body of the function (as lines of code)
};

// Function declarations
void parse(const vector<string>& tokens);

// Map to store all defined functions
extern map<string, UserFunction> functions;

#endif