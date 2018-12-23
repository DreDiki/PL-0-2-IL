#include "TokensPipeTree.h"
#include "Token.h"
#include "Liquids.h"
#include "WasteHandler.h"
#define throwE(message,location) { \
    wasteHandler->handle(message,Location(location));\
    return nullptr; \
}

std::shared_ptr<Liquid> TokensPipeTree::flow(std::shared_ptr<Liquid> in) const{
    auto tin = in->toTokensLiquid();
    auto tokens = tin->value;
    size_t i=0;
    auto tree = parseProgram(tokens,i);
    auto treeLiquid = std::make_shared<TreeLiquid>();
    debug("done tree")
    treeLiquid->value = tree;
    return treeLiquid;
}

std::string TokensPipeTree::locate(std::shared_ptr<Liquid> in,Location location) const{
    return {};
}
/**
    program = block "." .
**/
std::shared_ptr<Tree> TokensPipeTree::parseProgram(std::vector<std::shared_ptr<Token>> &container,size_t& i) const{
    
    auto mainBlock = parseBlock(container,i);
    auto header = std::make_shared<FunctionTree>();
    header->identifier = "program_root";
    header->childs.push_back(mainBlock);
    return header;
    //if(container[i]->type()!=TokenType::Operator)throwE("Program must end with '.' .",i);//error 
    //auto dot = container[i]->toOperatorToken();
    //if(dot->value!=OperatorType::Dot)throwE("Program must end with '.' .",i);//error
    //++i;
}

/**
    block = [ "const" ident "=" number {"," ident "=" number} ";"]
        [ "var" ident {"," ident} ";"]
        { "procedure" ident ";" block ";" } statement .
 **/
std::shared_ptr<Tree> TokensPipeTree::parseBlock(std::vector<std::shared_ptr<Token>> &container,size_t& i)const {
    auto tree = std::make_shared<BlockTree>();
    if(container[i]->type()==TokenType::Keyword){
        auto keyword = container[i]->toKeywordToken();
        if(keyword->value==keywordSpecific->lookup("const")){
            i++;
            if(container[i]->type()!=TokenType::Ident)throwE("const need an identifier.",i);//error 
            auto ident = container[i]->toIdentToken();
            i++;
            if(container[i]->type()!=TokenType::Operator)throwE("const need '='.",i);//error 
            if(container[i]->toOperatorToken()->value!=OperatorType::Equal)throwE("const need '='.",i);
            i++;
            if(container[i]->type()!=TokenType::Const)throwE("const need initial value.",i);//error 
            auto value = container[i]->toValueToken();
            i++;
            //construct tree
            auto constant = std::make_shared<ConstantTree>();
            constant->identifier = ident->value;
            constant->initial = LongValueTree::fromToken(value);
            tree->constants.push_back(constant);
            while(true){
                if(container[i]->type()!=TokenType::Operator)throwE("error in const.",i);//error 
                auto op = container[i]->toOperatorToken();
                if(op->value==OperatorType::Semicolon){
                    i++;
                    break;
                }
                if(op->value!=OperatorType::Comma)throwE("error in const.",i);//error
                i++;

                if(container[i]->type()!=TokenType::Ident)throwE("error in const.",i);//error 
                auto ident = container[i]->toIdentToken();
                i++;
                if(container[i]->type()!=TokenType::Operator)throwE("error in const.",i);//error 
                if(container[i]->toOperatorToken()->value!=OperatorType::Equal)throwE("error in const.",i);//error
                i++;
                if(container[i]->type()!=TokenType::Const)throwE("error in const.",i);//error 
                auto value = container[i]->toValueToken();
                i++;
                //check duplicate declaration in same block
                for(auto &cstree:tree->constants){
                    if(cstree->identifier==ident->value)throwE("identifier defined twice",i);//error
                }
                //construct tree
                auto constant = std::make_shared<ConstantTree>();
                constant->identifier = ident->value;
                constant->initial = LongValueTree::fromToken(value);
                tree->constants.push_back(constant);
            }
        }
    }if(container[i]->type()==TokenType::Keyword){
        auto keyword = container[i]->toKeywordToken();
        if(keyword->value==keywordSpecific->lookup("var")){
            i++;
            if(container[i]->type()!=TokenType::Ident)throwE("var need an identifier.",i);//error 
            auto ident = container[i]->toIdentToken();
            i++;
            //construct tree
            auto var = std::make_shared<VariableTree>();
            var->identifier = ident->value;
            //constant->initial = LongValueTree::fromToken(value);
            tree->variables.push_back(var);
            while(true){
                if(container[i]->type()!=TokenType::Operator)throwE("error in var.",i);//error 
                auto op = container[i]->toOperatorToken();
                if(op->value==OperatorType::Semicolon){
                    i++;
                    break;
                }
                if(op->value!=OperatorType::Comma)throwE("error in var.",i);//error
                i++;

                if(container[i]->type()!=TokenType::Ident)throwE("var need an identifier.",i);//error 
                auto ident = container[i]->toIdentToken();
                i++;

                //check duplicate declaration in same block
                for(auto &cstree:tree->constants){
                    if(cstree->identifier==ident->value)throwE("identifier defined twice",i);//error
                }
                for(auto &cstree:tree->variables){
                    if(cstree->identifier==ident->value)throwE("identifier defined twice",i);//error
                }

                //construct tree
                auto var = std::make_shared<VariableTree>();
                var->identifier = ident->value;
                //constant->initial = LongValueTree::fromToken(value);
                tree->variables.push_back(var);
            }
        }
    }
    while(container[i]->type()==TokenType::Keyword){
        auto keyword = container[i]->toKeywordToken();
        if(keyword->value==keywordSpecific->lookup("procedure")){
            i++;
            if(container[i]->type()!=TokenType::Ident)throwE("procedure need an identifier.",i);//error 
            auto ident = container[i]->toIdentToken();
            i++;
            if(container[i]->type()!=TokenType::Operator)throwE("Error in procedure.",i);//error 
            if(container[i]->toOperatorToken()->value!=OperatorType::Semicolon)throwE("Error in procedure.",i);//error
            i++;
            auto content = parseBlock(container,i);
            if(container[i]->type()!=TokenType::Operator)throwE("Error in procedure.",i);//error 
            if(container[i]->toOperatorToken()->value!=OperatorType::Semicolon)throwE("Error in procedure.",i);//error
            i++;
            debug("done procedure")
            auto proc = std::make_shared<FunctionTree>();
            proc->identifier = ident->value;
            proc->childs.push_back(content);
            tree->childs.push_back(proc);
        }else{
            break;
        }
    }
    //parse statement
    auto state = parseStatement(container,i);
    if(state)
        tree->childs.push_back(state);

    return tree;
}

/**
    statement = [ ident ":=" expression | "call" ident 
              | "?" ident | "!" expression 
              | "read(" ident { "," ident } ")" | "write(" expression  { "," expression } ")" 
              | "begin" statement {";" statement } "end" 
              | "if" condition "then" statement 
              | "while" condition "do" statement ]. 
*/
std::shared_ptr<Tree> TokensPipeTree::parseStatement(std::vector<std::shared_ptr<Token>> &container,size_t& i) const{
    if(container[i]->type()==TokenType::Ident){
        auto ident = container[i]->toIdentToken();
        auto idtree = std::make_shared<ReferenceTree>();
        idtree->identifier = ident->value;
        i++;
        if(container[i]->type()!=TokenType::Operator)throwE("Error in assignment.",i);//error 
        if(container[i]->toOperatorToken()->value!=OperatorType::Assign)throwE("Error in assignment.",i);//error
        i++;
        auto expr = parseExpression(container,i);
        auto a = std::make_shared<ExpressionTree>();
        a->otype = OperationType::Binary;
        a->op = OperatorType::Assign;
        a->childs.push_back(idtree);
        a->childs.push_back(expr);
        return a;
    }else if(container[i]->type()==TokenType::Keyword){
        auto keyw = container[i]->toKeywordToken();
        if(keyw->value==keywordSpecific->lookup("call")){
            i++;
            if(container[i]->type()!=TokenType::Ident)throwE("Error in call:identity required.",i);
            auto id = container[i]->toIdentToken();
            auto call = std::make_shared<CallTree>();
            call->identifier = id->value;
            i++;
            return call;
        }else if(keyw->value==keywordSpecific->lookup("write")){
            i++;
            if(container[i]->type()!=TokenType::Operator||container[i]->toOperatorToken()->value!=OperatorType::LeftParenth)
                throwE("Error in write: '(' required",i);
            i++;
            auto calltree = std::make_shared<CallTree>();
            calltree->identifier = "write";

            auto arg1 = parseExpression(container,i);
            calltree->childs.push_back(arg1);

            while(true){
                if(container[i]->type()!=TokenType::Operator||container[i]->toOperatorToken()->value!=OperatorType::Comma){
                    break;
                }
                i++;
                auto arg = parseExpression(container,i);
                calltree->childs.push_back(arg);
            }
            if(container[i]->type()!=TokenType::Operator||container[i]->toOperatorToken()->value!=OperatorType::RightParenth)
                throwE("Error in write: ')' required",i);
            i++;
            return calltree;
        }else if(keyw->value==keywordSpecific->lookup("read")){
                        i++;
            if(container[i]->type()!=TokenType::Operator||container[i]->toOperatorToken()->value!=OperatorType::LeftParenth)
                throwE("Error in read: '(' required",i);
            i++;
            auto calltree = std::make_shared<CallTree>();
            calltree->identifier = "read";
            if(container[i]->type()!=TokenType::Ident)
                throwE("Error in read: at least one arg required",i);
            auto arg1 = std::make_shared<ReferenceTree>();
            arg1->identifier = container[i]->toIdentToken()->value;
            calltree->childs.push_back(arg1);
            i++;
            while(true){
                if(container[i]->type()!=TokenType::Operator||container[i]->toOperatorToken()->value!=OperatorType::Comma){
                    break;
                }
                i++;
                if(container[i]->type()!=TokenType::Ident)
                    throwE("Error in read: identy required",i);
                auto arg1 = std::make_shared<ReferenceTree>();
                arg1->identifier = container[i]->toIdentToken()->value;
                calltree->childs.push_back(arg1);
                i++;
            }
            if(container[i]->type()!=TokenType::Operator||container[i]->toOperatorToken()->value!=OperatorType::RightParenth)
                throwE("Error in read: ')' required",i);
            i++;
            return calltree;
        }else if(keyw->value==keywordSpecific->lookup("begin")){
            i++;
            //debug("12--%ld",i)
            auto tree = std::make_shared<Tree>();

            auto a = parseStatement(container,i);
            tree->childs.push_back(a);
            while(true){
                if(container[i]->type()==TokenType::Keyword&&
                container[i]->toKeywordToken()->value==keywordSpecific->lookup("end")){
                    i++;
                    break;
                }
                else if(container[i]->type()!=TokenType::Operator)throwE("Error in statement.",i);//error 
                if(container[i]->toOperatorToken()->value!=OperatorType::Semicolon)throwE("Error in statement.';' required",i);//error
                i++;
                a = parseStatement(container,i);
                if(a)
                    tree->childs.push_back(a);
            }
            return tree;
        }else if(keyw->value==keywordSpecific->lookup("if")){
            i++;
            auto condition = parseCondition(container,i);
            if(container[i]->type()!=TokenType::Keyword||
                container[i]->toKeywordToken()->value!=keywordSpecific->lookup("then")){
                    throwE("Error in if, then required.",i);
                }
            i++;
            auto statement = parseStatement(container,i);
            auto iftree = std::make_shared<ConditionTree>();
            iftree->condition = condition;
            iftree->childs.push_back(statement);
            return iftree;

        }else if(keyw->value==keywordSpecific->lookup("while")){
            i++;
            auto condition = parseCondition(container,i);

            if(container[i]->type()!=TokenType::Keyword||
                container[i]->toKeywordToken()->value!=keywordSpecific->lookup("do")){
                    throwE("Error in while, do required.",i);
            }
            i++;
            auto statement = parseStatement(container,i);
            auto iftree = std::make_shared<LoopTree>();
            iftree->condition = condition;
            iftree->childs.push_back(statement);
            return iftree;
        }
    }
    return nullptr;
}

/**
condition = "odd" expression |
            expression ("="|"#"|"<"|"<="|">"|">=") expression . 
 
*/
std::shared_ptr<Tree> TokensPipeTree::parseCondition(std::vector<std::shared_ptr<Token>> &container,size_t& i) const{
    if(container[i]->type()==TokenType::Keyword&&
        container[i]->toKeywordToken()->value==keywordSpecific->lookup("odd")){
        i++;
        auto prefix = std::make_shared<ExpressionTree>();
        prefix->otype = OperationType::Unary;
        prefix->op = OperatorType::SingleQuot;
        //todo -> odd wa na ni?
        prefix->childs.push_back(parseExpression(container,i));
        return prefix;
    }else{
        auto expr = std::make_shared<ExpressionTree>();
        expr->otype = OperationType::Binary;
        auto aa = parseExpression(container,i);
        if(container[i]->type()!=TokenType::Operator)throwE("Error in condition: operator required.",i);
        auto op = container[i]->toOperatorToken();
        if(op->value==OperatorType::Equal||
            op->value==OperatorType::NotEqual||
            op->value==OperatorType::Less||
            op->value==OperatorType::LessEqual||
            op->value==OperatorType::Greater||
            op->value==OperatorType::GreaterEqual
        ){
            expr->op = op->value;
            i++;
        }else{
            throwE("Error in condition: operator not supported",i)
        }
        auto bb = parseExpression(container,i);
        expr->childs.push_back(aa);
        expr->childs.push_back(bb);
        return expr;
    }

}   


/**
    expression = [ "+"|"-"] term { ("+"|"-") term}. 
*/
std::shared_ptr<Tree> TokensPipeTree::parseExpression(std::vector<std::shared_ptr<Token>> &container,size_t& i) const{
    
    std::shared_ptr<Tree> expressionnow = nullptr;
    auto prefix = std::make_shared<ExpressionTree>();
    if(container[i]->type()==TokenType::Operator){
        auto op = container[i]->toOperatorToken();
        if(op->value==OperatorType::Plus){
            //do nothing
            i++;
        }else if(op->value==OperatorType::Minus){
            i++;
            prefix->otype = OperationType::Unary;
            prefix->op = OperatorType::Minus;
        }else{
            //do nothing
        }
    }
    auto front = parseTerm(container,i);

    if(prefix->otype!=OperationType::None){
        prefix->childs.push_back(front);
        expressionnow = prefix;
    }else{
        expressionnow = front;
    }
    while(true){
        if(container[i]->type()==TokenType::Operator){
            auto op = container[i]->toOperatorToken();
            auto expr = std::make_shared<ExpressionTree>();
            expr->otype = OperationType::Binary;
            if(op->value==OperatorType::Plus||op->value==OperatorType::Minus){
                expr->op = op->value;
            }else{
                break;
            }
            i++;
            auto next = parseTerm(container,i);
            expr->childs.push_back(expressionnow);
            expr->childs.push_back(next);
            expressionnow = expr;
        }else{
            break;
        }
    }
    
    return expressionnow;
}

/**
    term = factor {("*"|"/") factor}.
*/
std::shared_ptr<Tree> TokensPipeTree::parseTerm(std::vector<std::shared_ptr<Token>> &container,size_t& i) const{
    std::shared_ptr<Tree> expressionnow = nullptr;
    auto front= parseFactor(container,i);
    while(true){
        if(container[i]->type()==TokenType::Operator){
            auto op = container[i]->toOperatorToken();
            auto expr = std::make_shared<ExpressionTree>();
            expr->otype = OperationType::Binary;
            if(op->value==OperatorType::Multiply||op->value==OperatorType::Division){
                expr->op = op->value;
            }else{
                break;
            }
            i++;
            auto next= parseFactor(container,i);
            if(expressionnow==nullptr){
                expr->childs.push_back(front);
            }else{
                expr->childs.push_back(expressionnow);
            }
            expr->childs.push_back(next);
            expressionnow = expr;
        }else{
            break;
        }
    }
    if(expressionnow==nullptr){
        return front;
    }
    return expressionnow;
}
/**
    factor = ident | number | "(" expression ")".
*/
std::shared_ptr<Tree> TokensPipeTree::parseFactor(std::vector<std::shared_ptr<Token>> &container,size_t& i) const{
    
    if(container[i]->type()==TokenType::Ident){
        auto ident = container[i]->toIdentToken();
        i++;
        auto idtree = std::make_shared<ReferenceTree>();
        idtree->identifier = ident->value;
        return idtree;
    }
    else if(container[i]->type()==TokenType::Const){
        auto num = container[i]->toValueToken();
        i++;
        auto costree = LongValueTree::fromToken(num);
        return costree;
    }
    else if(container[i]->type()==TokenType::Operator){
        auto op = container[i]->toOperatorToken();
        if(op->value==OperatorType::LeftParenth){
            i++;
            auto expr = parseExpression(container,i);
            if(container[i]->type()==TokenType::Operator&&
                container[i]->toOperatorToken()->value==OperatorType::RightParenth)
                throwE("Parenth not pair",i);
            i++;
            return expr;
        }
    }
}
