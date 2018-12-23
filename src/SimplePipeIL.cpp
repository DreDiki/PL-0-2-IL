#include "SimplePipeIL.h"
#include "Liquids.h"
#include <sstream>
#include <string>
#include <algorithm>
using namespace std::string_literals;

#define CLASSNAME "PLProgram.Program"
std::shared_ptr<Liquid> SimplePipeIL::flow(std::shared_ptr<Liquid> in) const{
    auto tin=in->toSimpleLiquid();
    auto v = tin->value;
    std::stringstream ss;
    parseCommonHead(ss);
    for(auto &fun:v){
        parseFunction(ss,fun);
    }
    parseCommonTail(ss);
    auto tout = std::make_shared<StringLiquid>();
    tout->value = ss.str();
    return tout;
}
std::string SimplePipeIL::locate(std::shared_ptr<Liquid> in,Location location) const{
    return {};
}

void SimplePipeIL::parseCommonHead(std::stringstream& ss) const{
    ss<<".assembly extern mscorlib {}\n"
        ".assembly PLProgram {}\n"
        ".module PLProgram.exe\n"
        ".class PLProgram.Program\n"
        "extends [mscorlib]System.Object\n"
        "{\n"s;

}

void SimplePipeIL::parseCommonTail(std::stringstream& ss) const{
    ss<<"}\n"s;
}

void SimplePipeIL::parseFunction(std::stringstream& ss,SimpleFun& fun) const{
    for(auto &v:fun.variables){
        ss<<".field public static int32 "+v.second.identifier<<" \n";
    }
    for(auto &v:fun.statics){
        if(v.first.size()<1||v.first[0]=='\0')continue;
        ss<<".field public static literal int32 "+v.second.identifier<<" = int32("+std::to_string(v.second.value)+") \n";
    }

    if(fun.identifier=="\0ROOT"s){
        ss<<".method static void Main(string[] args) cil managed \n";
        ss<<"{\n";
        ss<<"    .entrypoint\n";
    }else{
        ss<<".method static void "<<fun.identifier<<"() \n";
        ss<<"{\n";
    }
    auto size = fun.statments.size();
    ILFun ilfun;
    for(size_t i=0;i<size;++i){
        parseStatement(fun,i,ilfun);
    }
    optimizeFunction(fun,ilfun);
    ss<<"    .locals init (\n";
    for(int i=0;i<ilfun.locs.size();++i){
        ss<<"    ["<<std::to_string(i)<<"] "<<ilfun.loctypes[i];
        if(i!=ilfun.locs.size()-1){
            ss<<',';
        }
        ss<<'\n';
    }
    ss<<"    )\n";
    for(int i=0;i<ilfun.instrs.size();++i){
        auto il = ilfun.instrs[i];
        if(il.op=="label"){
            ss<<il.target<<"\n";
        }else{
            ss<<"    "<<il.op<<" "<<il.target<<"\n";
        }
    }
    ss<<"    ret\n";
    ss<<"}\n";
    
}

void SimplePipeIL::optimizeFunction(SimpleFun& fun,ILFun& container)const{
    for(int i=0;i<container.instrs.size();++i){
        auto instr = container.instrs[i];
        if(i>1&&instr.op=="ldloc"&&container.instrs[i-1].op=="stloc"&&instr.target==container.instrs[i-1].target){
            bool remove=true;
            for(int j=i+1;j<container.instrs.size();++j){
                if(container.instrs[j].op=="stloc"&&container.instrs[j].target==instr.target){
                    break;
                }
                if(container.instrs[j].op=="ldloc"&&container.instrs[j].target==instr.target){
                    remove = false;
                    break;
                }
            }
            if(remove){
                container.instrs.erase(container.instrs.begin()+i-1,container.instrs.begin()+i+1);
                i-=2;
            }
        }
    }
    //todo remove unused loc from stack 
}
void SimplePipeIL::saveVariable(std::string var,SimpleFun& fun,ILFun& container,int type)const{
    if(var.size()==0)return;
    if(var[0]=='\0'){
        if(var[1]=='T'){
            auto find = std::find(container.locs.begin(),container.locs.end(),var);
            if(find!=container.locs.end()){
                auto pos = find- container.locs.begin(); //todo longlong-> int16 check
                container.instrs.push_back({"stloc",std::to_string(pos)});//todo i4 or i8?
            }else{
                auto pos = container.locs.size();
                container.locs.push_back(var);
                container.loctypes.push_back(type==0?"int32":"int32&");
                container.instrs.push_back({"stloc",std::to_string(pos)});//todo i4 or i8?

            }
            //container.stack.pop_back();//todo
        }
    }else{
        container.instrs.push_back({"stsfld","int32 "s+CLASSNAME+"::"+var});//todo i4 or i8?
        //container.stack.pop_back();//todo
    }
}
void SimplePipeIL::loadVariable(std::string var,SimpleFun& fun,ILFun& container)const{
    if(var.size()>0&&var[0]=='\0'){
        if(var[1]=='S'){ //constant
            auto i = fun.statics.find(var);
            if(i!=fun.statics.end()){
                auto v = std::to_string(i->second.value);
                container.instrs.push_back({"ldc.i4",v});//todo i4 or i8?
                //container.stack.push_back(var);
            }
        }else if(var[1]=='T'){
            auto find = std::find(container.locs.begin(),container.locs.end(),var);
            if(find!=container.locs.end()){
                auto pos = find- container.locs.begin(); //todo longlong-> int16 check
                container.instrs.push_back({"ldloc",std::to_string(pos)});//todo i4 or i8?
            }else{
                //error!
                error("error ldloc");
            }
        }
    }else{
        //debug("var%s",var.c_str());
        auto i = fun.statics.find(var);
        if(i!=fun.statics.end()){
            auto v = std::to_string(i->second.value);
            container.instrs.push_back({"ldc.i4",v});//todo i4 or i8?
            //container.stack.push_back(var);
            return;
        }else{
            //debug("global table size %d",fun.lookuptable->globalstatic.size());
            auto i = fun.lookuptable->globalstatic.find(var);
            if(i!=fun.lookuptable->globalstatic.end()){
                auto v = std::to_string(i->second.value);
                container.instrs.push_back({"ldc.i4",v});//todo i4 or i8?
                //container.stack.push_back(var);
                return;
            }
        }
        container.instrs.push_back({"ldsfld","int32 "s+CLASSNAME+"::"+var});//todo i4 or i8?

        //container.stack.push_back(var);//todo
    }
}
void SimplePipeIL::parseStatement(SimpleFun& fun,int i,ILFun& container)const{
    auto simple = fun.statments[i];
    //Nop,Label,Jmp,If,Mov,Add,Sub,Mul,Div,Gt,Lt,Eq,Ne,Ge,Le,Call,
    switch(simple.op){
        case SimpleOp::Nop:
            container.instrs.push_back({"nop"});
            break;
        case SimpleOp::Label:
            container.instrs.push_back({"label",simple.arg1.substr(1)+":"});
            break;
        case SimpleOp::Jmp:
            container.instrs.push_back({"br",simple.arg1.substr(1)});
            break;
        case SimpleOp::If:
            loadVariable(simple.arg1,fun,container);
            container.instrs.push_back({"brtrue",simple.res.substr(1)});
            break;
        case SimpleOp::Mov:
            loadVariable(simple.arg2,fun,container);
            saveVariable(simple.arg1,fun,container);
            break;
        case SimpleOp::Add:
            loadVariable(simple.arg1,fun,container);
            loadVariable(simple.arg2,fun,container);
            container.instrs.push_back({"add"});
            saveVariable(simple.res,fun,container);
            break;
        case SimpleOp::Sub:
            loadVariable(simple.arg1,fun,container);
            loadVariable(simple.arg2,fun,container);
            container.instrs.push_back({"sub"});
            saveVariable(simple.res,fun,container);
            break ;
        case SimpleOp::Mul:
            loadVariable(simple.arg1,fun,container);
            loadVariable(simple.arg2,fun,container);
            container.instrs.push_back({"mul"});
            saveVariable(simple.res,fun,container);
            break ;
        case SimpleOp::Div:
            loadVariable(simple.arg1,fun,container);
            loadVariable(simple.arg2,fun,container);
            container.instrs.push_back({"div"});
            saveVariable(simple.res,fun,container);
            break ;
        case SimpleOp::Gt:
            loadVariable(simple.arg1,fun,container);
            loadVariable(simple.arg2,fun,container);
            container.instrs.push_back({"cgt"});
            saveVariable(simple.res,fun,container);
            break ;
        case SimpleOp::Lt:
            loadVariable(simple.arg1,fun,container);
            loadVariable(simple.arg2,fun,container);
            container.instrs.push_back({"clt"});
            saveVariable(simple.res,fun,container);
            break ;
        case SimpleOp::Eq:
            loadVariable(simple.arg1,fun,container);
            loadVariable(simple.arg2,fun,container);
            container.instrs.push_back({"ceq"});
            saveVariable(simple.res,fun,container);
            break ;
        case SimpleOp::Ne:
            loadVariable(simple.arg1,fun,container);
            loadVariable(simple.arg2,fun,container);
            container.instrs.push_back({"ceq"});
            container.instrs.push_back({"ldc.i4.1"});
            container.instrs.push_back({"xor"});
            saveVariable(simple.res,fun,container);
            break ;
        case SimpleOp::Ge:
            loadVariable(simple.arg1,fun,container);
            loadVariable(simple.arg2,fun,container);
            container.instrs.push_back({"clt"});
            container.instrs.push_back({"ldc.i4.1"});
            container.instrs.push_back({"xor"});
            saveVariable(simple.res,fun,container);
            break ;
        case SimpleOp::Le:
            loadVariable(simple.arg1,fun,container);
            loadVariable(simple.arg2,fun,container);
            container.instrs.push_back({"cgt"});
            container.instrs.push_back({"ldc.i4.1"});
            container.instrs.push_back({"xor"});
            saveVariable(simple.res,fun,container);
            break ;
        case SimpleOp::Call:
            if(simple.arg1=="write"){
                loadVariable(simple.arg2,fun,container);
                container.instrs.push_back({"call","void [mscorlib]System.Console::Write(int32)"});
                container.instrs.push_back({"call","void [mscorlib]System.Console::WriteLine()"});
            }else if(simple.arg1=="read"){
                //saveVariable(simple.arg2)
                container.instrs.push_back({"call","string [mscorlib]System.Console::ReadLine()"});
                loadVariable(simple.arg2,fun,container);
                container.instrs.push_back({"call","bool [mscorlib]System.Int32::TryParse(string, int32&)"});
                container.instrs.push_back({"pop"});
            }else{
                container.instrs.push_back({"call","void "s+ CLASSNAME+"::"+simple.arg1+"()"});
            }
            //if()
            break ;
        case SimpleOp::Odd:
            loadVariable(simple.arg1,fun,container);
            container.instrs.push_back({"ldc.i4.1"});
            container.instrs.push_back({"and"});
            saveVariable(simple.res,fun,container);
            break;
        case SimpleOp::Addr:
            if(simple.arg1.size()>1&&simple.arg1[0]=='\0'&&simple.arg1[0]=='T'){
                auto find = std::find(container.locs.begin(),container.locs.end(),simple.arg1);
                if(find!=container.locs.end()){
                    auto pos = find- container.locs.begin(); //todo longlong-> int16 check
                    container.instrs.push_back({"ldloca",std::to_string(pos)});//todo i4 or i8?
                }else{
                    auto pos = container.locs.size();
                    container.locs.push_back(simple.arg1);
                    container.loctypes.push_back("int32");
                    container.instrs.push_back({"ldloca",std::to_string(pos)});//todo i4 or i8?
                }
            }else{
                container.instrs.push_back({"ldsflda","int32 "s+CLASSNAME+"::"+simple.arg1});//todo i4 or i8?
            }
            saveVariable(simple.res,fun,container,1);
            break;
    }
    //return {};
} 