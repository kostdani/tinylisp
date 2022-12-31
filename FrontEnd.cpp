//
// Created by user on 30.12.22.
//

#include "FrontEnd.h"
void FrontEnd::Lexer::readInput() {
    character=nextChar();

    if (character>='0' && character<='9')
        chartype = CHARTYPE::DIGIT;
    else if (character == EOF)
        chartype = CHARTYPE::EOI;
    else if (character <= ' ')
        chartype = CHARTYPE::WHITESPACE;
    else
        chartype = CHARTYPE::OTHER;
}

char FrontEnd::Lexer::nextChar() {
    return input.get();
}

TOKEN FrontEnd::Lexer::gettok() {
    TOKEN t;
    while(chartype==CHARTYPE::WHITESPACE)
        readInput();
    switch (chartype) {
        case EOI:
            t= TOKEN::END;
            break;
        case DIGIT:
            m_NumVal=0;
            while (chartype==CHARTYPE::DIGIT){
                m_NumVal=10*m_NumVal+(character-'0');
                readInput();
            }
            t= TOKEN::NUMBER;
            break;
        case OTHER:
            switch (character) {
                case '(':
                    readInput();
                    t= TOKEN::LEFTPAR;
                    break;
                case ')':
                    readInput();
                    t= TOKEN::RIGHTPAR;
                    break;
                case '\'':
                    readInput();
                    t= TOKEN::APOSTROPHE;
                    break;
                case '.':
                    readInput();
                    t= TOKEN::DOT;
                    break;
                case '\"':
                    readInput();
                    m_IdentifierStr="";
                    while (character!='\"') {
                        m_IdentifierStr.push_back(character);
                        readInput();
                    }
                    readInput();
                    t=  TOKEN::STRING;
                    break;
                default:
                    m_IdentifierStr="";
                    while (chartype!=CHARTYPE::WHITESPACE && character!=')') {
                        m_IdentifierStr.push_back(character);
                        readInput();
                    }
                    t= TOKEN::SYMBOL;
                    break;
            }
            break;
        default:
            readInput();
            t= TOKEN::UNKNOWN;
            break;
    }
    return t;
}

void CompareError(int s) {

    if(s>0)
        printf("Error while comparing, expected %c.\n",s);
    else

        printf("Error while comparing, expected %d.\n",s);
    exit(1);
}



void FrontEnd::Parser::Compare(TOKEN s) {
    if (CurTok == s)
        getNextToken();
    else
        CompareError(s);
}

bool FrontEnd::Parser::Parse() {
    getNextToken();
    tree=Expression(0);
    return true;
}

Value *FrontEnd::Parser::Atom(int d) {
    Value *v= nullptr;
    switch (CurTok) {
        case NUMBER:
            if(debug)
                printf("(4.1) A -> num (%d)\n",m_Lexer.numVal());
            v=new Integer(m_Lexer.numVal());
            if (d>0)
                getNextToken();
            return v;
        case STRING:
            if(debug)
                printf("(4.2) A -> str (%s)\n",m_Lexer.identifierStr().c_str());
            v=new String(m_Lexer.identifierStr());
            if (d>0)
                getNextToken();
            return v;
        case SYMBOL:
            if(debug)
                printf("(4.3) A -> sym (%s)\n",m_Lexer.identifierStr().c_str());
            v=new Symbol(m_Lexer.identifierStr());
            if (d>0)
                getNextToken();
            return v;
    }
    return nullptr;
}

Value *FrontEnd::Parser::Tail(int d) {
    Value *a;
    Value *b;
    switch (CurTok) {
        case RIGHTPAR:
            if(debug)
                printf("(3.1) T -> e\n");
            return new Nil();
        case DOT:
            if(debug)
                printf("(3.2) T -> . E\n");
            Compare(TOKEN::DOT);
            return Expression(d);
        default:
            if(debug)
                printf("(3.3) T -> E T\n");
            a= Expression(d);
            b= Tail(d);
            return new Pair(a,b);
    }
}

Value *FrontEnd::Parser::List(int d) {
    if(debug)
        printf("(2) T -> (E T)\n");
    Compare(TOKEN::LEFTPAR);
    //Value *a= Expression(d+1);
    Value *b= Tail(d+1);
    if(d>0){
        Compare(TOKEN::RIGHTPAR);
        return b;
    }else if(CurTok==TOKEN::RIGHTPAR)
        return b;
    else
        return new Nil();
}

Value *FrontEnd::Parser::Expression(int d) {
    switch (CurTok) {
        case LEFTPAR:
            if(debug)
                printf("(1.1) E -> L\n");
            return List(d);
        case APOSTROPHE:
            if(debug)
                printf("(1.2) E -> 'E\n");
            Compare(TOKEN::APOSTROPHE);
            return new Pair(new Symbol("quote"),
                            new Pair(Expression(d),
                                     new Nil()));
        default:
            if(debug)
                printf("(1.3) E -> A\n");
            return Atom(d);
    }
}

TOKEN FrontEnd::Parser::getNextToken() {
    return CurTok=m_Lexer.gettok();
}
