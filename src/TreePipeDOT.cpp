#include "TreePipeDOT.h"

#include <vector>
#include <map>
#include "Liquids.h"
#include "Tree.h"
#include <sstream>
#include <memory>
std::shared_ptr<Liquid> TreePipeDOT::flow(std::shared_ptr<Liquid> in) const{
    auto tin = in->toTreeLiquid();
    auto root = tin->value;
    std::map<std::shared_ptr<Tree>,std::string> labels;
    std::vector<std::string> edges;
    //std::map<std::shared_ptr<Tree>,std::string> edges;
    std::stringstream s;
    s<<"digraph Program { \n \
    node [shape=box, fontname=\"Arial\" ];\n";
    //root->treeType
    auto type2name = [](std::shared_ptr<Tree> t)->std::string{
        switch(t->type()){
            case TreeType::Block:
                return "style=filled,fillcolor=lightblue1 label=\"Block\"";
            case TreeType::Constant:
                return std::string("shape=ellipse style=filled,fillcolor=lavenderblush1 label=\"Const ")+t->toConstantTree()->identifier+"\"";
            case TreeType::LongValue:
                return std::string("label=\"")+std::to_string(t->toLongValueTree()->value)+"\"";
            case TreeType::Variable:
                return std::string("shape=ellipse style=filled,fillcolor=lavenderblush1 label=\"Var ")+t->toVariableTree()->identifier+"\"";
            case TreeType::Function:
                return std::string("style=filled,fillcolor=palegreen1 label=\"Function ")+t->toFunctionTree()->identifier+"\"";
            case TreeType::Condition:
                return "style=filled,fillcolor=pink shape=diamond label=\"Cond\"";
            case TreeType::Loop:
                return "style=filled,fillcolor=pink shape=diamond label=\"Loop\"";
            case TreeType::Expression:
                return std::string("shape=ellipse label=\"")+PLOperator::ttoString((t->toExpressionTree()->op))+"\"";
            case TreeType::Reference:
                return std::string("label=\"'")+t->toReferenceTree()->identifier+"'\"";
            case TreeType::Call:
                return std::string("style=filled,fillcolor=cornsilk label=\"Call ")+t->toCallTree()->identifier+"\"";
            case TreeType::Trivial:
            default:
                return "label=\"Node\"";
        }
    };
    labels[root] = "l"+std::to_string(labels.size());
    auto recursive = [&](std::shared_ptr<Tree> root,auto & ref)->void{
        if(root==nullptr)return;
        //labels[root] = "l"+labels.size();
        s<<labels[root]<<"[ "<<type2name(root)<<" ];\n";
        std::string se=labels[root]+" -> { ";
        if(root->type()==TreeType::Block){
            auto block = root->toBlockTree();
            for(auto& i:block->constants){

                if(labels.find(i)!=labels.end()){
                    se+=labels[i];
                    se+=' ';
                }else{
                    labels[i] = "l"+std::to_string(labels.size());
                    se+=labels[i];
                    se+=' ';
                }
                ref(i,ref);
            }
            for(auto& i:block->variables){
                if(labels.find(i)!=labels.end()){
                    se+=labels[i];
                    se+=' ';
                }else{
                    labels[i] = "l"+std::to_string(labels.size());
                    se+=labels[i];
                    se+=' ';
                }
                ref(i,ref);
            }            
        }else if(root->type()==TreeType::Condition){
            std::string se=labels[root]+" -> { ";
            auto cond = root->toConditionTree()->condition;
            if(labels.find(cond)!=labels.end()){
                se+=labels[cond];
                se+=' ';
            }else{
                labels[cond] = "l"+std::to_string(labels.size());
                se+=labels[cond];
                se+=' ';
            }
            ref(cond,ref);
            se+="} [shape=dot];\n";
            edges.push_back(se);
        }else if(root->type()==TreeType::Loop){
            std::string se=labels[root]+" -> { ";
            auto cond = root->toLoopTree()->condition;
            if(labels.find(cond)!=labels.end()){
                se+=labels[cond];
                se+=' ';
            }else{
                labels[cond] = "l"+std::to_string(labels.size());
                se+=labels[cond];
                se+=' ';
            }
            ref(cond,ref);
            se+="} [shape=dot];\n";
            edges.push_back(se);
        }else if(root->type()==TreeType::Variable||root->type()==TreeType::Constant){
            auto block = root->toVariableTree();
            auto i=block->initial;
            if(i!=nullptr){
                if(labels.find(i)!=labels.end()){
                    se+=labels[i];
                    se+=' ';
                }else{
                    labels[i] = "l"+std::to_string(labels.size());
                    se+=labels[i];
                    se+=' ';
                }
                ref(i,ref);
            }
        }

        for(auto& i:root->childs){
            if(labels.find(i)!=labels.end()){
                se+=labels[i];
                se+=' ';
            }else{
                labels[i] = "l"+std::to_string(labels.size());
                se+=labels[i];
                se+=' ';
            }
            ref(i,ref);
        }
        se+="};\n";
        edges.push_back(se);
    };
    recursive(root,recursive);

    //for()
    //auto parseroot = [&]()->{

    //}
    for(auto& se:edges){
        s<<se;
    }
    s<<"}";
    auto result = std::make_shared<StringLiquid>();
    result->value = s.str();
    return result;
}
std::string TreePipeDOT::locate(std::shared_ptr<Liquid> in,Location location) const{
    return {};
}