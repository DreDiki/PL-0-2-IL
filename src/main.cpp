#include <iostream>

#include "PipeLine.h"
#include "FilePipeString.h"
#include "StringPipeTokens.h"
#include "TokensPipeTree.h"
#include "TreePipeDOT.h"
#include "TreePipeSimple.h"
#include "SimplePipeIL.h"

#include "Liquids.h"
#include <memory>
#include "util.h"
#include <fstream>
void printtk(std::shared_ptr<TokensLiquid> liquid){
    static PLKeyword _plk;
    static PLOperator _plo;

    for(auto& token: liquid->value){
        switch(token->type()){
            case TokenType::Keyword:
            {
                auto keyword = token->toKeywordToken();
                ;
                std::cout<<"kw\t"<<_plk.toString(keyword->value)<<'\t';
            }
            break;
            case TokenType::Const:
            {
                auto con = token->toValueToken();
                std::cout<<"const\t"<<con->value<<'\t';
            }
            break;
            case TokenType::Ident:
            {
                auto name = token->toIdentToken();
                std::cout<<"name\t"<<name->value<<'\t';
            }
            break;
            case TokenType::Operator:
            {
                auto op = token->toOperatorToken();
                std::cout<<"op\t"<<_plo.toString(op->value)<<'\t';
            }
            break;
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

auto main(const int argc,const char* argv[])->int{
    std::string filename;
    std::string outpath="a.out";
    int mode=0;
    if(argc>1){
        int i=1;
        for(;i<argc;++i){
            if(argv[i][0]=='-'){
                if(argv[i][1]=='o'){
                    outpath=std::string(argv[++i]);
                }else if(argv[i][1]=='I'){
                    mode=1;
                }else if(argv[i][1]=='G'){
                    mode=2;
                }else if(argv[i][1]=='T'){
                    mode=4;
                }else if(argv[i][1]=='h'){
                    std::cout<<"Usage: dcc [options] file"<<std::endl;
                    std::cout<<"Options:"<<std::endl;
                    std::cout<<"  -I       \t\tOutput simple code"<<std::endl;
                    std::cout<<"  -G       \t\tOutput DOT code"<<std::endl;
                    std::cout<<"  -T       \t\tPrint Tokens"<<std::endl;
                    std::cout<<"  -o <file>\t\tPlace the output into <file>"<<std::endl;
                    return 0;
                }
            }else{
                filename = std::string(argv[i]);
            }
        }
        
    }else{
        error("Filename equired.");
        return 1;
    }
    PipeLine mainPipe;
    auto fps = std::make_shared<FilePipeString>();
    auto sptks = std::make_shared<StringPipeTokens>();
    auto tksptre = std::make_shared<TokensPipeTree>();
    auto trepdot = std::make_shared<TreePipeDOT>();
    auto trepsim = std::make_shared<TreePipeSimple>();
    auto simpil = std::make_shared<SimplePipeIL>();
    mainPipe<<fps<<sptks;
    if(mode!=4)
        mainPipe<<tksptre;
    switch(mode){
        case 2:
            mainPipe<<trepdot;
            break;
        case 1:
            mainPipe<<trepsim;
            break;
        case 0:
            mainPipe<<trepsim<<simpil;
            break;
    }
    auto file = std::make_shared<StringLiquid>();
    file->value = filename;
    auto result = mainPipe.flow(file);
    if(result){
        std::cout<<"Done."<<std::endl;
    }
    //std::cout<<result->toString()<<std::endl;
    
    std::ofstream outfile;
    outfile.open(outpath);
    if(!outfile.is_open()){
        return 2;
    }
    outfile<<result->toString();
    outfile.close();
    if(mode==4)
        printtk(std::dynamic_pointer_cast<TokensLiquid>(result));
    return 0;
}
