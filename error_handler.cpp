#include "error_handler.h"
#include <iostream>

using namespace std;

void reportError(const string& message) {
    cerr << "Compiler Error: " << message << endl;
}
