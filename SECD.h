//
// Created by user on 30.12.22.
//

#ifndef TINYLIST_SECD_H
#define TINYLIST_SECD_H
#include <iostream>
#include <stack>
#include <utility>
#include <vector>
#include <queue>

class SECD;
class Value;

class Code{
    virtual void print(std::ostream & os) const{};
public:
    friend std::ostream& operator<<(std::ostream& os, const Code& dt);
    virtual void run(SECD &secd){};
};
class LDNIL:public Code{

    void print(std::ostream & os) const override;

public:
    void run(SECD &secd) override;
};
class LDC:public Code{
    Value * c;
    void print(std::ostream & os) const override;

public:
    LDC(Value* c):c(c){}
    void run(SECD &secd) override;
};
class MUL:public Code{
    void print(std::ostream & os) const override;
public:
    void run(SECD &secd) override;

};
class ADD:public Code{
    void print(std::ostream & os) const override;
public:
    void run(SECD &secd) override;

};
class SUB:public Code{
    void print(std::ostream & os) const override;
public:
    void run(SECD &secd) override;

};
class SEL:public Code{
    std::queue<Code *> th;
    std::queue<Code *> el;
    void print(std::ostream & os) const override;
public:
    SEL(std::queue<Code *> t,std::queue<Code *>e):th(std::move(t)),el(std::move(e)){}
    void run(SECD &secd) override;
};
class JOIN: public Code{
    void print(std::ostream & os) const override;

public:
    void run(SECD &secd) override;
};
class LD: public Code{
    int d,p;
    void print(std::ostream & os) const override;

public:
    LD(int d,int p):d(d),p(p){}
    void run(SECD &secd) override;
};

class LDF: public Code{
    std::queue<Code *> code;
    void print(std::ostream & os) const override;
public:
    LDF(std::queue<Code *> c):code(c){}
    void run(SECD &secd) override;
};

class AP:public Code{
    void print(std::ostream & os) const override;
public:
    void run(SECD &secd) override;
};

class RET:public Code{
    void print(std::ostream & os) const override;
public:
    void run(SECD &secd) override;

};

class CONS:public Code{

    void print(std::ostream & os) const override;
public:
    void run(SECD &secd) override;
};

struct SEC{

    std::stack<Value *> stack;
    std::vector<std::vector<Value *>> environment;
    std::queue<Code *> code;
    SEC(std::stack<Value *> stack,std::vector<std::vector<Value *>> environment,std::queue<Code *> code):stack(stack),environment(environment),code(code){}
};
class SECD{
public:
 std::stack<Value *> stack;
 std::queue<Code *> code;
 std::stack<SEC> dump;
 std::vector<std::vector<Value *>> environment;
    Value * eval(){
     while(!code.empty()) {
         auto c = code.front();
         code.pop();
         c->run(*this);
     }
     return stack.top();
 }
};

#endif //TINYLIST_SECD_H
