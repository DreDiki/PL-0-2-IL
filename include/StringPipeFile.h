#ifndef __H_STRINGPIPEFILE__
#define __H_STRINGPIPEFILE__

#include "Pipe.h"

class StringPipeFile:public Pipe{
public:
    std::shared_ptr<Liquid> flow(std::shared_ptr<Liquid> in) const override;
    std::string locate(std::shared_ptr<Liquid> in,Location location) const override;
};

#endif
