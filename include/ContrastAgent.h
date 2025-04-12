#pragma once 
#include <string>

class ContrastAgent{
    private:
    std::string name;
    float dose;

    public:
    ContrastAgent();
    bool loadFromFile(const std::string& filename);
    
    std::string getName() const {return name; }
    float getDose() const {return dose; }
};