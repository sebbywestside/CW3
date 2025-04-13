#include "Mask.h"
#include "debug.h"
// Creating mask and displaying debug messages
Mask::Mask(int width, int height) : width(width), height(height) {
    maskData.resize(height, std::vector<bool>(width, false));
    debug::log("Mask created with dimensions: " + std::to_string(width) + "x" + std::to_string(height));
}

// 
void Mask::createSquare(int centerX, int centerY, int size) {
    debug::log("Creating square mask at (" + std::to_string(centerX) + ", " + std::to_string(centerY) + ") with size " + std::to_string(size));
    if (size <= 0) {
        debug::log("Invalid size for square mask: " + std::to_string(size));
        return;
    }
    int halfSize = size / 2;
    for (int y = centerY - halfSize; y <= centerY + halfSize; ++y) {
        for (int x = centerX - halfSize; x <= centerX + halfSize; ++x) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                maskData[y][x] = true;
            }
        }
    }
    int count = 0;
    for (int y = 0; y < height; ++y){
         for ( int x = 0; x < width; ++x){
                if (maskData[y][x]){
                    count++;
                }
         }
    }
    debug::log("Square mask created with " + std::to_string(count) + " active pixels");   
}

bool Mask::isInside(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    return maskData[y][x];
