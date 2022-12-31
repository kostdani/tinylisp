//
// Created by user on 30.12.22.
//

#include "FrontEnd.h"
void Lexer::readInput() {
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

char Lexer::nextChar() {
    if(character==EOF)
        return EOF;
    int c= input.get();
    if(c==-1) {
        c = EOF;
    }
    return c;
}

TOKEN Lexer::gettok() {
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



void Parser::Compare(TOKEN s) {
    if (CurTok == s)
        getNextToken();
    else
        CompareError(s);
}

SEXP  Parser::Parse() {
    getNextToken();
    if(CurTok==END)
        return std::make_shared<Nil>();
    return Expression(0);
}

SEXP Parser::Atom(int d) {
    SEXP v;
    switch (CurTok) {
        case NUMBER:
            if(debug)
                printf("(4.1) A -> num (%d)\n",m_Lexer.numVal());
            v=std::make_shared<Integer>(m_Lexer.numVal());
            if (d>0)
                getNextToken();
            return v;
        case STRING:
            if(debug)
                printf("(4.2) A -> str (%s)\n",m_Lexer.identifierStr().c_str());
            v=std::make_shared<String>(m_Lexer.identifierStr());
            if (d>0)
                getNextToken();
            return v;
        case SYMBOL:
            if(debug)
                printf("(4.3) A -> sym (%s)\n",m_Lexer.identifierStr().c_str());
            v=std::make_shared<Symbol>(m_Lexer.identifierStr());
            if (d>0)
                getNextToken();
            return v;
    }
    return nullptr;
}

SEXP Parser::Tail(int d) {
    SEXP a;
    SEXP b;
    switch (CurTok) {
        case RIGHTPAR:
            if(debug)
                printf("(3.1) T -> e\n");
            return std::make_shared< Nil>();
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
            return std::make_shared<Pair>(a,b);
    }
}

SEXP Parser::List(int d) {
    if(debug)
        printf("(2) T -> (E T)\n");
    Compare(TOKEN::LEFTPAR);
    //Value *a= Expression(d+1);
    SEXP b= Tail(d+1);
    if(d>0){
        Compare(TOKEN::RIGHTPAR);
        return b;
    }else if(CurTok==TOKEN::RIGHTPAR)
        return b;
    else
        return std::make_shared< Nil>();
}

SEXP Parser::Expression(int d) {
    switch (CurTok) {
        case LEFTPAR:
            if(debug)
                printf("(1.1) E -> L\n");
            return List(d);
        case APOSTROPHE:
            if(debug)
                printf("(1.2) E -> 'E\n");
            Compare(TOKEN::APOSTROPHE);
            return std::make_shared< Pair>(std::make_shared< Symbol>("quote"),
                            std::make_shared< Pair>(Expression(d),
                                     std::make_shared< Nil>()));
        default:
            if(debug)
                printf("(1.3) E -> A\n");
            return Atom(d);
    }
}

TOKEN Parser::getNextToken() {
    if(CurTok==END)
        return END;
    return CurTok=m_Lexer.gettok();
}
