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
    SEXP read();
    SEXP eval(SEXP v);
    void print(SEXP v);
public:
    Runtime(std::istream& in,std::ostream &out,Environment env): parser(in),out(out),env(env){}
    void repl();
};


class StandartRuntime: public Runtime{
    Environment stdenv();
public:
    StandartRuntime(): Runtime(std::cin,std::cout,stdenv()){}
};
#endif //TINYLIST_RUNTIME_H
