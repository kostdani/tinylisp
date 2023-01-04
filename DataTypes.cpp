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
        SEXP p = cdr;
        while (true) {
            if (p->pair()) {
                if (p->null()) {
                    os << ")";
                    break;
                }
                Pair *pair = (Pair *) p.get();
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

    void Nil::toRPN(std::queue<CODE> &code) {
        code.push(std::make_shared<LDNIL>());
    }


    void Pair::toRPN(std::queue<CODE> &code) {

        std::vector<SEXP> list;
        SEXP p=std::make_shared<Pair>(*this);
        while(p->pair()&&!p->null()){
            list.push_back(((Pair*)p.get())->car);
            p=((Pair*)p.get())->cdr;
        }
        list.push_back(p);
        if(list.back()->null()){
            if(list[0]->symbol()) {
                if (((Symbol *) list[0].get())->getName() == "if" && list.size() > 3) {
                    std::queue<CODE> th;
                    std::queue<CODE> el;
                    list[1]->toRPN(code);
                    list[2]->toRPN(th);
                    list[3]->toRPN(el);
                    th.push(std::make_shared<JOIN>());
                    el.push(std::make_shared<JOIN>());
                    code.push(std::make_shared<SEL>(th, el));
                    return;
                }
                if (((Symbol *) list[0].get())->getName() == "lambda" && list.size() > 3) {
                    std::queue<CODE> body;
                    for (int i = 2; i < list.size() - 1; ++i)
                        list[i]->toRPN(body);
                    body.push(std::make_shared<RET>());
                    code.push(std::make_shared<LDF>(body));
                    return;
                }
                if (((Symbol *) list[0].get())->getName() == "quote" && list.size() > 2) {
                    code.push(std::make_shared<LDC>(list[1]));
                    return;
                }
            }
            if(list[0]->builtin()){
                for(int i=list.size()-2;i>0;--i)
                    list[i]->toRPN(code);
                code.push(((Builtin *)list[0].get())->c);
                return;
            }else{//if(list[0]->pair())
                code.push(std::make_shared<LDNIL>());
                for(int i=list.size()-2;i>0;--i){
                    list[i]->toRPN(code);
                    code.push(std::make_shared<CONS>());
                }
                list[0]->toRPN(code);
                code.push(std::make_shared<AP>());
                return;
            }
        }
        throw "not collable";
    }

    void Integer::toRPN(std::queue<CODE> &code) {
        code.push(std::make_shared<LDC>(std::make_shared<Integer>(*this)));
    }

SEXP Integer::PartialEval(Environment &env, int d) {
    return std::make_shared<Integer>(*this);
}

void String::toRPN(std::queue<CODE> &code) {
        code.push(std::make_shared<LDC>(std::make_shared<String>(*this)));
    }

    void Symbol::toRPN(std::queue<CODE> &code) {
        code.push(std::make_shared<LDC>(std::make_shared<Symbol>(*this)));
    }

    void LambdaArgument::toRPN(std::queue<CODE> &code) {
        code.push(std::make_shared<LD>(d,p));
    }

    SEXP Symbol::PartialEval(Environment &env, int d) {


        SEXP v=env.lookup(string);
        if(v.get()==this)
            return v;
        if(v) {
            if(v->arg()){
                auto la=((LambdaArgument *)v.get());
                return std::make_shared<LambdaArgument>(1+d-la->d,la->p);
            }
            return v->PartialEval(env, 0);
        }
        throw std::string("Symbol "+string+" is not associated with any value");
    }

    SEXP Pair::PartialEval(Environment &env, int d) {

        if(car->symbol()&&((Symbol *) car.get())->getName() == "lambda"&& cdr->pair()&& !cdr->null()&& ((Pair *)cdr.get())->car->pair()){
            d++;
            Environment nenv=env;
            SEXP p=((Pair *)cdr.get())->car;
            int pos=0;
            while(p->pair()&&!p->null()){
                nenv.addSymb(((Symbol*)((Pair*)p.get())->car.get())->getName(),std::make_shared<LambdaArgument>(d,pos));
                p=((Pair*)p.get())->cdr;
                ++pos;
            }
            return std::make_shared<Pair>(car,std::make_shared<Pair>(((Pair *)cdr.get())->car,((Pair *)cdr.get())->cdr->PartialEval(nenv,d)));
        }
        return std::make_shared<Pair>(car->PartialEval(env, 0), cdr->PartialEval(env, 0));
    }

    void Builtin::toRPN(std::queue<CODE> &code) {
        code.push(c);
    }

    void Closure::print(std::ostream &os) const {
        os<<"closure!";
    }
