#pragma once

#include <vector>

class Mask {
private:
    std::vector<std::vector<bool>> maskData;
    int width;
    int height;

public:
    // Constructors
    Mask(int width, int height);
    
    // Mask creation methods
    void createSquare(int centerX, int centerY, int size);
    
    // Accessors
    bool isInside(int x, int y) const;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};