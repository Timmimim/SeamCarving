#ifndef ENERGYFUNCTIONS_HPP
#define ENERGYFUNCTIONS_HPP

#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class EnergyFunctions
{
public:
    EnergyFunctions();

    /**
      * @brief convertToGrayscale Converts images from 8UC3 (8-Bit depth unsigned char with 3 channels) to 8UC1 (same with only one channel) by averaging RGB channels pixel-wise.
      *
      * @param image Original 8UC3 image matrix / tensor / whatchamacallit.
      * @return Averaged 8UC1 (grayscale) image.
      */
    static cv::Mat convertToGrayscale(const cv::Mat image);

    static cv::Mat foldWithHorizontalOperator(const cv::Mat& image, const int choiceOperator);
    static cv::Mat foldWithVerticalOperator(const cv::Mat& image, const int choiceOperator);

private:
    static int horizontalSobelOperator[3][3];
    static int verticalSobelOperator[3][3];
    static int horizontalPrevittOperator[3][3];
    static int verticalPrevittOperator[3][3];
};

#endif // ENERGYFUNCTIONS_HPP
