#include "KeywordType.h"
#include <algorithm>
#include <locale>
#define MapKey(string,key) {#string,PLKeywordType::key}
std::unordered_map<std::string,PLKeywordType> PLKeyword::PLKeywordMap = {
    MapKey(const,Const),
    MapKey(var,Var),
    MapKey(procedure,Procedure),
    MapKey(call,Call),
    MapKey(begin,Begin),
    MapKey(end,End),
    MapKey(if,If),
    MapKey(then,Then),
    MapKey(while,While),
    MapKey(do,Do),
    MapKey(read,Read),
    MapKey(write,Write),
    MapKey(odd,Odd)
};

#undef MapKey

std::string Keyword::toString(int symbol) const{
    return "";
}

std::string PLKeyword::toString(int symbol) const{
    std::string result;
    std::any_of(PLKeywordMap.begin(), PLKeywordMap.end(), [symbol,&result](auto &n){ 
        if((int)n.second== symbol){
            result = n.first;
            return true;
        }
        return false;
    });
    return result;
}

int Keyword::lookup(std::string word) const{
        return -1;
}

int PLKeyword::lookup(std::string word) const{
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    auto key = PLKeywordMap.find(word);
    if(key!=PLKeywordMap.end()){
        return (int)key->second;
    }
    return -1;
}
