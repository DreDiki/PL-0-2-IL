#ifndef __H_TREEPIPEDOT__
#define __H_TREEPIPEDOT__

#include "Pipe.h"
class TreePipeDOT : public Pipe{
public:
    std::shared_ptr<Liquid> flow(std::shared_ptr<Liquid> in) const override;
    std::string locate(std::shared_ptr<Liquid> in,Location location) const override;
};

#endif