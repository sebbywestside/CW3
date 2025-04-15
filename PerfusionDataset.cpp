// PerfusionDataset.cpp - Implementation of PerfusionDataset class
#include "PerfusionDataset.h"
#include "debug.h"
#include "GraphicsUtil.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>

// Default constructor - initializes all member variables to default values
PerfusionDataset::PerfusionDataset()
 : arrivalFrame(-1),
   arrivalSignal(0.0f),
   peakFrame(-1),
   peakSignal(0.0f),
   temporalGradient(0.0f),
   lvbpMask(0, 0) {
    debug::log("PerfusionDataset object initialized");
}

// Loads a sequence of PGM images with numbered filenames
// baseFilename is the prefix for the image files (e.g., "mri-")
// numImages is the number of images to load
bool PerfusionDataset::loadImages(const std::string& baseFilename, int numImages) {
    debug::log("Loading " + std::to_string(numImages) + " images with base filename: " + baseFilename);
    frames.clear();
    bool success = true;
    
    for (int i = 1; i <= numImages && success; ++i) {
        std::string filename = baseFilename +
            (i < 10 ? "0" : "") + std::to_string(i) + ".pgm";
        debug::log("Loading image: " + filename);
        
        ImageFrame frame;
        if (frame.loadFromPGM(filename)) {
            frames.push_back(frame);
            debug::log("Successfully loaded image " + std::to_string(i));
        } else {
            debug::log("Failed to load image: " + filename);
            // If we're on the first image and it fails, consider it an error
            // Otherwise, we might have just reached the end of the sequence
            if (i == 1) success = false;
            break;
        }
    }
    
    debug::log("Loaded " + std::to_string(frames.size()) + " images in total");
    return success && !frames.empty();
}

// Creates a square mask for the Left Ventricular Blood Pool (LVBP) region
// centerX, centerY - center coordinates of the mask
// size - width/height of the square mask
void PerfusionDataset::createLVBPMask(int centerX, int centerY, int size) {
    debug::log("Creating LVBP mask at (" + std::to_string(centerX) + ", " +
        std::to_string(centerY) + ") with size " + std::to_string(size));
    
    if (frames.empty()) {
        debug::log("ERROR: Cannot create mask - no frames loaded");
        return;
    }
    
    // Create mask with same dimensions as the first image
    lvbpMask = Mask(frames[0].getWidth(), frames[0].getHeight());
    lvbpMask.createSquare(centerX, centerY, size);
    debug::log("LVBP mask created successfully");
}

// Calculates the mean signal intensity within the LVBP mask for each frame
// Results are stored in signalTimecourse vector
void PerfusionDataset::calculateTimecourse() {
    debug::log("Calculating signal timecourse in LVBP region");
    
    if (frames.empty()) {
        debug::log("ERROR: Cannot calculate timecourse - no frames loaded");
        return;
    }
    
    signalTimecourse.clear();
    signalTimecourse.reserve(frames.size());
    
    for (size_t i = 0; i < frames.size(); ++i) {
        float meanSignal = frames[i].calculateMeanInRegion(lvbpMask);
        signalTimecourse.push_back(meanSignal);
        debug::log("Frame " + std::to_string(i) + " mean signal: " + std::to_string(meanSignal));
    }
    
    debug::log("Signal timecourse calculation complete");
}

// Analyzes signal timecourse to find peak and arrival frames
// thresholdGradient - minimum signal gradient that indicates contrast arrival
void PerfusionDataset::findPeakAndArrival(float thresholdGradient) {
    debug::log("Finding peak and arrival frames with gradient threshold: " +
        std::to_string(thresholdGradient));
    
    if (signalTimecourse.empty()) {
        debug::log("ERROR: Cannot find peak/arrival - no timecourse data");
        return;
    }
    
    // Calculate gradient timecourse (frame-to-frame differences)
    gradientTimecourse.clear();
    gradientTimecourse.reserve(signalTimecourse.size() - 1);
    
    for (size_t i = 0; i < signalTimecourse.size() - 1; ++i) {
        float gradient = signalTimecourse[i + 1] - signalTimecourse[i];
        gradientTimecourse.push_back(gradient);
        debug::log("Frame " + std::to_string(i) + " gradient: " + std::to_string(gradient));
    }
    
    // Find peak frame 
    auto peakIter = std::max_element(signalTimecourse.begin(), signalTimecourse.end());
    peakFrame = std::distance(signalTimecourse.begin(), peakIter);
    peakSignal = *peakIter;
    debug::log("Peak frame: " + std::to_string(peakFrame) +
        ", signal: " + std::to_string(peakSignal));
    
    // Find first frame before peak where gradient exceeds threshold
    arrivalFrame = -1;
    for (int i = 0; i < peakFrame && i < gradientTimecourse.size(); ++i) {
        if (gradientTimecourse[i] > thresholdGradient) {
            arrivalFrame = i;
            arrivalSignal = signalTimecourse[i];
            debug::log("Found arrival frame: " + std::to_string(arrivalFrame) +
                ", signal: " + std::to_string(arrivalSignal));
            break;
        }
    }
    
    if (arrivalFrame == -1) {
        debug::log("WARNING: Could not find arrival frame with gradient > " +
            std::to_string(thresholdGradient));
    }
}

// Calculates the temporal gradient between contrast arrival and peak
// Represents the rate of contrast uptake in the LVBP region
void PerfusionDataset::calculateTemporalGradient() {
    debug::log("Calculating temporal gradient between arrival and peak");
    
    if (arrivalFrame >= 0 && peakFrame > arrivalFrame) {
        temporalGradient = (peakSignal - arrivalSignal) / (peakFrame - arrivalFrame);
        debug::log("Temporal gradient: " + std::to_string(temporalGradient) +
            " = (" + std::to_string(peakSignal) + " - " + std::to_string(arrivalSignal) +
            ") / (" + std::to_string(peakFrame) + " - " + std::to_string(arrivalFrame) + ")");
    } else {
        debug::log("ERROR: Cannot calculate temporal gradient - invalid arrival/peak frames");
        temporalGradient = 0.0f;
    }
}

// Displays the image frame that contains the peak contrast signal
void PerfusionDataset::displayPeakImage() const {
    debug::log("Displaying peak image (frame " + std::to_string(peakFrame) + ")");
    
    if (peakFrame >= 0 && peakFrame < frames.size()) {
        frames[peakFrame].displayOnTerminal();
    } else {
        debug::log("ERROR: Cannot display peak image - invalid frame index");
    }
}

// Plots the signal timecourse (mean intensity vs. frame number)
void PerfusionDataset::plotSignalTimecourse() const {
    debug::log("Plotting signal timecourse");
    
    if (signalTimecourse.empty()) {
        debug::log("ERROR: Cannot plot timecourse - no data");
        return;
    }
    
    // Calculate y limits for the plot
    float minSignal = *std::min_element(signalTimecourse.begin(), signalTimecourse.end());
    float maxSignal = *std::max_element(signalTimecourse.begin(), signalTimecourse.end());
    debug::log("Signal range: " + std::to_string(minSignal) + " to " + std::to_string(maxSignal));
    
    // Use GraphicsUtil to plot with yellow line (color index 2)
    GraphicsUtil::plotTimecourse(signalTimecourse, minSignal, maxSignal, 2, 20, 2);
}

// Plots the gradient timecourse (frame-to-frame signal differences)
void PerfusionDataset::plotGradientTimecourse() const {
    debug::log("Plotting gradient timecourse");
    
    if (gradientTimecourse.empty()) {
        debug::log("ERROR: Cannot plot gradient - no data");
        return;
    }
    
    // Calculate y limits for the plot
    float minGradient = *std::min_element(gradientTimecourse.begin(), gradientTimecourse.end());
    float maxGradient = *std::max_element(gradientTimecourse.begin(), gradientTimecourse.end());
    debug::log("Gradient range: " + std::to_string(minGradient) + " to " + std::to_string(maxGradient));
    
    // Use GraphicsUtil to plot with magenta line (color index 3)
    GraphicsUtil::plotTimecourse(gradientTimecourse, minGradient, maxGradient, 2, 10, 3);
}

// Displays key numerical results for the analysis
void PerfusionDataset::displayResults() const {
    debug::log("Displaying numerical results");
    std::cout << "Contrast arrival occurs at frame " << arrivalFrame
        << ", with signal intensity: " << std::fixed << std::setprecision(2) << arrivalSignal << std::endl;
    std::cout << "Peak contrast concentration occurs at frame " << peakFrame
        << ", with signal intensity: " << std::fixed << std::setprecision(2) << peakSignal << std::endl;
    std::cout << "Temporal gradient of signal during contrast update: "
        << std::fixed << std::setprecision(3) << temporalGradient << std::endl;
}