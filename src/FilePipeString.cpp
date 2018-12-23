#include "FilePipeString.h"

#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
#include "Liquids.h"
#include "util.h"

std::shared_ptr<Liquid> FilePipeString::flow(std::shared_ptr<Liquid> in) const{
    auto filename = in->toStringLiquid();
    std::ifstream file;
    file.open(filename->value);
    if(!file.is_open()){
        return nullptr;
    }
    debug("Opening file %s",filename->value.c_str());

    std::string s{std::istreambuf_iterator<char>(file), {}};
    //remove /r in file
    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
    auto result = std::make_shared<StringLiquid>();
    result->value = s;
    return result;
}

std::string FilePipeString::locate(std::shared_ptr<Liquid> in,Location location) const{
    auto fin = in->toStringLiquid();
    return fin->value;
}