#ifndef __H_FILEPIPESTRING__
#define __H_FILEPIPESTRING__

#include "Pipe.h"

class FilePipeString:public Pipe{
public:
    std::shared_ptr<Liquid> flow(std::shared_ptr<Liquid> in) const override;
    std::string locate(std::shared_ptr<Liquid> in,Location location) const override;
};

#endif