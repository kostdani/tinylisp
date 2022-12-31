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
    Integer* a=(Integer *)secd.stack.top();
    secd.stack.pop();
    Integer* b=(Integer *)secd.stack.top();
    secd.stack.pop();
    secd.stack.push(new Integer(a->integer*b->integer));
}

void ADD::run(SECD &secd) {
    Integer* a=(Integer *)secd.stack.top();
    secd.stack.pop();
    Integer* b=(Integer *)secd.stack.top();
    secd.stack.pop();
    secd.stack.push(new Integer(a->integer+b->integer));
}

void SUB::run(SECD &secd) {
    Integer* a=(Integer *)secd.stack.top();
    secd.stack.pop();
    Integer* b=(Integer *)secd.stack.top();
    secd.stack.pop();
    secd.stack.push(new Integer(a->integer-b->integer));
}

void SEL::run(SECD &secd) {
    Value* cond=secd.stack.top();
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
    secd.stack.push(new Closure(code,secd.environment));
}

void AP::run(SECD &secd) {

    Closure* cls=(Closure *)secd.stack.top();
    secd.stack.pop();
    Value* args=(Pair *)secd.stack.top();
    secd.stack.pop();


    secd.dump.push({secd.stack, secd.environment, secd.code});
    secd.code=cls->code;
    secd.stack={};
    secd.environment={{}};
    while(args->pair()&&!args->null()){
        secd.environment[0].push_back(((Pair *)args)->Car());
        args=((Pair *)args)->Cdr();
    }
    for(auto r:cls->env)
        secd.environment.push_back(r);
}

void RET::run(SECD &secd) {
    Value *res=secd.stack.top();
    secd.code=secd.dump.top().code;
    secd.environment=secd.dump.top().environment;
    secd.stack=secd.dump.top().stack;
    secd.dump.pop();
    secd.stack.push(res);
}

void CONS::run(SECD &secd) {

    Value* a=secd.stack.top();
    secd.stack.pop();
    Value* b=secd.stack.top();
    secd.stack.pop();
    secd.stack.push(new Pair(a,b));
}

void LDNIL::run(SECD &secd) {
    secd.stack.push(new Nil());
}