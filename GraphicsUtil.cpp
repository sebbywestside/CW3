#include "GraphicsUtil.h"
#include "terminal_graphics.h"
#include "debug.h"
#include <algorithm>

// Displays an ImageFrame object in the terminal using the terminal_graphics library
void GraphicsUtil::displayImage(const ImageFrame& image) {
    debug::log("Displaying image on terminal");
    
    // Create a lambda function adapter to access ImageFrame pixel data
    // This provides a consistent interface for the terminal graphics library
    auto imageAdapter = [&image](int x, int y) -> int {
        return image.getPixel(x, y);
    };
    
    // Create a wrapper class that interfaces with TG::imshow
    // This adapts our ImageFrame to the interface expected by terminal_graphics
    struct ImageView {
        decltype(imageAdapter) adapter;  // Store the lambda function
        int w, h;                        // Width and height of the image
        int width() const { return w; }  // Interface method required by TG::imshow
        int height() const { return h; } // Interface method required by TG::imshow
        int operator()(int x, int y) const { return adapter(x, y); } // Pixel access operator
    };
    
    // Create an ImageView instance with our adapter and image dimensions
    ImageView view = { imageAdapter, image.getWidth(), image.getHeight() };
    
    // Use the image's max value for proper scaling in the display
    int maxVal = image.getMaxValue();
    
    // Display the image using the terminal_graphics library
    // Parameters: image view, minimum value (0), maximum value
    TG::imshow(view, 0, maxVal);
}

// Plots a time series of values in the terminal
// Parameters:
//   values - Vector of y-values to plot
//   minY/maxY - Y-axis range limits
//   gridX/gridY - Grid spacing for X and Y axes
//   colorIndex - Color to use for the plot line
void GraphicsUtil::plotTimecourse(const std::vector<float>& values,
                                float minY, float maxY,
                                float gridX, float gridY,
                                int colorIndex) {
    // Create x-axis values (frame numbers)
    // Each value corresponds to its index in the input vector
    std::vector<float> xValues(values.size());
    for (size_t i = 0; i < xValues.size(); ++i) {
        xValues[i] = i;
    }
    
    // Add a 10% margin to the Y-axis range for better visualization
    float yMargin = (maxY - minY) * 0.1f;
    
    // Create and configure plot using the terminal_graphics library
    // Method chaining is used to set various plot parameters
    TG::plot(512, 200)                             // Create plot with width 512 and height 200
        .set_xlim(0, values.size() - 1)            // Set X-axis limits
        .set_ylim(minY - yMargin, maxY + yMargin)  // Set Y-axis limits with margins
        .set_grid(gridX, gridY)                    // Set grid spacing
        .add_line(xValues, values, colorIndex);    // Add the actual data line with specified color
}