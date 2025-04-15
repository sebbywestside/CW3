#include <iostream>
#include <fstream>
#include <sstream>
#include "ImageFrame.h"
#include "debug.h"
#include "Mask.h"
#include "GraphicsUtil.h"

// Default constructor initializes an empty image frame
ImageFrame::ImageFrame() : width(0), height(0), maxValue(0) {}

// Parameterized constructor creates an image frame with specified dimensions and max value
ImageFrame::ImageFrame(int width, int height, int maxValue)
 : width(width), height(height), maxValue(maxValue) {
    // Allocate memory for the pixel data based on the image dimensions
    pixelData.resize(height, std::vector<int>(width));
}

// Loads an image from a PGM file format
bool ImageFrame::loadFromPGM(const std::string& filename) {
    // Open the file for reading
    std::ifstream file(filename);
    if (!file.is_open()) {
        debug::log("Failed to open file: " + filename);
        return false;
    }
    debug::log("Loading PGM file: " + filename);

    // Read PGM Header 
    std::string magic;
    file >> magic;
    if (magic != "P2") {
        debug::log("Invalid PGM format: " + magic);
        return false;
    }

    // Read image dimensions and maximum pixel value from the header
    file >> width >> height >> maxValue;
    if (width <= 0 || height <= 0) {
        std::cerr << "Invalid image dimensions: " << width << "x" << height << std::endl;
        return false;
    }
    debug::log("Image dimensions: " + std::to_string(width) + "x" + std::to_string(height));
    debug::log("Max pixel value: " + std::to_string(maxValue));

    // Allocate memory for the pixel data based on the image dimensions
    pixelData.resize(height, std::vector<int>(width));
    if (maxValue <= 0) {
        std::cerr << "Invalid max value: " << maxValue << std::endl;
        return false;
    }

    // Read the pixel data values into our 2D vector
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            file >> pixelData[y][x];
        }
    }
    debug::log("Successfully loaded data: " + filename);
    file.close();
    return true;
}

// Returns the pixel value at the specified coordinates
// Returns 0 if coordinates are out of bounds
int ImageFrame::getPixel(int x, int y) const {
    // Check if coordinates are within valid image boundaries
    if (x < 0 || x >= width || y < 0 || y >= height) {
        std::cerr << "Pixel coordinates out of bounds: (" << x << ", " << y << ")" << std::endl;
        return 0;
    }
    return pixelData[y][x];
}

// Calculates the mean pixel value within a region defined by a mask
float ImageFrame::calculateMeanInRegion(const Mask& mask) const {
    int sum = 0;
    int count = 0;
    
    // Iterate through all pixels in the image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Only include pixels that are inside the mask
            if (mask.isInside(x, y) == 1) {
                sum += pixelData[y][x];
                count++;
            }
        }
    }
    
    // Calculate the mean, avoiding division by zero
    float mean = 0.0f;
    if (count > 0) {
        mean = static_cast<float>(sum) / count;
    } else {
        mean = 0.0f;
    }
    debug::log("Region mean calculation: sum =" + std::to_string(sum) + 
               ", count =" + std::to_string(count) + 
               ",mean=" + std::to_string(mean));
    return mean;
}

// Displays the image in the terminal
void ImageFrame::displayOnTerminal() const {
    debug::log("Displaying image on terminal");
    // Delegate to GraphicsUtil for actual display implementation
    GraphicsUtil::displayImage(*this);
}