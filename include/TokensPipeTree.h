#ifndef __H_TOKENSPIPETREE__
#define __H_TOKENSPIPETREE__

#include "Pipe.h"
#include <string>
#include "Tree.h"
class Token;

class TokensPipeTree : public Pipe{
public:
    std::shared_ptr<Liquid> flow(std::shared_ptr<Liquid> in) const override;
    std::string locate(std::shared_ptr<Liquid> in,Location location) const override;
protected:
    std::shared_ptr<Tree> parseProgram(std::vector<std::shared_ptr<Token>> &container,size_t& i) const;
    std::shared_ptr<Tree> parseBlock(std::vector<std::shared_ptr<Token>> &container,size_t& i) const;
    std::shared_ptr<Tree> parseStatement(std::vector<std::shared_ptr<Token>> &container,size_t& i) const;
    std::shared_ptr<Tree> parseCondition(std::vector<std::shared_ptr<Token>> &container,size_t& i) const;    
    std::shared_ptr<Tree> parseExpression(std::vector<std::shared_ptr<Token>> &container,size_t& i) const;
    std::shared_ptr<Tree> parseTerm(std::vector<std::shared_ptr<Token>> &container,size_t& i) const;
    std::shared_ptr<Tree> parseFactor(std::vector<std::shared_ptr<Token>> &container,size_t& i) const;
};


#endif