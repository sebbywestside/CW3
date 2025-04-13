#include "GraphicsUtil.h"
#include "terminal_graphics.h"
#include "debug.h"
#include <algorithm>

void GraphicsUtil::displayImage(const ImageFrame& image) {
    debug::log("Displaying image on terminal");
    
    // Create adapter to access ImageFrame pixel data
    auto imageAdapter = [&image](int x, int y) -> int {
        return image.getPixel(x, y);
    };
    
    // Create a wrapper class that interfaces with TG::imshow
    struct ImageView {
        decltype(imageAdapter) adapter;
        int w, h;
        
        int width() const { return w; }
        int height() const { return h; }
        int operator()(int x, int y) const { return adapter(x, y); }
    };
    
    ImageView view = { imageAdapter, image.getWidth(), image.getHeight() };
    
    // Use the image's max value for the range
    int maxVal = image.getMaxValue();
    
    // Display the image using TG::imshow
    TG::imshow(view, 0, maxVal);
}

void GraphicsUtil::plotTimecourse(const std::vector<float>& values, 
                                float minY, float maxY, 
                                float gridX, float gridY,
                                int colorIndex) {
    // Create x-axis (frame numbers)
    std::vector<float> xValues(values.size());
    for (size_t i = 0; i < xValues.size(); ++i) {
        xValues[i] = i;
    }
    
    float yMargin = (maxY - minY) * 0.1f;  // 10% margin
    
    // Plot using TG library
    TG::plot(512, 200)
        .set_xlim(0, values.size() - 1)
        .set_ylim(minY - yMargin, maxY + yMargin)
        .set_grid(gridX, gridY)
        .add_line(xValues, values, colorIndex);
}