#include "WasteHandler.h"
#include "util.h"

void WasteHandler::reset(){
    errors.clear();
    pipes.clear();
}
bool WasteHandler::hasError(){
    return !errors.empty();
}
void WasteHandler::move(std::shared_ptr<Pipe> pipe,std::shared_ptr<Liquid> in){
    pipes.push_back(pipe);
    current = in;
}

void WasteHandler::handle(std::string message,Location location,ErrorLevel level){
    auto pipe = pipes[pipes.size()-2].lock();
    std::string reason;
    if(pipe)
        reason = pipe->locate(current,location);
    error("Error happened:\n%s\nAt:%ld,%s",message.c_str(),location.begin,reason.c_str());
    errors.push_back("...not implemented");
    //pipe->locate(current,location);
    ///pipes[pipes.size()-2].
}