//
// Created by user on 31.12.22.
//

#ifndef TINYLIST_RUNTIME_H
#define TINYLIST_RUNTIME_H
#include "DataTypes.h"
#include "FrontEnd.h"
#include "SECD.h"
class Runtime {
Parser parser;
std::ostream &out;
Environment env;
SECD secd;
SEXP read(){
    return parser.Parse();
}
SEXP eval(SEXP v){

    std::queue<CODE> code;

    auto s=v->PartialEval(env,0);
    s->toRPN(code);
    auto sg=s.get();
    secd.code=code;
    return secd.eval();
}
void print(SEXP v){
    out<<*v<<std::endl;
}
public:
    Runtime(std::istream& in,std::ostream &out,Environment env): parser(in),out(out),env(env){}
    void repl(){
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
};


class StandartRuntime: public Runtime{
    Environment sdenv(){
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
public:
    StandartRuntime(): Runtime(std::cin,std::cout,sdenv()){}
};
#endif //TINYLIST_RUNTIME_H
