#include <iostream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "error_handler.h"
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;

int main() {
    cout << "Welcome to the Simple Compiler!" << endl;
    cout << "Enter your code (type 'exit' to quit):\n";

    while (true) {
        char* input = readline("> "); // Prompt user input
        if (!input) break;            // Exit on EOF (Ctrl+D)
        string code(input);
        if (code == "exit") break;    // Exit on "exit"

        try {
            auto tokens = tokenize(code);
            parse(tokens);
        } catch (const exception& e) {
            reportError(e.what());
        }

        add_history(input); // Add command to history
        free(input);        // Free readline buffer
    }

    return 0;
}
