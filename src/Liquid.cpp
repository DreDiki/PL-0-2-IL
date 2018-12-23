#include "Liquid.h"
#include "Liquids.h"
std::string Liquid::toString() const{
    return "Blank Liquid";
}

void Liquid::setLocation(Location loc){
    location = loc;
}
Location Liquid::getLocation() const {
    return location;
}

Convert2(StringLiquid,Liquid);
Convert2(TokensLiquid,Liquid);
Convert2(TreeLiquid,Liquid);
Convert2(SimpleLiquid,Liquid);

Location::Location():Location(0,0){
    
}
Location::Location(long b):Location(b,0){

}
Location::Location(long b,long e):begin(b),end(e){

}