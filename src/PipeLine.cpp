#include "PipeLine.h"

PipeLine::PipeLine(){
    wasteHandler = std::make_shared<WasteHandler>();
    keywordSpecific = std::make_shared<PLKeyword>();
}

std::shared_ptr<Liquid> PipeLine::flow(std::shared_ptr<Liquid> in) const{
    wasteHandler->reset();
    auto lastin = in;
    int i=0;
    for(auto pipe:pipes)
    {
        wasteHandler->move(pipe,lastin);
        lastin = pipe->flow(lastin);
        if(wasteHandler->hasError())break;//todo
        debug("done pipe stage %d",i++);
    }
    return lastin;
}

PipeLine& PipeLine::operator << (std::shared_ptr<Pipe> pipe){
    pipes.push_back(pipe);
    pipe->setHandler(wasteHandler);
    pipe->setKeyword(keywordSpecific);
    return *this;
}