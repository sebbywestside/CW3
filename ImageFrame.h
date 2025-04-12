#pragma once

#include <vector>
#include <string>
class Mask;

class ImageFrame
{
    public:
        ImageFrame();
        ImageFrame(int width, int height, int maxValue);
        bool loadFromPGM(const std::string& filename);
        int getPixel(int x, int y) const;
        int getWidth() const {return width;}
        int getHeight() const {return height;}
        int getMaxValue() const {return maxValue;}

        float calculateMeanInRegion(const Mask& mask) const;

        void displayOnTerminal() const;
        
    private:
       std::vector<std::vector<int>> pixelData;
       int width;
       int height;
       int maxValue;
};