// main.cpp
#include <iostream>
#include <string>
#include "ImageFrame.h"
#include "Mask.h"
#include "ContrastAgent.h"
#include "PerfusionDataset.h"
#include "debug.h"

int main() {
    // Enable/Disable debug logging
    debug::verbose = false;
    debug::log("Program started");
    
    // Load contrast agent info
    debug::log("Loading contrast agent information");
    ContrastAgent contrast;
    if (!contrast.loadFromFile("contrast_info.txt")) {
        std::cerr << "Failed to load contrast info" << std::endl;
        debug::log("ERROR: Failed to load contrast info, exiting");
        return 1;
    }
    
    // Load MRI perfusion dataset
    debug::log("Loading MRI perfusion dataset");
    PerfusionDataset dataset;
    if (!dataset.loadImages("mri-", 20)) { 
        std::cerr << "Failed to load MRI images" << std::endl;
        debug::log("ERROR: Failed to load MRI images, exiting");
        return 1;
    }
    
    // Create LVBP mask (centered at (74, 90) with 5x5 size)
    debug::log("Creating LVBP mask");
    dataset.createLVBPMask(74, 90, 5);
    
    // Calculate signal timecourse
    debug::log("Calculating signal timecourse");
    dataset.calculateTimecourse();
    
    // Find peak and arrival frames
    debug::log("Finding peak and arrival frames");
    dataset.findPeakAndArrival(10.0f);
    
    // Calculate temporal gradient
    debug::log("Calculating temporal gradient");
    dataset.calculateTemporalGradient();
    
    // Display results
    debug::log("Displaying results");
    
    std::cout << "Image at peak contrast concentration:" << std::endl;
    dataset.displayPeakImage();
    
    std::cout << "Signal timecourse within ROI:" << std::endl;
    dataset.plotSignalTimecourse();
    
    std::cout << "Gradient of signal timecourse within ROI:" << std::endl;
    dataset.plotGradientTimecourse();
    
    // Display contrast agent info
    std::cout << "Contrast agent: " << contrast.getName() 
              << ", dose = " << contrast.getDose() << std::endl;
              
    // Display numerical results
    dataset.displayResults();
    
    debug::log("Program completed successfully");
    return 0;
}