#include "StringPipeTokens.h"
#include "Liquids.h"
#include "util.h"
constexpr char numbers[] = "0123456789";
constexpr int CharToNumber(const char& number){
    //Todo: using std::isdigit
    //Todo: impove performance
    for(int i=0;i<10;++i){
        if(numbers[i]==number)return i;
    }
    return -1;
}

std::string StringPipeTokens::locate(std::shared_ptr<Liquid> in,Location location) const{
    auto fin = in->toTokensLiquid();
    return std::to_string(fin->value[location.begin]->line);
    //auto lines = split(fin->value,'\n');

    //return lines[fin->value[location.begin]->line];
}
std::shared_ptr<Liquid> StringPipeTokens::flow(std::shared_ptr<Liquid> in) const{
    static PLOperator _Operator;
    auto fin = in->toStringLiquid();
    auto& content = fin->value;
    auto contentsize = content.size();
    auto result = std::make_shared<TokensLiquid>();
    size_t wordbegin = -1;
    size_t wordsize = 0;
    long curline = 1;
    for(size_t i=0;i<contentsize;++i){
        if(content[i]==' '||content[i]=='\t'||content[i]=='\n'){
            if(content[i]=='\n')curline++;
            if(wordsize!=0){
                auto token = parseWord(content.substr(wordbegin,wordsize));
                token->line = curline;
                result->value.push_back(token);
                wordsize = 0;
                wordbegin = -1;
            }
            continue;
        }else if(content[i]=='/'){
            if((i<contentsize-1)&&content[i+1]=='/'){
                while(content[i]!='\n')++i;
                curline++;
                continue;
            }
        }
        //Operand
        std::shared_ptr<OperatorToken> op;
        OperatorType opout;
        auto opstack = content.substr(i,MaxOperatorLength);
        auto oplen = _Operator.lookup(opstack,opout);
        if(oplen>0){
            if(wordsize!=0){
                auto token = parseWord(content.substr(wordbegin,wordsize));
                token->line = curline;
                result->value.push_back(token);
                wordsize = 0;
                wordbegin = -1;
            }
            op = std::make_shared<OperatorToken>();
            op->value = opout;
            i+= oplen-1;
            op->line = curline;
            result->value.push_back(std::dynamic_pointer_cast<Token>(op));
            continue;
        }

        //Number
        auto num = CharToNumber(content[i]);
        if(wordsize==0&&num!=-1){
            auto token = parseNumber(i,content);
            token->line = curline;
            result->value.push_back(token);
            continue;
        }
        if(wordsize==0){
            wordbegin = i;
        }
        ++wordsize;
    }
    if(wordsize!=0&&wordbegin!=-1){
        auto token = parseWord(content.substr(wordbegin,wordsize));
        token->line = curline;
        result->value.push_back(token);
        wordsize = 0;
    }

    return result;
}

std::shared_ptr<Token> StringPipeTokens::parseWord(const std::string & word) const{
    //todo replace plkeyword instance
    //static PLKeyword _Keyword;
    auto key = keywordSpecific->lookup(word);
    //debug("%d,%s",key,word.c_str());
    if(key!=-1){
        auto token = std::make_shared<KeywordToken>();
        token->value = key;
        return token;
    }
    auto token = std::make_shared<IdentToken>();
    token->value = word;
    return std::dynamic_pointer_cast<Token>(token);
}
std::shared_ptr<Token> StringPipeTokens::parseNumber(size_t &begin,const std::string & line) const{
    size_t left;
    auto value = std::stol(line.substr(begin),&left);
    begin +=left-1;
    auto token = std::make_shared<ValueToken>();
    token->value = value;
    return token;
}

