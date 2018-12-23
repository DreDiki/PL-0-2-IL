#ifndef __H_SIMPLEPIPEIL__
#define __H_SIMPLEPIPEIL__

#include "Pipe.h"
#include "Simple.h"
#include "IL.h"
#include <sstream>

class ILFun {
public:
    std::vector<ILInst> instrs;
    std::vector<std::string> locs;
    std::vector<std::string> loctypes;
    std::vector<std::string> stack; 
};

class SimplePipeIL : public Pipe{
public:
    std::shared_ptr<Liquid> flow(std::shared_ptr<Liquid> in) const override;
    std::string locate(std::shared_ptr<Liquid> in,Location location) const override;
protected:
    void parseCommonHead(std::stringstream& s) const;
    void parseCommonTail(std::stringstream& s) const;

    void parseFunction(std::stringstream& s,SimpleFun& fun) const;

    void optimizeFunction(SimpleFun& fun,ILFun& container)const;

    void parseStatement(SimpleFun& fun,int i,ILFun& container)const;
    void loadVariable(std::string var,SimpleFun& fun,ILFun& container)const;
    //0 for int32 1 for &int32
    void saveVariable(std::string var,SimpleFun& fun,ILFun& container,int type=0)const;



    //std::string parseStatement(SimpleFun& fun,int& i)const;

};

#endif