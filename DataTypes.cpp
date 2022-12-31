//
// Created by user on 30.12.22.
//

#include "DataTypes.h"
    std::ostream &operator<<(std::ostream &os, const Value &v) {
        v.print(os);
        return os;
    }

    void Nil::print(std::ostream &os) const {
        os << "()";
    }

    void Pair::print(std::ostream &os) const {
        os << "(" << *car;
        Value *p = cdr;
        while (true) {
            if (p->pair()) {
                if (p->null()) {
                    os << ")";
                    break;
                }
                Pair *pair = (Pair *) p;
                os << " " << *pair->car;
                p = pair->cdr;
            } else {
                os << " . " << *p << ")";
                break;
            }
        }
    }

    void Integer::print(std::ostream &os) const {
        os << integer;
    }

    void String::print(std::ostream &os) const {
        os << '"' << string << '"';
    }

    void Symbol::print(std::ostream &os) const {
        os << string;
    }

    void Builtin::print(std::ostream &os) const {
        os<<"<"<<*c<<">";
    }

    void LambdaArgument::print(std::ostream &os) const {
        os<<"("<<d<<"."<<p<<")";
    }

    void Nil::toRPN(std::queue<Code *> &code) {
        code.push(new LDNIL());
    }


    void Pair::toRPN(std::queue<Code *> &code) {

        std::vector<Value *> list;
        Value *p=this;
        while(p->pair()&&!p->null()){
            list.push_back(((Pair*)p)->car);
            p=((Pair*)p)->cdr;
        }
        list.push_back(p);
        if(list.back()->null()){
            if(list[0]->symbol()) {
                if (((Symbol *) list[0])->getName() == "if" && list.size() > 3) {
                    std::queue<Code *> th;
                    std::queue<Code *> el;
                    list[1]->toRPN(code);
                    list[2]->toRPN(th);
                    list[3]->toRPN(el);
                    th.push(new JOIN());
                    el.push(new JOIN());
                    code.push(new SEL(th, el));
                    return;
                }
                if (((Symbol *) list[0])->getName() == "lambda" && list.size() > 3) {
                    std::queue<Code *> body;
                    for (int i = 2; i < list.size() - 1; ++i)
                        list[i]->toRPN(body);
                    body.push(new RET());
                    code.push(new LDF(body));
                    return;
                }
            }
            if(list[0]->pair()&& ((Pair *)list[0])->car->symbol() && ((Symbol *)((Pair *)list[0])->car)->getName()=="lambda"){
                code.push(new LDNIL());
                for(int i=list.size()-2;i>0;--i){
                    list[i]->toRPN(code);
                    code.push(new CONS());
                }

                list[0]->toRPN(code);
                code.push(new AP());
                return;

            }

        }
        if(!(cdr->null()&&cdr->pair()))
            cdr->toRPN(code);
        car->toRPN(code);
    }

    void Integer::toRPN(std::queue<Code *> &code) {
        code.push(new LDC(this));
    }

    void String::toRPN(std::queue<Code *> &code) {

    }

    void Symbol::toRPN(std::queue<Code *> &code) {
        if (string == "+")
            code.push(new ADD());
        else if (string == "*")
            code.push(new MUL());
        else if (string == "-")
            code.push(new SUB());
    }

    void LambdaArgument::toRPN(std::queue<Code *> &code) {
        code.push(new LD(d,p));
    }

    Value * Symbol::PartialEval(Environment &env, int d) {


        Value *v=env.lookup(string);
        if(v==this)
            return v;
        if(v) {
            if(v->arg()){
                auto la=((LambdaArgument *)v);
                return new LambdaArgument(1+d-la->d,la->p);
            }
            return v->PartialEval(env, 0);
        }
        throw 0;
    }

    Value * Pair::PartialEval(Environment &env, int d) {

        if(car->symbol()&&((Symbol *) car)->getName() == "lambda"&& cdr->pair()&& !cdr->null()&& ((Pair *)cdr)->car->pair()){
            d++;
            Environment nenv=env;
            Value *p=((Pair *)cdr)->car;
            int pos=0;
            while(p->pair()&&!p->null()){
                nenv.addSymb(((Symbol*)((Pair*)p)->car)->getName(),new LambdaArgument(d,pos));
                p=((Pair*)p)->cdr;
                ++pos;
            }
            return new Pair(car,new Pair(((Pair *)cdr)->car,((Pair *)cdr)->cdr->PartialEval(nenv,d)));
        }
        return new Pair(car->PartialEval(env, 0), cdr->PartialEval(env, 0));
    }

    void Builtin::toRPN(std::queue<Code *> &code) {
        code.push(c);
    }

    void Closure::print(std::ostream &os) const {
        os<<"closure!";
    }
