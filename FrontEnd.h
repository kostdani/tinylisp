//
// Created by user on 30.12.22.
//

#ifndef UNTITLED_FRONTEND_H
#define UNTITLED_FRONTEND_H
#include <string>
#include <istream>
#include "DataTypes.h"

enum TOKEN {
    LEFTPAR,
    RIGHTPAR,
    DOT,
    APOSTROPHE,
    SYMBOL,
    STRING,
    NUMBER,
    UNKNOWN,
    END} ;
enum CHARTYPE {
    DIGIT,
    WHITESPACE,
    EOI,
    OTHER
};
    class Lexer {
        std:: istream& input;
        std::string m_IdentifierStr;
        int m_NumVal;
        char character;
        CHARTYPE chartype; // input symbol type
        void readInput(void);
    public:
        Lexer(std::istream& in):input(in) {readInput();};
        ~Lexer() = default;
        char nextChar();
        TOKEN gettok();
        const std::string& identifierStr() const { return this->m_IdentifierStr; }
        int numVal() { return this->m_NumVal; }
    };

    class Parser {
        bool debug=false;
        TOKEN getNextToken();

        Lexer m_Lexer;                   // lexer is used to read tokens
        TOKEN CurTok;                      // to keep the current token

        void Compare(TOKEN s);

        SEXP Atom(int d);
        SEXP Tail(int d);
        SEXP List(int d);
        SEXP Expression(int d);
    public:
        Parser(std::istream& in): m_Lexer(in){};

        ~Parser() {

        };
        SEXP Parse();                    // parse
    };


#endif //UNTITLED_FRONTEND_H
