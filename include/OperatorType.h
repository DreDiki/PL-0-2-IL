#ifndef __H_OPERATORTYPE__
#define __H_OPERATORTYPE__
#include <unordered_map>
#include <string>

enum class OperatorType: int{
    None,
    Assign, // =
    Comma,Semicolon,Colon, // ,;:
    Dot,Exclamation,QMark,Tilde,Caret, // .!?~^
    Plus,Minus,Multiply,Division,Mod, // +-*/%
    SingleQuot,DoubleQuot, // ' "
    LeftParenth,RightParenth, // ()
    LeftSquare,RightSquare, // []
    LeftCurly,RightCurly,//{}
    LeftArrow,RightArrow,//<- ->
    Less,Equal,NotEqual,Greater, // < == != >
    LessEqual,GreaterEqual, //<= >=
    NumberSign//#
};


constexpr int MaxOperatorLength=2;

class Operator {
public:
    virtual int lookup(std::string stack,OperatorType & out)const;
    virtual std::string toString(OperatorType op) const;
};

class PLOperator:public Operator{
public:
    int lookup(std::string stack,OperatorType & out) const override;
    virtual std::string toString(OperatorType op) const override;
    static std::string ttoString(OperatorType op);
protected:
    static std::unordered_map<char,OperatorType> PLOperatorMap;
};


#endif // __H_OPERATORTYPE__
