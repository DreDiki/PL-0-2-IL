#ifndef __H_LIQUID__
#define __H_LIQUID__
#include "util.h"

#include <string>
#include <memory>
enum class LiquidType{

};

struct Location{
    Location();
    Location(long);
    Location(long,long);
    long begin;
    long end;
};

class StringLiquid;
class TokensLiquid;
class TreeLiquid;
class SimpleLiquid;
class Liquid : public std::enable_shared_from_this<Liquid>{
public:
    
    virtual std::string toString() const;
    //virtual LiquidType type() const;

    //from location -- get info from last source
    //to location   -- get info from 

    //location in parent liquid
    void setLocation(Location loc);
    Location getLocation() const;

    DConvert2(StringLiquid);
    DConvert2(TokensLiquid);
    DConvert2(TreeLiquid);
    DConvert2(SimpleLiquid);
protected:
    Location location;
};

#endif