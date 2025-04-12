#pragma once
#include <string>
#include <vector>
#include "ImageFrame.h"
#include "Mask.h"

class PerfusionDataset {
private:
    std::vector<ImageFrame> frames;
    std::vector<float> signalTimecourse;   
    std::vector<float> gradientTimecourse; 
    std::vector<Mask> masks;
    std::string datasetName;
    int arrivalFrame;
    float arrivalSignal;
    int peakFrame;
    float peakSignal;
    float temporalGradient;
    Mask lvbpMask;

public:
    PerfusionDataset();
    bool loadImages(const std::string& baseFilename, int numImages);
    void createLVBPMask(int centerX, int centerY, int size);
    void calculateTimecourse();  
    void findPeakAndArrival(float thresholdGradient);
    void calculateTemporalGradient();
    void displayPeakImage() const;
    void plotSignalTimecourse() const;  
    void plotGradientTimecourse() const; 
    void displayResults() const;
    int getArrivalFrame() const { return arrivalFrame; }
    float getArrivalSignal() const { return arrivalSignal; }
    int getPeakFrame() const { return peakFrame; }  
    float getPeakSignal() const { return peakSignal; }
    float getTemporalGradient() const { return temporalGradient; }
};