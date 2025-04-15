#include "Mask.h"
#include "debug.h"

// Constructor creates a mask with specified dimensions
// All mask values are initialized to false (not part of the mask)
Mask::Mask(int width, int height) : width(width), height(height) {
    // Allocate and initialize the 2D vector for mask data
    maskData.resize(height, std::vector<bool>(width, false));
    debug::log("Mask created with dimensions: " + std::to_string(width) + "x" + std::to_string(height));
}

// Creates a square-shaped mask centered at the given coordinates with specified size
void Mask::createSquare(int centerX, int centerY, int size) {
    debug::log("Creating square mask at (" + std::to_string(centerX) + ", " + std::to_string(centerY) + ") with size " + std::to_string(size));
    
    // Validate the size parameter
    if (size <= 0) {
        debug::log("Invalid size for square mask: " + std::to_string(size));
        return;
    }
    
    // Calculate the half size to determine square boundaries
    int halfSize = size / 2;
    
    // Set all pixels within the square boundaries to true (part of the mask)
    for (int y = centerY - halfSize; y <= centerY + halfSize; ++y) {
        for (int x = centerX - halfSize; x <= centerX + halfSize; ++x) {
            // Only set pixels that are within the mask dimensions
            if (x >= 0 && x < width && y >= 0 && y < height) {
                maskData[y][x] = true;
            }
        }
    }
    
    // Count the number of active pixels in the mask for debugging
    int count = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (maskData[y][x]) {
                count++;
            }
        }
    }
    debug::log("Square mask created with " + std::to_string(count) + " active pixels");
}

// Checks if a pixel at the given coordinates is inside the mask
// Returns true if the pixel is part of the mask, false otherwise
bool Mask::isInside(int x, int y) const {
    // Check if coordinates are within valid mask boundaries
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    return maskData[y][x];
}