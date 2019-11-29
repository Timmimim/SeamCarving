#ifndef ENERGYFUNCTIONS_HPP
#define ENERGYFUNCTIONS_HPP

#include <iostream>
#include <string>
#include <algorithm>
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
    static cv::Mat convertToGrayscale(const cv::Mat& image);

    static cv::Mat computeEnergy(const cv::Mat& image, const int choiceOperator);

private:
    static constexpr int horizontalSobelOperator[3][3] =
        {
           {-1, 0, 1},
           {-2, 0, 2},
           {-1, 0, 1}
       };
    static constexpr int verticalSobelOperator[3][3] =
       {
           {-1, -2, -1},
           { 0,  0,  0},
           { 1,  2,  1}
       };

    static constexpr int horizontalPrevittOperator[3][3] =
        {
           {-1, 0, 1},
           {-1, 0, 1},
           {-1, 0, 1}
       };
    static constexpr int verticalPrevittOperator[3][3] =
       {
           {-1, -1, -1},
           { 0,  0,  0},
           { 1,  1,  1}
       };

    template<typename T>
    static const T& clampImageBorder(const cv::Mat& image, int row, int col)
    {
        auto size = image.size();
        row = std::clamp(row, 0, size.height-1);
        col = std::clamp(col, 0, size.width-1);
        return image.at<T>(col,row);
    }
};

#endif // ENERGYFUNCTIONS_HPP
