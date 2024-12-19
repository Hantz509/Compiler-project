#include "lexer.h"
#include <vector>
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <string>

using namespace std;

vector<string> tokenize(const string& code) {
    vector<string> tokens;
    string token;
    bool inQuotes = false;
    string quotedString;

    istringstream input(code);
    string line;

    while (getline(input, line)) {
        // Ignore lines starting with a comment marker (# or //)
        if (line.empty()) continue;

        size_t commentPos = line.find_first_of("#/");
        if (commentPos != string::npos && (line[commentPos] == '#' || 
            (commentPos + 1 < line.size() && line[commentPos] == '/' && line[commentPos + 1] == '/'))) {
            line = line.substr(0, commentPos); // Remove the comment portion
        }

        // Tokenize the filtered line
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];

            if (c == '"') { // Handle quoted strings
                inQuotes = !inQuotes;
                if (!inQuotes) {
                    tokens.push_back('"' + quotedString + '"'); // Add the full quoted string
                    quotedString.clear();
                }
            } 
            else if (inQuotes) {
                quotedString += c; // Add characters to the quoted string
            } 
            else if (isspace(c)) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            } 
            else if (c == '=' || c == '!' || c == '>' || c == '<') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }

                // Check for two-character operators like "==", "!=", ">=", "<="
                if (i + 1 < line.size() && line[i + 1] == '=') {
                    tokens.push_back(string(1, c) + "="); // Combine current char with '='
                    ++i; // Skip the '=' character
                } else {
                    tokens.push_back(string(1, c)); // Single-character operator
                }
            } 
            else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
                tokens.push_back(string(1, c)); // Add single-character operator
            } 
            else {
                token += c; // Build token
            }
        }

        if (!token.empty()) {
            tokens.push_back(token);
            token.clear();
        }
    }

    if (inQuotes) {
        throw runtime_error("Syntax Error: Missing closing quote (\") in input.");
    }

    return tokens;
}