#include "ContrastAgent.h"
#include "debug.h"
#include <fstream>  
#include <iostream>

ContrastAgent::ContrastAgent() : name(""), dose(0.0f){
    debug::log("ContrastAgent object initialized");
}

bool ContrastAgent::loadFromFile(const std::string& filename){
    debug::log("Attenmpting to load contrast agent data from file: " + filename);

    std::ifstream file(filename);
    if (!file.is_open()){
        debug::log("Failed to open file: " + filename);
        return false;
    }

    std::getline(file, name);
    if (name.empty()){
        debug::log("Failed to read contrast agent name from file: " + filename);
        return false;
   
    }

    file >> dose;
    debug::log("Contrast agent name: " + name);
    debug::log("Contrast agent dose: " + std::to_string(dose));
    if (dose <= 0.0f){
        debug::log("Invalid dose value: " + std::to_string(dose));
        return false;
    }
    debug::log("Successfully loaded contrast agent data from file: " + filename);
    file.close();
    return true;
}