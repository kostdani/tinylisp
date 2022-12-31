//
// Created by user on 31.12.22.
//

#ifndef TINYLIST_RUNTIME_H
#define TINYLIST_RUNTIME_H
#include "DataTypes.h"
#include "FrontEnd.h"
#include "SECD.h"
class Runtime {
FrontEnd front;
std::ostream &out;
Environment env;
SECD secd;
Value *read(){
    front.parser.Parse();
    return front.parser.tree;
}
Value *eval(Value *v){

    std::queue<Code *> code;

    auto s=v->PartialEval(env,0);
    //std::cout<<*s<<std::endl;
    s->toRPN(code);
    secd.code=code;
    return secd.eval();
}
void print(Value *v){
    out<<*v<<std::endl;
}
public:
    Runtime(std::istream& in,std::ostream &out,Environment env): front(in),out(out),env(env){}
    void repl(){
        while(true)
            print(eval(read()));
    }
};


class StandartRuntime: public Runtime{
    Environment sdenv(){
        Environment env;
        env.addSymb("if",new Symbol("if"));
        env.addSymb("lambda",new Symbol("lambda"));
        env.addSymb("+",new Builtin(new ADD()));
        env.addSymb("*",new Builtin(new MUL()));
        env.addSymb("-",new Builtin(new SUB()));
        env.addSymb("cons",new Builtin(new CONS()));
        return env;
    }
public:
    StandartRuntime(): Runtime(std::cin,std::cout,sdenv()){}
};
#endif //TINYLIST_RUNTIME_H
