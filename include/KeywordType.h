#ifndef __H_KEYWORDTYPE__
#define __H_KEYWORDTYPE__

#include <string>
#include <unordered_map>
class Keyword{
public:
    virtual int lookup(std::string word) const;
    virtual std::string toString(int symbol) const;
};

enum class PLKeywordType {
    Const,Var,Procedure,Call,Begin,End,If,Then,While,Do,Read,Write,Odd
};

class PLKeyword : public Keyword{
public:
    int lookup(std::string word) const override;
    std::string toString(int symbol) const override;
protected:
    static std::unordered_map<std::string,PLKeywordType> PLKeywordMap;
};

#endif