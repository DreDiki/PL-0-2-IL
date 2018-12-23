#ifndef __H_TREEPIPESIMPLE__
#define __H_TREEPIPESIMPLE__

#include "Pipe.h"
#include "Simple.h"
#include "Tree.h"

#include <vector>
class TreePipeSimple : public Pipe{
public:
    std::shared_ptr<Liquid> flow(std::shared_ptr<Liquid> in) const override;
    std::string locate(std::shared_ptr<Liquid> in,Location location) const override;
protected:
    std::string parseBlock(std::shared_ptr<Tree> root,std::vector<SimpleFun>& funs) const;

    std::string parseToString(std::shared_ptr<Tree> root,SimpleFun& fun) const;
    std::string parseStatement(std::shared_ptr<Tree> root,SimpleFun& fun) const;
    std::string parseCall(std::shared_ptr<CallTree> root,SimpleFun& fun) const;
    std::string parseExpression(std::shared_ptr<ExpressionTree> root,SimpleFun& fun,bool genresult=false) const;
    std::string parseLoop(std::shared_ptr<LoopTree> root,SimpleFun& fun) const;
    std::string parseIf(std::shared_ptr<ConditionTree> root,SimpleFun& fun) const;
    std::string parseCreateLabel(SimpleFun& fun)const;
};

#endif