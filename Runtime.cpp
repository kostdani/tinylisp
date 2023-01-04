//
// Created by user on 31.12.22.
//

#include "Runtime.h"

SEXP Runtime::read(){
    return parser.Parse();
}
SEXP Runtime::eval(SEXP v){

    std::queue<CODE> code;

    auto s=v->PartialEval(env,0);
    s->toRPN(code);
    auto sg=s.get();
    secd.code=code;
    return secd.eval();
}
void Runtime::print(SEXP v){
    out<<*v<<std::endl;
}

void Runtime::repl(){
    while(true) {
        try {
            SEXP r=read();
            if(parser.CurTok==END)
                break;
            print(eval(r));
        }
        catch (std::string err) {
            out << "ERROR: " << err << std::endl;
        }
        secd=SECD();
    }
}

Environment StandartRuntime::stdenv(){
    Environment env;
    env.addSymb("if",std::make_shared<Symbol>("if"));
    env.addSymb("lambda",std::make_shared<Symbol>("lambda"));
    env.addSymb("quote",std::make_shared<Symbol>("quote"));
    env.addSymb("+",std::make_shared<Builtin>(std::make_shared<ADD>()));
    env.addSymb("*",std::make_shared<Builtin>(std::make_shared<MUL>()));
    env.addSymb("-",std::make_shared<Builtin>(std::make_shared<SUB>()));
    env.addSymb("cons",std::make_shared<Builtin>(std::make_shared<CONS>()));
    return env;
}