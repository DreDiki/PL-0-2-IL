#include "OperatorType.h"

std::unordered_map<char,OperatorType> PLOperator::PLOperatorMap = {
    {'=',OperatorType::Equal},
    {',',OperatorType::Comma},{';',OperatorType::Semicolon},{':',OperatorType::Colon},
    {'.',OperatorType::Dot},{'!',OperatorType::Exclamation},{'?',OperatorType::QMark},{'~',OperatorType::Tilde},{'^',OperatorType::Caret},
    {'+',OperatorType::Plus},{'-',OperatorType::Minus},{'*',OperatorType::Multiply},{'/',OperatorType::Division},{'%',OperatorType::Mod},
    {'\'',OperatorType::SingleQuot},{'\"',OperatorType::DoubleQuot},
    {'(',OperatorType::LeftParenth},{')',OperatorType::RightParenth},
    {'#',OperatorType::NotEqual}
};

int Operator::lookup(std::string stack,OperatorType & out)const{
    return 0;
}

int PLOperator::lookup(std::string stack,OperatorType& out)const{
    switch(stack[0]){
        case '>':
            if(stack[1]=='='){
                out = OperatorType::GreaterEqual;
                return 2;
            }else{
                out = OperatorType::Greater;
                return 1;
            }
            break;
        case '<':
            if(stack[1]=='='){
                out = OperatorType::LessEqual;
                return 2;
            }else{
                out = OperatorType::Less;
                return 1;
            }
            break;
        case ':':
            if(stack[1]=='='){
                out = OperatorType::Assign;
                return 2;
            }else{
                
            }
            break;
        default:
        auto search = PLOperatorMap.find(stack[0]);
        if(search != PLOperatorMap.end()){
            out = search->second;
            return 1;
        }
    }
    return 0;
}

std::string Operator::toString(OperatorType op) const{
    return "";
}
std::string PLOperator::toString(OperatorType op) const{
    return PLOperator::ttoString(op);
}
std::string PLOperator::ttoString(OperatorType op){
    switch(op){
        case OperatorType::Assign: return "=";
        case OperatorType::Equal: return "==";
        case OperatorType::NotEqual: return "!=";
        case OperatorType::GreaterEqual: return ">=";
        case OperatorType::Greater: return ">";
        case OperatorType::Less: return "<";
        case OperatorType::LessEqual: return "<=";
        case OperatorType::Comma: return "comma";
        case OperatorType::Colon: return "colon";
        case OperatorType::Plus: return "+";
        case OperatorType::Minus: return "-";
        case OperatorType::Multiply: return "*";
        case OperatorType::Division: return "/";
        case OperatorType::LeftParenth: return "(";
        case OperatorType::RightParenth: return ")";
        case OperatorType::Exclamation: return "!";
        case OperatorType::QMark: return "?";
        case OperatorType::Semicolon: return ";";
        case OperatorType::Dot: return ".";
        case OperatorType::SingleQuot: return "'";
    }
    return "";
}