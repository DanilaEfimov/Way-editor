#include "Parser.h"
#include<fstream>

Parser::Parser() {}

Parser::~Parser() {

}

uint Parser::getType(std::string path) const {
    std::fstream graphFile(path);
    std::string type = "";
    std::getline(graphFile, type);
    if(types.find(type) == types.end()){
        return -1;
    }
    else {
        std::map<std::string, int>::const_iterator it = types.find(path);
        return it->second;
    }
    return -1;
}
