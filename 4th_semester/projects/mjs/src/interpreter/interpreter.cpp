#include "interpreter.h"

void Interpreter::interpretation() {
    parse.analyze();
    executer.execute(parse.poliz);
}