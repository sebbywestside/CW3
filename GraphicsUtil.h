#pragma once
#include "ImageFrame.h"
#include <vector>

class GraphicsUtil {
public:
    // Image display
    static void displayImage(const ImageFrame& image);
    
    // Plot timecourse data
    static void plotTimecourse(const std::vector<float>& values, 
                              float minY, float maxY, 
                              float gridX, float gridY,
                              int colorIndex);
};