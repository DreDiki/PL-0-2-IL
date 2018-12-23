#include "TreePipeSimple.h"
#include "Liquid.h"
#include "Liquids.h"
#include <unordered_map>
#include <string>
using namespace std::string_literals;

std::shared_ptr<Liquid> TreePipeSimple::flow(std::shared_ptr<Liquid> in) const{
    auto tin = in->toTreeLiquid();
    auto root = tin->value;
    //std::unordered_map<std::string,long> staticvalues;
    auto tout = std::make_shared<SimpleLiquid>();
    std::vector<SimpleFun> value;

    SimpleFun fun;
    fun.identifier = "\0ROOT"s;
    fun.lookuptable = std::make_shared<SimpleLookup>();
    value.push_back(fun);
    //auto lookuptable = std::make_shared<SimpleLookup>();
    parseBlock(root->childs[0],value);
    tout->value = value;
    debug("done simple");
    return tout;
}

std::string TreePipeSimple::locate(std::shared_ptr<Liquid> in,Location location) const{
    return {};
}

std::string TreePipeSimple::parseBlock(std::shared_ptr<Tree> root,std::vector<SimpleFun>& funs) const{
    auto i=funs.size()-1;
    // if(root->type()!=TreeType::Block);
    auto block = root->toBlockTree();
    for(auto variable:block->variables){
        std::string name=variable->toVariableTree()->identifier;
        funs[i].variables[name]={name};
    }
    for(auto statics:block->constants){
        std::string name=statics->toVariableTree()->identifier;
        long value = 0;
        if(statics->initial&&statics->initial->type()==TreeType::LongValue){
            value = statics->initial->toLongValueTree()->value;
        }
        funs[i].statics[name] = {name,value};
        funs[i].lookuptable->globalstatic[name]= {name,value};
    }
    
    for(auto child:root->childs){
        if(child==nullptr)continue;
        if(child->type()==TreeType::Function){
            SimpleFun fun;
            //auto childfun = child->toFunctionTree();
            fun.lookuptable = funs[i].lookuptable;
            fun.identifier = child->toFunctionTree()->identifier;
            funs.push_back(fun);
            
            if(child->childs.size()>0)
                parseBlock(child->childs[0],funs);
        }else{
            parseStatement(child,funs[i]);
        }
    }
    return {};
}
std::string TreePipeSimple::parseToString(std::shared_ptr<Tree> root,SimpleFun& fun) const{
    if(root->type()==TreeType::Reference){
        return root->toReferenceTree()->identifier;
    }else if(root->type()==TreeType::LongValue){
        auto l = root->toLongValueTree();
        std::string s = "\0S"s+std::to_string(fun.statics.size()+1);
        SimpleVariable v{s,l->value};
        fun.statics[s]=v;
        return s;
    }else if(root->type()==TreeType::Expression){
        return parseExpression(root->toExpressionTree(),fun,true);
    }
    error("error in parseToString,%d",root->type());
    return {};
}

std::string TreePipeSimple::parseStatement(std::shared_ptr<Tree> root,SimpleFun& fun) const{
    switch(root->type()){
        case TreeType::Condition:
            return parseIf(root->toConditionTree(),fun);
        case TreeType::Loop:
            return parseLoop(root->toLoopTree(),fun);
        case TreeType::Expression:
            return parseExpression(root->toExpressionTree(),fun);
        case TreeType::Call:
            return parseCall(root->toCallTree(),fun);
        default:
            for(auto child:root->childs)
            {
                if(child==nullptr)continue;
                debug("parse %s,%d",fun.identifier.c_str(),child->type());
                parseStatement(child,fun);
            }            
    }
    return {};
}
std::string TreePipeSimple::parseCall(std::shared_ptr<CallTree> root,SimpleFun& fun) const{
    if(root->identifier=="write"){
        for(auto child:root->childs)
        {
            if(child==nullptr)continue;
            auto res =parseToString(child,fun);
            fun.statments.push_back({SimpleOp::Call,root->identifier,res});
            //debug("parse %s,%d",fun.identifier.c_str(),child->type());
            //parseStatement(child,fun);
        }   
    }else if(root->identifier=="read"){
        for(auto child:root->childs)
        {
            if(child==nullptr)continue;
            auto res =parseToString(child,fun);
            std::string s = "\0T"s+std::to_string(fun.temps.size()+1);
            SimpleVariable v{s};
            fun.temps[s]=v;

            fun.statments.push_back({SimpleOp::Addr,res,"",s});
            fun.statments.push_back({SimpleOp::Call,root->identifier,s});
            //debug("parse %s,%d",fun.identifier.c_str(),child->type());
            //parseStatement(child,fun);
        }   
    }else{
        fun.statments.push_back({SimpleOp::Call,root->identifier});
    }
    //fun.statments.push_back()
    return {};
}

std::string TreePipeSimple::parseLoop(std::shared_ptr<LoopTree> root,SimpleFun& fun) const{
    if(root->condition==nullptr)return {};
    //judgelabel
    std::string s = "\0L"s+std::to_string(fun.labels.size()+1);
    SimpleVariable v{s,(long)fun.statments.size()};
    fun.labels.push_back(v);
    fun.statments.push_back({SimpleOp::Jmp,s});
    //main label
    std::string beginlabel = parseCreateLabel(fun);
    //childs
    if(root->childs.size()!=0){
        parseStatement(root->childs[0],fun);
    }
    //judgelabel    
    fun.statments.push_back({SimpleOp::Label,s});
    //if t1,jump main
    auto cond = parseExpression(root->condition->toExpressionTree(),fun,true);
    fun.statments.push_back({SimpleOp::If,cond,"",beginlabel});
    //fun.statments.push_back({SimpleOp::Label,s});
    return {};
}
std::string TreePipeSimple::parseIf(std::shared_ptr<ConditionTree> root,SimpleFun& fun) const{
    if(root->condition==nullptr)return {};
    
    std::string endl = "\0L"s+std::to_string(fun.labels.size()+1);
    fun.labels.push_back({endl,(long)fun.statments.size()});
    
    std::string beginl = "\0L"s+std::to_string(fun.labels.size()+1);
    fun.labels.push_back({beginl,(long)fun.statments.size()});

    auto cond = parseExpression(root->condition->toExpressionTree(),fun,true);
    fun.statments.push_back({SimpleOp::If,cond,"",beginl});
    fun.statments.push_back({SimpleOp::Jmp,endl});
    fun.statments.push_back({SimpleOp::Label,beginl});

    //childs
    if(root->childs.size()!=0){
        parseStatement(root->childs[0],fun);
    }

    fun.statments.push_back({SimpleOp::Label,endl});

    //fun.statments.push_back();
    return {};
}
std::string TreePipeSimple::parseExpression(std::shared_ptr<ExpressionTree> root,SimpleFun& fun,bool genresult) const{
    //debug("parse in expression");
    if(root->otype==OperationType::Unary){
        if(root->childs.size()!=1)return{};
        auto first = root->childs[0];
        std::string s;
        if(genresult){
            s = "\0T"s+std::to_string(fun.temps.size()+1);
            SimpleVariable v{s};
            fun.temps[s]=(v);
            fun.statments.push_back({OpToSOP(root->op),parseToString(first,fun),"",s});
        }else{
            fun.statments.push_back({OpToSOP(root->op),parseToString(first,fun)});
        }
        return s;
    }else if(root->otype==OperationType::Binary){
        if(root->childs.size()!=2)return{};
        auto first = root->childs[0];
        auto second = root->childs[1];
        std::string s;
        if(genresult){
            s = "\0T"s+std::to_string(fun.temps.size()+1);
            SimpleVariable v{s};
            fun.temps[s]=(v);
            fun.statments.push_back({OpToSOP(root->op),parseToString(first,fun),parseToString(second,fun),s});
        }else{
            fun.statments.push_back({OpToSOP(root->op),parseToString(first,fun),parseToString(second,fun),});
        }
        return s;
    }
    return {};
}

std::string TreePipeSimple::parseCreateLabel(SimpleFun& fun) const{
    std::string s = "\0L"s+std::to_string(fun.labels.size()+1);
    SimpleVariable v{s,(long)fun.statments.size()};
    fun.labels.push_back(v);
    fun.statments.push_back({SimpleOp::Label,s});
    return s;
}
