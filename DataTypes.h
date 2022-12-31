//
// Created by user on 30.12.22.
//

#ifndef KOSTDANI_DATATYPES_H
#define KOSTDANI_DATATYPES_H


#include <string>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <memory>
#include "SECD.h"
class Environment;
class Value;
typedef std::shared_ptr<Value> SEXP;
class Value { ;
protected:

    virtual void print(std::ostream &os) const {}

public:
    friend std::ostream &operator<<(std::ostream &os, const Value &dt);

    virtual bool pair() { return false; }

    virtual bool null() { return false; }
    virtual bool arg() { return false; }

    virtual bool symbol(){return false;}
    virtual bool builtin(){return false;}
    virtual SEXP PartialEval(Environment &env, int d) {return std::make_shared<Value>(*this);};

    virtual void toRPN(std::queue<Code *> &code) {}
};

class Nil : public Value {
    void print(std::ostream &os) const override;

public:
    bool null() override { return true; }

    bool pair() override { return true; }

    SEXP PartialEval(Environment &env, int d) override {return std::make_shared<Nil>(*this);};

    void toRPN(std::queue<Code *> &code) override;
};

class Pair : public Value {
    SEXP car;
    SEXP cdr;

    void print(std::ostream &os) const override;
    SEXP PartialEval(Environment &env, int d) override;

public:
    Pair(SEXP a, SEXP b) : car(a), cdr(b) {}
    SEXP Car(){return car;}
    SEXP Cdr(){return cdr;}
    bool pair() override { return true; }

    void toRPN(std::queue<Code *> &code) override;
};

class Integer : public Value {

    void print(std::ostream &os) const override;

public:
    int integer;
    Integer(int i) : integer(i) {};
    SEXP PartialEval(Environment &env, int d) override;

    bool null() override { return !integer; }

    void toRPN(std::queue<Code *> &code) override;
};

class String : public Value {
    std::string string;

    void print(std::ostream &os) const override;

public:
    String(std::string str) : string(std::move(str)) {}

    bool null() override { return string.empty(); }
    SEXP PartialEval(Environment &env, int d) override {return std::make_shared<String>(*this);};

    void toRPN(std::queue<Code *> &code) override;
};

class Symbol : public Value {
    std::string string;

    void print(std::ostream &os) const override;
    SEXP PartialEval(Environment &env, int d) override;

public:
    Symbol(std::string str) : string(std::move(str)) {}
    bool symbol()override{return true;}
    std::string getName(){return string;}
    void toRPN(std::queue<Code *> &code) override;
};
class Builtin : public Value{
    void print(std::ostream &os) const override;
public:
    Code *c;
    Builtin(Code *c):c(c){}
    void toRPN(std::queue<Code *> &code) override;
    SEXP PartialEval(Environment &env, int d) override {return std::make_shared<Builtin>(*this);};
    bool builtin()override{return true;}

};
    class LambdaArgument:public Value{
        void print(std::ostream &os) const override;
    public:
        int d,p;
        LambdaArgument(int d,int p):d(d),p(p){}
        void toRPN(std::queue<Code *> &code) override;
        bool arg() override { return true; }
    };
    class Closure : public Value{
        void print(std::ostream &os) const override;
    public:
        std::queue<Code *> code;
        std::vector<std::vector<SEXP>> env;
        Closure(std::queue<Code *> code, std::vector<std::vector<SEXP>> env):code(code),env(env){}
    };



class Environment{
    std::map<std::string ,SEXP> env;
public:
    void addSymb(const std::string& sym,SEXP val){
        env[sym]=val;
    }
    SEXP lookup(const std::string& sym) const{
        auto it=env.find(sym);
        if(it==env.end())
            return nullptr;
        else
            return it->second;
    }
};

#endif //KOSTDANI_DATATYPES_H
