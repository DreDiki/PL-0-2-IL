#ifndef __H_TREE__
#define __H_TREE__

#include <memory>
#include <vector>
#include <string>
#include "Token.h"
#include "util.h"

enum class TreeType {
    Trivial,LongValue,Reference,Variable,Constant,Block,Function,Expression,Condition,Loop,Call
};

//tree base
class LongValueTree;
class VariableTree;
class ConstantTree;
class BlockTree;
class FunctionTree;
class ReferenceTree;
class ExpressionTree; 
class ConditionTree; 
class LoopTree; 
class CallTree; 

class Tree : public std::enable_shared_from_this<Tree>
{
public:
    virtual TreeType type() const;

    Convert(LongValueTree,Tree);
    Convert(ReferenceTree,Tree);
    Convert(VariableTree,Tree);
    Convert(ConstantTree,Tree);
    Convert(BlockTree,Tree);
    Convert(FunctionTree,Tree);
    Convert(ExpressionTree,Tree);    
    Convert(ConditionTree,Tree);
    Convert(LoopTree,Tree);
    Convert(CallTree,Tree);
public:
    std::vector<std::shared_ptr<Tree>> childs;
    std::shared_ptr<Tree> parent;
};

//define value
class LongValueTree : public Tree{
public:
    long value;
    static std::shared_ptr<LongValueTree> fromToken(std::shared_ptr<ValueToken> token);
    virtual TreeType type() const override;

};


class ReferenceTree : public Tree{
public:
    std::string identifier;
    virtual TreeType type() const override;
};


class CallTree : public ReferenceTree{
public:
    virtual TreeType type() const override;
};

//define constant
class VariableTree: public ReferenceTree{
public:
    std::shared_ptr<Tree> initial;
    virtual TreeType type() const override;

};

//define variable
class ConstantTree: public VariableTree{
public:
    virtual TreeType type() const override;

};

class BlockTree : public Tree{
public:
    std::vector<std::shared_ptr<ConstantTree>> constants;
    std::vector<std::shared_ptr<VariableTree>> variables;
    virtual TreeType type() const override;

};

//define function
class FunctionTree : public Tree{
public:
    virtual TreeType type() const override;
    std::string identifier;
};

//define a type
class TypeTree: public Tree{
    
};

enum class OperationType{
    None,Unary,Binary,Ternary
};
class ExpressionTree :public Tree{
public:
    virtual TreeType type() const override;
    OperationType otype;
    OperatorType op;
};


class ConditionTree : public Tree{
public:
    virtual TreeType type() const override;
    std::shared_ptr<Tree> condition;    
};

class LoopTree : public Tree{
public:
    virtual TreeType type() const override;
    std::shared_ptr<Tree> condition;        
};


class StatementTree : public Tree{

};

//define expression


#endif