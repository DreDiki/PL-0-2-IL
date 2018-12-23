#ifndef __H_SIMPLE__
#define __H_SIMPLE__

#include <string>
#include <vector>
#include <unordered_map>
#include "OperatorType.h"
#include <memory>

enum class SimpleOp{
    Nop,Label,Jmp,If,Mov,Add,Sub,Mul,Div,Gt,Lt,Eq,Ne,Ge,Le,Call,Odd,Addr
};


SimpleOp OpToSOP(OperatorType op);

class SimpleFun;
class Simple{
public:
    std::string toString(const SimpleFun& ref) const;
public:
    SimpleOp op;
    std::string arg1;
    std::string arg2;
    std::string res;
};

class SimpleVariable{
public:
    std::string identifier;
    //type
    long value;  
};

class SimpleLookup : public std::enable_shared_from_this<SimpleLookup>{
    //std::unordered_map<std::string,SimpleVariable> variables;
public:
    std::unordered_map<std::string,SimpleVariable> globalstatic;
    std::shared_ptr<SimpleLookup> parent;
};

class SimpleFun{
public:
    std::string identifier;
    int level;
    std::vector<Simple> statments;
    std::unordered_map<std::string,SimpleVariable> variables;
    std::unordered_map<std::string,SimpleVariable> statics;
    std::unordered_map<std::string,SimpleVariable> temps;
    std::vector<SimpleVariable> labels;
    std::shared_ptr<SimpleLookup> lookuptable;
};


class SimpleBlock{
public:
    std::vector<Simple> statments;
};

#endif