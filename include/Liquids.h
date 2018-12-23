#ifndef __H_LIQUIDS__
#define __H_LIQUIDS__

#include <vector>
#include <string>
#include "Liquid.h"
#include "Token.h"
#include <memory>
#include "Simple.h"
class Tree;
class StringLiquid: public Liquid{
public:
    std::string toString() const override;
public:
    std::string value;
};

class TokensLiquid : public Liquid{
public:
    std::string toString() const override;
public:
    std::vector<std::shared_ptr<Token>> value;
};

class TreeLiquid : public Liquid{
public:
    std::string toString() const override;
public:
    std::shared_ptr<Tree> value;
};

class SimpleLiquid : public Liquid{
public:
    std::string toString() const override;
public:
    std::vector<SimpleFun> value;
};

#endif