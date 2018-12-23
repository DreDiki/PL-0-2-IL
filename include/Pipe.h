#ifndef __H_PIPE__
#define __H_PIPE__

#include <string>
#include <memory>
#include "Liquid.h"
//#include "WasteHandler.h"
#include "KeywordType.h"

class WasteHandler;
class Pipe{
public:

    virtual std::shared_ptr<Liquid> flow(std::shared_ptr<Liquid> in) const = 0;
    //typeof()
    //virtual

    std::shared_ptr<Liquid> operator >> (std::shared_ptr<Liquid> in);

    virtual std::string locate(std::shared_ptr<Liquid> in,Location location) const = 0;

    void setHandler(std::shared_ptr<WasteHandler> handler);

    void setKeyword(std::shared_ptr<Keyword> keyword);

protected:
    std::shared_ptr<Keyword> keywordSpecific;
    std::shared_ptr<WasteHandler> wasteHandler;
};
#endif