#ifndef __H_TOKEN__
#define __H_TOKEN__
#include "OperatorType.h"
#include "KeywordType.h"
#include "util.h"
#include <memory>
enum class TokenType {
    Void,Operator,Keyword,Ident,Const
};


class OperatorToken;
class KeywordToken;
class IdentToken;
class ValueToken;
class Token : public std::enable_shared_from_this<Token>{
public:
    Token(){};
    virtual TokenType type() const;
    Convert(OperatorToken,Token);
    Convert(IdentToken,Token);
    Convert(ValueToken,Token);
    Convert(KeywordToken,Token);

    //std::string ref;
    long line;
};

class OperatorToken : public Token{
public:
    OperatorToken(){}
    OperatorType value;
    virtual TokenType type() const override;
};



class KeywordToken: public Token{
public:
    KeywordToken(){}
    int value;//for general use
    virtual TokenType type() const override;
};

class IdentToken : public Token{
public:
    IdentToken(){}
    std::string value;
    virtual TokenType type() const override;
};

class ValueToken : public Token{
public:
    ValueToken(){}
    long value;
    virtual TokenType type() const override;
};


#endif