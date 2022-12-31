//
// Created by user on 30.12.22.
//

#include "SECD.h"
#include "DataTypes.h"
std::ostream& operator<<(std::ostream& os, const Code& v)
{
    v.print(os);
    return os;
}

void LDC::print(std::ostream &os) const {
    os<<"LDC "<<*c;
}

void LDNIL::print(std::ostream &os) const {
    os<<"NIL";
}

void MUL::print(std::ostream &os) const {
    os<<"MUL";
}

void ADD::print(std::ostream &os) const {
    os<<"ADD";
}

void SUB::print(std::ostream &os) const {
    os<<"SUB";
}

void SEL::print(std::ostream &os) const {
    os<<"SEL ({\n";
    auto thc=th;
    auto elc=el;
    while(!thc.empty()){
        os<<"    "<<*thc.front()<<std::endl;
        thc.pop();
    }
    os<<"},{\n";
    while(!elc.empty()){
        os<<"    "<<*elc.front()<<std::endl;
        elc.pop();
    }
    os<<"})\n";
}

void JOIN::print(std::ostream &os) const {
    os<<"JOIN";
}

void LD::print(std::ostream &os) const {
    os<<"LD ("<<d<<"."<<p<<")";
}

void LDF::print(std::ostream &os) const {
    os<<"LDF {\n";
    auto ccode=code;
    while(!ccode.empty()){
        os<<"    "<<*ccode.front()<<std::endl;
        ccode.pop();
    }
    os<<"}\n";
}

void AP::print(std::ostream &os) const {
    os<<"AP";
}

void RET::print(std::ostream &os) const {
    os<<"RET";
}

void CONS::print(std::ostream &os) const {
    os<<"CONS";
}
void LDC::run(SECD &secd) {
    secd.stack.push(c);
}

void MUL::run(SECD &secd) {
    SEXP f=secd.stack.top();
    secd.stack.pop();
    SEXP s=secd.stack.top();
    secd.stack.pop();
    Integer* a=dynamic_cast<Integer *>(f.get());
    Integer* b=dynamic_cast<Integer *>(s.get());
    if(!a||!b)
        throw std::string("Can only multiply numbers");
    secd.stack.push(std::make_shared<Integer>(a->integer*b->integer));
}

void ADD::run(SECD &secd) {
    SEXP f=secd.stack.top();
    secd.stack.pop();
    SEXP s=secd.stack.top();
    secd.stack.pop();
    Integer* a=dynamic_cast<Integer *>(f.get());
    Integer* b=dynamic_cast<Integer *>(s.get());
    if(!a||!b)
        throw std::string("Can only add numbers");
    secd.stack.push(std::make_shared<Integer>(a->integer+b->integer));
}

void SUB::run(SECD &secd) {
    SEXP f=secd.stack.top();
    secd.stack.pop();
    SEXP s=secd.stack.top();
    secd.stack.pop();
    Integer* a=dynamic_cast<Integer *>(f.get());
    Integer* b=dynamic_cast<Integer *>(s.get());
    if(!a||!b)
        throw std::string("Can only subtract numbers");
    secd.stack.push(std::make_shared<Integer>(a->integer-b->integer));
}

void SEL::run(SECD &secd) {
    SEXP cond=secd.stack.top();
    secd.stack.pop();
    secd.dump.push({secd.stack, secd.environment, secd.code});
    secd.code=!cond->null()?th:el;
}

void JOIN::run(SECD &secd) {
    secd.code=secd.dump.top().code;
    secd.dump.pop();
}
void LD::run(SECD &secd) {
    secd.stack.push(secd.environment[d][p]);
}

void LDF::run(SECD &secd) {
    secd.stack.push(std::make_shared< Closure>(code,secd.environment));
}

void AP::run(SECD &secd) {
    SEXP f=secd.stack.top();
    secd.stack.pop();
    SEXP s=secd.stack.top();
    secd.stack.pop();

    Closure* cls=dynamic_cast<Closure *>(f.get());
    Value* args=dynamic_cast<Pair *>(s.get());
    if(!cls || !args)
        throw std::string("Not collable");


    secd.dump.push({secd.stack, secd.environment, secd.code});
    secd.code=cls->code;
    secd.stack={};
    secd.environment={{}};
    while(args->pair()&&!args->null()){
        secd.environment[0].push_back(((Pair *)args)->Car());
        args=((Pair *)args)->Cdr().get();
    }
    for(auto r:cls->env)
        secd.environment.push_back(r);
}

void RET::run(SECD &secd) {
    SEXP res=secd.stack.top();
    secd.code=secd.dump.top().code;
    secd.environment=secd.dump.top().environment;
    secd.stack=secd.dump.top().stack;
    secd.dump.pop();
    secd.stack.push(res);
}

void CONS::run(SECD &secd) {

    SEXP a=secd.stack.top();
    secd.stack.pop();
    SEXP b=secd.stack.top();
    secd.stack.pop();
    secd.stack.push(std::make_shared< Pair>(a,b));
}

void LDNIL::run(SECD &secd) {
    secd.stack.push(std::make_shared< Nil>());
}