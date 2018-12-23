#include "Token.h"

#define GenerateType(Class,Type) \
    TokenType Class::type() const{ \
    return TokenType::Type; \
}

GenerateType(Token,Void)
GenerateType(OperatorToken,Operator)
GenerateType(IdentToken,Ident)
GenerateType(KeywordToken,Keyword)
GenerateType(ValueToken,Const)
#undef GenerateType
