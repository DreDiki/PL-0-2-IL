#include "Pipe.h"
#include "WasteHandler.h"

std::shared_ptr<Liquid> Pipe::operator >> (std::shared_ptr<Liquid> in){
    return flow(in);
}

void Pipe::setHandler(std::shared_ptr<WasteHandler> handler){
    wasteHandler = handler;
}

void Pipe::setKeyword(std::shared_ptr<Keyword> keyword){
    keywordSpecific = keyword;
}