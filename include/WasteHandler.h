#ifndef __H_WASTEHANDLER__
#define __H_WASTEHANDLER__

#include "Pipe.h"
#include "Liquid.h"
#include <vector>
#include <string>

enum class ErrorLevel{
    OTHER,ERROR,WARNING,INFO
};

struct Waste{
    std::weak_ptr<Liquid> waste;
};

class WasteHandler{
public:

    void reset();
    bool hasError();
    void move(std::shared_ptr<Pipe> pipe,std::shared_ptr<Liquid> in);
    void handle(std::string message,Location location,ErrorLevel level=ErrorLevel::ERROR);
private:
    std::shared_ptr<Liquid> current;
    std::vector<std::weak_ptr<Pipe>> pipes;
    std::vector<std::string> errors;
};

#endif