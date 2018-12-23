#include "Simple.h"
#include <algorithm>
using namespace std::string_literals;

SimpleOp OpToSOP(OperatorType op){
    switch(op){
        case OperatorType::Plus:
            return SimpleOp::Add;
        case OperatorType::Minus:
            return SimpleOp::Sub;
        case OperatorType::Multiply:
            return SimpleOp::Mul;
        case OperatorType::Division:
            return SimpleOp::Div;
        case OperatorType::Assign:
            return SimpleOp::Mov;
        case OperatorType::Equal:
            return SimpleOp::Eq;
        case OperatorType::NotEqual:
            return SimpleOp::Ne;
        case OperatorType::LessEqual:
            return SimpleOp::Le;
        case OperatorType::GreaterEqual:
            return SimpleOp::Ge;
        case OperatorType::Less:
            return SimpleOp::Lt;
        case OperatorType::Greater:
            return SimpleOp::Gt;
        case OperatorType::SingleQuot:
            return SimpleOp::Odd;
        default:
            return SimpleOp::Nop;
    }
}

std::string Simple::toString(const SimpleFun& ref) const{
    auto stos = [&ref](std::string a){
        if(a.size()>1&&a.front()=='\0'){
            if(a[1]=='S'){
                auto i = ref.statics.find(a);
                if(i!=ref.statics.end())
                    return std::to_string(i->second.value);
            }
            //ref.statics.
            return a.substr(1);
        }
        return a;
    };
    std::string s="\t"s;
    switch(op){
        case SimpleOp::Label:
            return s=stos(arg1)+":"s;
        case SimpleOp::Add:
            s+="add \t"s;
            break;
        case SimpleOp::Call:
            s+="call\t"s;
            break;
        case SimpleOp::Div:
            s+="div \t"s;
            break;
        case SimpleOp::If:
            s+="if  \t"s;
            break;
        case SimpleOp::Jmp:
            s+="jmp \t"s;
            break;
        case SimpleOp::Mul:
            s+="mul \t"s;
            break;
        case SimpleOp::Nop:
            s+="nop \t"s;
            break;
        case SimpleOp::Sub:
            s+="sub \t"s;
            break;
        case SimpleOp::Eq:
            s+="eq  \t"s;
            break;
        case SimpleOp::Ne:
            s+="ne  \t"s;
            break;
        case SimpleOp::Le:
            s+="le  \t"s;
            break;
        case SimpleOp::Ge:
            s+="ge  \t"s;
            break;
        case SimpleOp::Lt:
            s+="lt  \t"s;
            break;
        case SimpleOp::Gt:
            s+="gt  \t"s;
            break;
        case SimpleOp::Mov:
            s+="mov \t"s;
            break;
        case SimpleOp::Odd:
            s+="odd \t"s;
            break;
        case SimpleOp::Addr:
            s+="addr\t"s;
            break;
        default:
            s+="??? \t"s;
            break;
        // case SimpleOp::Mov:
        //     s+="mov \t"s;
        //     break;
    }
    s+=stos(arg1);
    if(arg2.size()>0){
        s+=" , "+stos(arg2);
    }
    if(res.size()>0){
        s+=" => "+stos(res);
    }
    return s;
}