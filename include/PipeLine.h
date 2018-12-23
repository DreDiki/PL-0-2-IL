#ifndef __H_PIPELINE__
#define __H_PIPELINE__

#include <vector>
#include "Pipe.h"
#include "WasteHandler.h"
#include "KeywordType.h"
class PipeLine{
public:
    PipeLine();
    std::shared_ptr<Liquid> flow(std::shared_ptr<Liquid> in) const;
    PipeLine& operator << (std::shared_ptr<Pipe> pipe);    
private:
    std::shared_ptr<Keyword> keywordSpecific;
    std::shared_ptr<WasteHandler> wasteHandler;
    std::vector<std::shared_ptr<Pipe>> pipes;
};
#endif