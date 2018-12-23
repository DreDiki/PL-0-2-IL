#ifndef __H_STRINGPIPETOKENS__
#define __H_STRINGPIPETOKENS__

#include "Pipe.h"
#include <string>

class Token;

class StringPipeTokens : public Pipe{
public:
    std::shared_ptr<Liquid> flow(std::shared_ptr<Liquid> in) const override;
    std::string locate(std::shared_ptr<Liquid> in,Location location) const override;
protected:
    std::shared_ptr<Token> parseWord(const std::string & word) const;
    std::shared_ptr<Token> parseNumber(size_t &begin,const std::string & line) const;

};

#endif