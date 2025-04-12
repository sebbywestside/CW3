#include <iostream>
#include <fstream>
#include <sstream>

#include "ImageFrame.h"
#include "debug.h"
#include "Mask.h"


ImageFrame::ImageFrame() : width(0), height(0), maxValue(0) {} 

ImageFrame::ImageFrame(int width, int height, int maxValue)
    : width(width), height(height), maxValue(maxValue) {
    pixelData.resize(height, std::vector<int>(width));
}

bool ImageFrame::loadFromPGM(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        debug::log("Failed to open file: " + filename);
        return false;
    }
    debug::log("Loading PGM file: " + filename);

    // Read PGM Header 
    std::string line;
    std::getline(file, line); 
    if (line != "P2") {
        debug::log("Invalid PGM format: " + line);
        return false;
    }

    // Read width, height, and max value
    file >> width >> height >> maxValue;
    if (width <= 0 || height <= 0) {
        std::cerr << "Inavlid image dimensions: " << width << "x" << height << std::endl;
        return false;
    }
    debug::log("Image dimensions: " + std::to_string(width) + "x" + std::to_string(height));
    debug::log("Max pixel value: " + std::to_string(maxValue));

    pixelData.resize(height, std::vector<int>(width));
    if (maxValue <= 0) {
        std::cerr << "Invalid max value: " << maxValue << std::endl;
        return false;
    }
    // Read pixel data
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            file >> pixelData[y][x];
        }
    }
    debug::log("Successfully loaded data: " + filename);
    file.close();
    return true;
}

int ImageFrame::getPixel(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        std::cerr << "Pixel coordinates out of bounds: (" << x << ", " << y << ")" << std::endl;
        return 0;
    }
    return pixelData[y][x];
}

float ImageFrame::calculateMeanInRegion(const Mask& mask) const {
    int sum = 0;
    int count = 0;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (mask.isInside(x, y) == 1) {
                sum += pixelData[y][x];
                count++;
            }
        }
    }

    float mean = 0.0f;
    if (count > 0) {
        mean = static_cast<float>(sum) / count;
    } else {
        mean = 0.0f;
    }
    debug::log("Region mean calculation: sum =" + std::to_string(sum) + ", count =" + std::to_string(count) + ",mean=" + std::to_string(mean));
    return mean;
}

void ImageFrame::displayOnTerminal() const{
    std::cout << "Image" << width << "x" << height << std::endl;
    
    debug::log("Displaying image on terminal");

    for (int y = 0; y < height; y += 4) {
        for (int x = 0; x < width; x += 2) {
            int value = pixelData[y][x];
            char c = ' ';
            if (value < maxValue / 4) c = ' ';
            else if (value < maxValue / 2) c = '.';
            else if (value < 3 * maxValue / 4) c = 'o';
            else c = 'O';
            std::cout << c;
        }
        std::cout << std::endl;
    }
}
