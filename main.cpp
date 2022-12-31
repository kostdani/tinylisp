#include "FrontEnd.h"
#include <sstream>
#include <iostream>
#include "SECD.h"
#include "Runtime.h"
int main() {
    std::istringstream ss("(+ 1 ((lambda (x y) (+ x y)) 10 20))");/*
    FrontEnd fe(ss);
    auto sexp=fe.parser.tree;
    std::cout<<*sexp<<std::endl;
    std::queue<Code *> code;

    Environment env;
    env.addSymb("if",new Symbol("if"));
    env.addSymb("lambda",new Symbol("lambda"));
    env.addSymb("+",new Builtin(new ADD()));
    auto s=sexp->PartialEval(env,0);
    std::cout<<*s<<std::endl;
    s->toRPN(code);
    auto ccode=code;
    while(!ccode.empty()){
        std::cout<<*ccode.front()<<std::endl;
        ccode.pop();
    }
    SECD secd;
    secd.code=code;
    Value* res=secd.eval();
    std::cout<<*res;
*/
    StandartRuntime sr;
    sr.repl();
    return 0;
}
