#include "Tree.h"
#include "util.h"

#define GenerateType(Class,Type) \
    TreeType Class::type() const{ \
    return TreeType::Type; \
}
GenerateType(Tree,Trivial);
GenerateType(ReferenceTree,Reference);
GenerateType(LongValueTree,LongValue);
GenerateType(VariableTree,Variable);
GenerateType(ConstantTree,Constant);
GenerateType(BlockTree,Block);
GenerateType(FunctionTree,Function);
GenerateType(ExpressionTree,Expression);
GenerateType(ConditionTree,Condition);
GenerateType(LoopTree,Loop);
GenerateType(CallTree,Call);
#undef GenerateType

std::shared_ptr<LongValueTree> LongValueTree::fromToken(std::shared_ptr<ValueToken> token){
    auto tree = std::make_shared<LongValueTree>();
    tree->value = token->value;
    return tree;
}

