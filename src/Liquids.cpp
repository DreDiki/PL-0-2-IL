#include "Liquids.h"
#include <sstream>

std::string StringLiquid::toString() const{
    return value;
}

std::string TokensLiquid::toString() const{
    std::ostringstream ss;
    ss<<"Token array,size ";
    ss<<value.size();
    return ss.str();
}

std::string TreeLiquid::toString() const{
    return "TreeLiquid";
}


std::string SimpleLiquid::toString() const{
    std::ostringstream ss;
    for(auto& fun : value){
        ss<<"/** Function "<<fun.identifier <<" **/\n";
        ss<<"/** Begin **/\n";
        for(auto& s : fun.statments){
            ss<<s.toString(fun)<<"\n";
        }
        ss<<"/** End **/\n";
    }
    return ss.str();
}
